#!/usr/bin/env bash

set -euo pipefail

git fetch origin ci-base
git fetch --filter=blob:none upstream master

git checkout -B master origin/master
git merge --ff-only upstream/master

git checkout -B ci-base origin/ci-base
old_ci_base=$(git rev-parse HEAD)

if git merge-base --is-ancestor master HEAD; then
  echo "ci-base already contains current master"
else
  merge_status=0
  git merge --no-commit --no-ff master || merge_status=$?

  if ((merge_status != 0)); then
    if ! git rev-parse --verify -q MERGE_HEAD >/dev/null; then
      echo "::error::git merge failed without starting a merge"
      exit "$merge_status"
    fi

    conflict_count=0
    unexpected_conflict=0
    while IFS= read -r -d '' path; do
      ((conflict_count += 1))
      if [[ "$path" != .github/* ]]; then
        echo "::error file=$path::unexpected merge conflict"
        unexpected_conflict=1
      fi
    done < <(git diff --name-only --diff-filter=U -z --)

    if ((conflict_count == 0)); then
      echo "::error::git merge failed without conflicts"
      git merge --abort
      exit "$merge_status"
    fi

    if ((unexpected_conflict != 0)); then
      git merge --abort
      exit 1
    fi
  fi

  # ci-base owns .github. Discard upstream's version and restore ours.
  git rm -r -f --ignore-unmatch -- .github
  git restore --source="$old_ci_base" --staged --worktree -- .github

  if [[ -n $(git ls-files -u --) ]]; then
    echo "::error::unresolved merge conflicts remain"
    git merge --abort
    exit 1
  fi

  if ! git diff --cached --quiet "$old_ci_base" -- .github; then
    echo "::error::.github does not match its pre-merge state"
    git merge --abort
    exit 1
  fi

  git commit -m "Merge upstream master into ci-base"
fi

git push --atomic origin master:master ci-base:ci-base
