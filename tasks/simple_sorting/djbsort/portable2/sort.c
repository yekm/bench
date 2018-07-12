#include "int32_sort.h"
#define int32 int32_t

static inline int32 minmax(int32 a,int32 *y)
{
  int32 b = *y;
  int32 ab = b ^ a;
  int32 c = b - a;
  c ^= ab & (c ^ b);
  c >>= 31;
  c &= ab;
  a ^= c;
  *y = b ^ c;
  return a;
}

/* precondition: n >= 2 */
/* precondition: m = ceil(n/2) */
/* inputs: x[0],...,x[n-1] */
/* outputs: x[0],x[m],x[1],x[m+1],... */
static void outshuffle(int32 *x,long long n,long long m)
{
  int32 y[m];
  long long j;
  for (j = 0;j < m;++j) y[j] = x[j];
  for (j = m;j < n;++j) x[(j - m) * 2 + 1] = x[j];
  for (j = 0;j < m;++j) x[j * 2] = y[j];
}

/* precondition: n >= 2 */
/* precondition: m = ceil(n/2) */
/* precondition: x[0]...x[m-1] are in order */
/* precondition: x[m]...x[n-1] are in order */
/* postcondition: x[0]...x[n-1] are in order */
/* (and are same multiset as original inputs) */
static void merge(int32 *x,long long n,long long m)
{
  long long q = 1;
  long long p;
  int32 *xj;
  int32 *y;

  y = x + n - m;
  for (xj = x;xj != y;++xj)
    *xj = minmax(*xj,xj + m);

  xj = x;
  while (q < m) q <<= 1;
  while (q >>= 1) {
    y = x + m - q;
    for (;xj != y;++xj) {
      int32 a = xj[m];
      p = q;
      do
        a = minmax(a,xj + p);
      while (p >>= 1);
      xj[m] = a;
    }
  }
  outshuffle(x,n,m);
}

void int32_sort(int32 *x,long long n)
{
  long long m;
  if (n < 2) return;
  m = (n + 1) >> 1; /* assumes n below 2^63-1 */
  int32_sort(x,m);
  int32_sort(x + m,n - m);
  merge(x,n,m);
}
