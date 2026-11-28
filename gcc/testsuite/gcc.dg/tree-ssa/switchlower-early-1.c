/* A switch whose controlling value is loop invariant is lowered before the
   late threaders so that they do not duplicate the case chain once per case
   value (PR tree-optimization/128962).  */
/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-switchlower_early" } */

int f (int *a, int n, int k)
{
  int s = 0;
  for (int i = 0; i < n; i++)
    {
      switch (k)
	{
	case 1:
	  s += a[i] > 0 ? 1 : -1;
	  break;
	case 2:
	  s += a[i] > 0 ? 2 : -2;
	  break;
	default:
	  s += a[i] > 0 ? 3 : -3;
	  break;
	}
    }
  return s;
}

/* { dg-final { scan-tree-dump "SWITCH statement" "switchlower_early" } } */
