/* A switch on a value that changes in the loop is left for the late
   threaders, they can make use of it (PR tree-optimization/128102).  */
/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-switchlower_early" } */

int f (int *a, int n)
{
  int s = 0;
  for (int i = 0; i < n; i++)
    {
      switch (a[i])
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

/* { dg-final { scan-tree-dump-not "SWITCH statement" "switchlower_early" } } */
