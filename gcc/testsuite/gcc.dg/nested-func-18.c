/* A call replaced by a peephole2 or split must keep its REG_EH_REGION
   note, which says it cannot perform a nonlocal goto.  On alpha the
   __divti3 libcall is replaced by a peephole2.  */
/* { dg-do compile } */
/* { dg-options "-O2" } */
/* { dg-require-effective-target int128 } */

__int128
g (__int128 x, __int128 y)
{
  __label__ l;
  __attribute__ ((noinline)) void f (int c) { if (c) goto l; }
  x = x / y;
  f (y != 0);
  return x;
 l:
  return y;
}
