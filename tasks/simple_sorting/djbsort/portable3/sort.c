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

void int32_sort(int32 *x,long long n)
{
  long long top,p,q,r,i;

  if (n < 2) return;
  top = 1;
  while (top < n - top) top += top;

  for (p = top;p > 0;p >>= 1) {
    for (i = 0;i < n - p;++i)
      if (!(i & p))
        x[i] = minmax(x[i],x + i + p);
    i = 0;
    for (q = top;q > p;q >>= 1) {
      for (;i < n - q;++i) {
        if (!(i & p)) {
          int32 a = x[i + p];
          for (r = q;r > p;r >>= 1)
            a = minmax(a,x + i + r);
	  x[i + p] = a;
	}
      }
    }
  }
}
