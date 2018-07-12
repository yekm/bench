#include "int32_sort.h"
#define int32 int32_t

static void minmax(int32 *x,int32 *y)
{
  int32 a = *x;
  int32 b = *y;
  int32 ab = b ^ a;
  int32 c = b - a;
  c ^= ab & (c ^ b);
  c >>= 31;
  c &= ab;
  *x = a ^ c;
  *y = b ^ c;
}

static void minmax_vector(int32 *x,int32 *y,long long n)
{
  while (n-- > 0) minmax(x++,y++);
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
  long long p = 1;
  while (p < m) p <<= 1;
  minmax_vector(x,x + m,n - m);
  while (p >>= 1)
    minmax_vector(x + m,x + p,m - p);
    /* 0-1 invariant: pairs (x[0],x[m]) ... (x[n-m-1],x[n-1]) */
    /* are (0,0)* (0,1)* (1,1)* with at most p copies of (0,1) */
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
