//#include "int32_sort.h"
#include <immintrin.h>
#define int32 int32_t

static inline int32 int32_minmax(int32 x,int32 *y)
{
  asm("\
movl (%1),%%ebx;\
cmpl %%ebx,%%eax;\
mov %%eax,%%edx;\
cmovg %%ebx,%%eax;\
cmovg %%edx,%%ebx;\
movl %%ebx,(%1)\
" : "+a"(x) : "r"(y) : "%ebx","%edx");
  return x;
}

typedef __m256i int32x8;
#define int32x8_load(z) _mm256_loadu_si256((__m256i *) (z))
#define int32x8_store(z,i) _mm256_storeu_si256((__m256i *) (z),(i))
#define int32x8_min _mm256_min_epi32
#define int32x8_max _mm256_max_epi32

static inline int32x8 int32x8_minmax(int32x8 a,int32 *y)
{
  int32x8 b = int32x8_load(y);
  int32x8_store(y,int32x8_max(a,b));
  return int32x8_min(a,b);
}

static inline void minmax_vector_simple(int32 *x,int32 *y,long long n)
{
  while (n > 0) {
    *x = int32_minmax(*x,y);
    ++x;
    ++y;
    --n;
  }
}

/* n >= 16 */
static inline void minmax_vector_large(int32 *x,int32 *y,long long n)
{
  int32x8 a, b, c, d;

  if (n & 15) {
    a = int32x8_load(x + n - 16);
    b = int32x8_load(x + n - 8);
    a = int32x8_minmax(a,y + n - 16);
    b = int32x8_minmax(b,y + n - 8);
    int32x8_store(x + n - 16,a);
    int32x8_store(x + n - 8,b);
    n &= ~15;
  }
  while (n >= 32) {
    a = int32x8_load(x);
    b = int32x8_load(x + 8);
    c = int32x8_load(x + 16);
    d = int32x8_load(x + 24);
    a = int32x8_minmax(a,y);
    b = int32x8_minmax(b,y + 8);
    c = int32x8_minmax(c,y + 16);
    d = int32x8_minmax(d,y + 24);
    int32x8_store(x,a);
    int32x8_store(x + 8,b);
    int32x8_store(x + 16,c);
    int32x8_store(x + 24,d);
    x += 32;
    y += 32;
    n -= 32;
  }
  if (n) {
    a = int32x8_load(x);
    b = int32x8_load(x + 8);
    a = int32x8_minmax(a,y);
    b = int32x8_minmax(b,y + 8);
    int32x8_store(x,a);
    int32x8_store(x + 8,b);
  }
}

static inline void merge_to_simple(int32 *y,int32 *x,long long n,long long m)
{
  long long p, q, todo;

  if (n < 1) return;
  if (n == 1) { *y = *x; return; }

  for (p = 1;p < m;p <<= 1) ;
  todo = m - p;
  p >>= 1;
  todo += p;

  n -= m;
  minmax_vector_simple(x,x + m,n);

  while (p) {
    while (todo) {
      int32 a = x[m];
      q = p;
      do
        a = int32_minmax(a,x + q);
      while (q >>= 1);
      x[m] = a;
      x += 1; todo -= 1;
    }
    p >>= 1;
    todo += p;
  }
  x -= m - 1;
  if (n < m) y[n * 2] = x[n];
  while (n > 0) {
    y[0] = x[0];
    y[1] = x[m];
    x += 1;
    y += 2;
    n -= 1;
  }
}

/* m == (n + 1) / 2 */
static void merge_to(int32 *y,int32 *x,long long n,long long m)
{
  int32x8 a01234567, b01234567, c01234567, d01234567, e01234567, f01234567, g01234567, h01234567;
  long long p, q, todo;
  int32 *xm;

  if (n < 48) {
    merge_to_simple(y,x,n,m);
    return;
  }

  for (p = 1;p < m;p <<= 1) ;
  todo = m - p;
  p >>= 1;
  todo += p;

  n -= m;
  minmax_vector_large(x,x + m,n);

  if (m - p < 16) {
    minmax_vector_simple(x + m,x + p,m - p);
    p >>= 1;
    todo += p;
  }

  x += m - 16;
  e01234567 = int32x8_load(x);
  f01234567 = int32x8_load(x + 8);
  q = p;
  do {
    xm = x + m - q;
    a01234567 = int32x8_load(xm);
    b01234567 = int32x8_load(xm + 8);
    int32x8_store(xm,int32x8_min(a01234567,e01234567));
    int32x8_store(xm + 8,int32x8_min(b01234567,f01234567));
    e01234567 = int32x8_max(a01234567,e01234567);
    f01234567 = int32x8_max(b01234567,f01234567);
  } while (q >>= 1);
  int32x8_store(x,e01234567);
  int32x8_store(x + 8,f01234567);
  x -= m - 16;

  xm = x + m + 32;

  for (;;) {
    while (todo >= 64) {
      a01234567 = int32x8_load(xm - 32);
      b01234567 = int32x8_load(xm - 24);
      c01234567 = int32x8_load(xm - 16);
      d01234567 = int32x8_load(xm - 8);
      e01234567 = int32x8_load(xm);
      f01234567 = int32x8_load(xm + 8);
      g01234567 = int32x8_load(xm + 16);
      h01234567 = int32x8_load(xm + 24);
      q = p;
      do {
        a01234567 = int32x8_minmax(a01234567,x + q);
        b01234567 = int32x8_minmax(b01234567,x + q + 8);
        c01234567 = int32x8_minmax(c01234567,x + q + 16);
        d01234567 = int32x8_minmax(d01234567,x + q + 24);
        e01234567 = int32x8_minmax(e01234567,x + q + 32);
        f01234567 = int32x8_minmax(f01234567,x + q + 40);
        g01234567 = int32x8_minmax(g01234567,x + q + 48);
        h01234567 = int32x8_minmax(h01234567,x + q + 56);
      } while (q >>= 1);
      int32x8_store(xm - 32,a01234567);
      int32x8_store(xm - 24,b01234567);
      int32x8_store(xm - 16,c01234567);
      int32x8_store(xm - 8,d01234567);
      int32x8_store(xm,e01234567);
      int32x8_store(xm + 8,f01234567);
      int32x8_store(xm + 16,g01234567);
      int32x8_store(xm + 24,h01234567);
      x += 64; xm += 64; todo -= 64;
    }
    if (todo >= 32) {
      a01234567 = int32x8_load(xm - 32);
      b01234567 = int32x8_load(xm - 24);
      c01234567 = int32x8_load(xm - 16);
      d01234567 = int32x8_load(xm - 8);
      q = p;
      do {
        a01234567 = int32x8_minmax(a01234567,x + q);
        b01234567 = int32x8_minmax(b01234567,x + q + 8);
        c01234567 = int32x8_minmax(c01234567,x + q + 16);
        d01234567 = int32x8_minmax(d01234567,x + q + 24);
      } while (q >>= 1);
      int32x8_store(xm - 32,a01234567);
      int32x8_store(xm - 24,b01234567);
      int32x8_store(xm - 16,c01234567);
      int32x8_store(xm - 8,d01234567);
      x += 32; xm += 32; todo -= 32;
    }
    if (todo >= 16) {
      a01234567 = int32x8_load(xm - 32);
      b01234567 = int32x8_load(xm - 24);
      q = p;
      do {
        a01234567 = int32x8_minmax(a01234567,x + q);
        b01234567 = int32x8_minmax(b01234567,x + q + 8);
      } while (q >>= 1);
      int32x8_store(xm - 32,a01234567);
      int32x8_store(xm - 24,b01234567);
      x += 16; xm += 16; todo -= 16;
    }
    p >>= 1;
    if (!p) break;
    todo += p;
  }

  x += todo;
  x -= m - 1;

  if (n < m) y[n * 2] = x[n];

  {
    int32x8 a, b, c, d, e, f;

    n -= 8;
    a = int32x8_load(x + n);
    b = int32x8_load(x + n + m);
    c = _mm256_unpacklo_epi32(a,b); /* ab0ab1ab4ab5 */
    d = _mm256_unpackhi_epi32(a,b); /* ab2ab3ab6ab7 */
    e = _mm256_permute2x128_si256(c,d,0x20); /* ab0ab1ab2ab3 */
    f = _mm256_permute2x128_si256(c,d,0x31); /* ab4ab5ab4ab5 */
    int32x8_store(y + n * 2,e);
    int32x8_store(y + n * 2 + 8,f);

    while (n >= 0) {
      a = int32x8_load(x);
      b = int32x8_load(x + m);
      x += 8;
      c = _mm256_unpacklo_epi32(a,b); /* ab0ab1ab4ab5 */
      d = _mm256_unpackhi_epi32(a,b); /* ab2ab3ab6ab7 */
      e = _mm256_permute2x128_si256(c,d,0x20); /* ab0ab1ab2ab3 */
      f = _mm256_permute2x128_si256(c,d,0x31); /* ab4ab5ab4ab5 */
      int32x8_store(y,e);
      int32x8_store(y + 8,f);
      y += 16;
      n -= 8;
    }
  }
}

static void transpose(int32 *y,long long *pos,const int32 *x,long long n)
{
  long long i0,i1,i2,i3,i4,i5,i6,i7;

  pos[4] = i4 = (n + 1)/2;
  pos[2] = i2 = (i4 + 1)/2;
  pos[1] = i1 = (i2 + 1)/2;
  pos[3] = i3 = i2 + ((i4 - i2) + 1)/2;
  pos[6] = i6 = i4 + ((n - i4) + 1)/2;
  pos[5] = i5 = i4 + ((i6 - i4) + 1)/2;
  pos[7] = i7 = i6 + ((n - i6) + 1)/2;
  i0 = 0;
  /* pos[0] = 0; */
  /* pos[8] = n; */

  while (n >= 64) {
    int32x8 a0 = int32x8_load(x + 0); /* A01234567 */
    int32x8 a1 = int32x8_load(x + 8); /* B01234567 */
    int32x8 a2 = int32x8_load(x + 16); /* C01234567 */
    int32x8 a3 = int32x8_load(x + 24); /* D01234567 */
    int32x8 a4 = int32x8_load(x + 32); /* E01234567 */
    int32x8 a5 = int32x8_load(x + 40); /* F01234567 */
    int32x8 a6 = int32x8_load(x + 48); /* G01234567 */
    int32x8 a7 = int32x8_load(x + 56); /* H01234567 */
    x += 64;

    int32x8 b0 = _mm256_unpacklo_epi32(a0,a1); /* AB0AB1AB4AB5 */
    int32x8 b1 = _mm256_unpackhi_epi32(a0,a1); /* AB2AB3AB6AB7 */
    int32x8 b2 = _mm256_unpacklo_epi32(a2,a3); /* CD0CD1CD4CD5 */
    int32x8 b3 = _mm256_unpackhi_epi32(a2,a3); /* CD2CD3CD6CD7 */
    int32x8 b4 = _mm256_unpacklo_epi32(a4,a5); /* EF0EF1EF4EF5 */
    int32x8 b5 = _mm256_unpackhi_epi32(a4,a5); /* EF2EF3EF6EF7 */
    int32x8 b6 = _mm256_unpacklo_epi32(a6,a7); /* GH0GH1GH4GH5 */
    int32x8 b7 = _mm256_unpackhi_epi32(a6,a7); /* GH2GH3GH6GH7 */

    int32x8 c0 = _mm256_unpacklo_epi64(b0,b2); /* ABCD0ABCD4 */
    int32x8 c1 = _mm256_unpacklo_epi64(b1,b3); /* ABCD2ABCD6 */
    int32x8 c2 = _mm256_unpackhi_epi64(b0,b2); /* ABCD1ABCD3 */
    int32x8 c3 = _mm256_unpackhi_epi64(b1,b3); /* ABCD5ABCD7 */
    int32x8 c4 = _mm256_unpacklo_epi64(b4,b6); /* EFGH0EFGH4 */
    int32x8 c5 = _mm256_unpacklo_epi64(b5,b7); /* EFGH2EFGH6 */
    int32x8 c6 = _mm256_unpackhi_epi64(b4,b6); /* EFGH1EFGH3 */
    int32x8 c7 = _mm256_unpackhi_epi64(b5,b7); /* EFGH5EFGH7 */

    int32x8 d0 = _mm256_permute2x128_si256(c0,c4,0x20); /* ABCDEFGH0 */
    int32x8 d1 = _mm256_permute2x128_si256(c1,c5,0x20); /* ABCDEFGH2 */
    int32x8 d2 = _mm256_permute2x128_si256(c2,c6,0x20); /* ABCDEFGH1 */
    int32x8 d3 = _mm256_permute2x128_si256(c3,c7,0x20); /* ABCDEFGH5 */
    int32x8 d4 = _mm256_permute2x128_si256(c0,c4,0x31); /* ABCDEFGH4 */
    int32x8 d5 = _mm256_permute2x128_si256(c1,c5,0x31); /* ABCDEFGH6 */
    int32x8 d6 = _mm256_permute2x128_si256(c2,c6,0x31); /* ABCDEFGH3 */
    int32x8 d7 = _mm256_permute2x128_si256(c3,c7,0x31); /* ABCDEFGH7 */

    int32x8_store(y + i0,d0);
    int32x8_store(y + i2,d1);
    int32x8_store(y + i4,d2);
    int32x8_store(y + i6,d3);
    int32x8_store(y + i1,d4);
    int32x8_store(y + i3,d5);
    int32x8_store(y + i5,d6);
    int32x8_store(y + i7,d7);
    y += 8;
    n -= 64;
  }

  if (!n) return;

  while (n >= 8) {
    y[i0] = x[0];
    y[i4] = x[1];
    y[i2] = x[2];
    y[i6] = x[3];
    y[i1] = x[4];
    y[i5] = x[5];
    y[i3] = x[6];
    y[i7] = x[7];
    x += 8;
    ++y;
    n -= 8;
  }

  if (n == 0) return; else y[i0] = x[0];
  if (n == 1) return; else y[i4] = x[1];
  if (n == 2) return; else y[i2] = x[2];
  if (n == 3) return; else y[i6] = x[3];
  if (n == 4) return; else y[i1] = x[4];
  if (n == 5) return; else y[i5] = x[5];
  if (n == 6) return; else y[i3] = x[6];
}

void int32_sort(int32 *x,long long n)
{
  long long top,p,q,r,i,j,pos[8];
  int32 * yp = 0, * y = 0;
  //int32 y[n];
  int32 a0;
  int32x8 a01234567, b01234567, c01234567, d01234567;

  if (n <= 2) {
    if (n < 2) return;
    a0 = x[0];
    a0 = int32_minmax(a0,x + 1);
    x[0] = a0;
    return;
    /* XXX: support more small sizes */
  }

  yp = (int32*)calloc(n, sizeof(int32));
  y = yp;

  top = 1;
  while (top < n - top) top += top;

  p = top;
  if (top <= 16) {
    if (top == 16) goto top16;
    if (top == 8) goto top8;
    goto top124;
  }

  for (;p >= 32;p >>= 1) {
    i = 0;
    while (i + 2 * p <= n) {
      for (j = i;j < i + p;j += 32) {
        a01234567 = int32x8_load(x + j);
        b01234567 = int32x8_load(x + j + 8);
        c01234567 = int32x8_load(x + j + 16);
        d01234567 = int32x8_load(x + j + 24);
        a01234567 = int32x8_minmax(a01234567,x + j + p);
        b01234567 = int32x8_minmax(b01234567,x + j + p + 8);
        c01234567 = int32x8_minmax(c01234567,x + j + p + 16);
        d01234567 = int32x8_minmax(d01234567,x + j + p + 24);
        int32x8_store(x + j,a01234567);
        int32x8_store(x + j + 8,b01234567);
        int32x8_store(x + j + 16,c01234567);
        int32x8_store(x + j + 24,d01234567);
      }
      i += 2 * p;
    }
    for (j = i;j < n - p;++j)
      x[j] = int32_minmax(x[j],x + j + p);

    i = 0;
    j = 0;
    for (q = top;q > p;q >>= 1) {
      if (j != i) for (;;) {
        if (j == n - q) goto done32;
        a0 = x[j + p];
        for (r = q;r > p;r >>= 1)
          a0 = int32_minmax(a0,x + j + r);
        x[j + p] = a0;
        ++j;
        if (j == i + p) {
          i += 2 * p;
          break;
        }
      }

      while (i + p <= n - q) {
        for (j = i;j < i + p;j += 32) {
          a01234567 = int32x8_load(x + j + p);
          b01234567 = int32x8_load(x + j + p + 8);
          c01234567 = int32x8_load(x + j + p + 16);
          d01234567 = int32x8_load(x + j + p + 24);
          for (r = q;r > p;r >>= 1) {
            a01234567 = int32x8_minmax(a01234567,x + j + r);
            b01234567 = int32x8_minmax(b01234567,x + j + r + 8);
            c01234567 = int32x8_minmax(c01234567,x + j + r + 16);
            d01234567 = int32x8_minmax(d01234567,x + j + r + 24);
          }
          int32x8_store(x + j + p,a01234567);
          int32x8_store(x + j + p + 8,b01234567);
          int32x8_store(x + j + p + 16,c01234567);
          int32x8_store(x + j + p + 24,d01234567);
        }
        i += 2 * p;
      }

      j = i;
      while (j < n - q) {
        a0 = x[j + p];
        for (r = q;r > p;r >>= 1)
          a0 = int32_minmax(a0,x + j + r);
        x[j + p] = a0;
        ++j;
      }

      done32: ;
    }
  }

top16:
  i = 0;
  while (i + 2 * 16 <= n) {
    a01234567 = int32x8_load(x + i);
    b01234567 = int32x8_load(x + i + 8);
    a01234567 = int32x8_minmax(a01234567,x + i + 16);
    b01234567 = int32x8_minmax(b01234567,x + i + 16 + 8);
    int32x8_store(x + i,a01234567);
    int32x8_store(x + i + 8,b01234567);
    i += 2 * 16;
  }

  for (j = i;j < n - 16;++j)
    x[j] = int32_minmax(x[j],x + j + 16);

  i = 0;
  j = 0;
  for (q = top;q > 16;q >>= 1) {
    if (j != i) for (;;) {
      if (j == n - q) goto done16;
      a0 = x[j + 16];
      for (r = q;r > 16;r >>= 1)
        a0 = int32_minmax(a0,x + j + r);
      x[j + 16] = a0;
      ++j;
      if (j == i + 16) {
        i += 2 * 16;
        break;
      }
    }
    while (i + 3 * 16 <= n - q) {
      a01234567 = int32x8_load(x + i + 16);
      b01234567 = int32x8_load(x + i + 16 + 8);
      c01234567 = int32x8_load(x + i + 16 + 2 * 16);
      d01234567 = int32x8_load(x + i + 16 + 2 * 16 + 8);
      for (r = q;r > 16;r >>= 1) {
        a01234567 = int32x8_minmax(a01234567,x + i + r);
        b01234567 = int32x8_minmax(b01234567,x + i + r + 8);
        c01234567 = int32x8_minmax(c01234567,x + i + r + 2 * 16);
        d01234567 = int32x8_minmax(d01234567,x + i + r + 2 * 16 + 8);
      }
      int32x8_store(x + i + 16,a01234567);
      int32x8_store(x + i + 16 + 8,b01234567);
      int32x8_store(x + i + 16 + 2 * 16,c01234567);
      int32x8_store(x + i + 16 + 2 * 16 + 8,d01234567);
      i += 4 * 16;
    }
    while (i + 16 <= n - q) {
      a01234567 = int32x8_load(x + i + 16);
      b01234567 = int32x8_load(x + i + 16 + 8);
      for (r = q;r > 16;r >>= 1) {
        a01234567 = int32x8_minmax(a01234567,x + i + r);
        b01234567 = int32x8_minmax(b01234567,x + i + r + 8);
      }
      int32x8_store(x + i + 16,a01234567);
      int32x8_store(x + i + 16 + 8,b01234567);
      i += 2 * 16;
    }

    j = i;
    while (j < n - q) {
      a0 = x[j + 16];
      for (r = q;r > 16;r >>= 1)
        a0 = int32_minmax(a0,x + j + r);
      x[j + 16] = a0;
      ++j;
    }

    done16: ;
  }

top8:
  i = 0;
  while (i + 2 * 8 <= n) {
    a01234567 = int32x8_load(x + i);
    a01234567 = int32x8_minmax(a01234567,x + i + 8);
    int32x8_store(x + i,a01234567);
    i += 2 * 8;
  }
  for (j = i;j < n - 8;++j)
    x[j] = int32_minmax(x[j],x + j + 8);

  i = 0;
  j = 0;
  for (q = top;q > 8;q >>= 1) {
    if (j != i) for (;;) {
      if (j == n - q) goto done8;
      a0 = x[j + 8];
      for (r = q;r > 8;r >>= 1)
        a0 = int32_minmax(a0,x + j + r);
      x[j + 8] = a0;
      ++j;
      if (j == i + 8) {
        i += 2 * 8;
        break;
      }
    }
    while (i + 7 * 8 <= n - q) {
      a01234567 = int32x8_load(x + i + 8);
      b01234567 = int32x8_load(x + i + 3 * 8);
      c01234567 = int32x8_load(x + i + 5 * 8);
      d01234567 = int32x8_load(x + i + 7 * 8);
      for (r = q;r > 8;r >>= 1) {
        a01234567 = int32x8_minmax(a01234567,x + i + r);
        b01234567 = int32x8_minmax(b01234567,x + i + r + 2 * 8);
        c01234567 = int32x8_minmax(c01234567,x + i + r + 4 * 8);
        d01234567 = int32x8_minmax(d01234567,x + i + r + 6 * 8);
      }
      int32x8_store(x + i + 8,a01234567);
      int32x8_store(x + i + 3 * 8,b01234567);
      int32x8_store(x + i + 5 * 8,c01234567);
      int32x8_store(x + i + 7 * 8,d01234567);
      i += 8 * 8;
    }
    while (i + 8 <= n - q) {
      a01234567 = int32x8_load(x + i + 8);
      for (r = q;r > 8;r >>= 1)
        a01234567 = int32x8_minmax(a01234567,x + i + r);
      int32x8_store(x + i + 8,a01234567);
      i += 2 * 8;
    }

    j = i;
    while (j < n - q) {
      a0 = x[j + 8];
      for (r = q;r > 8;r >>= 1)
        a0 = int32_minmax(a0,x + j + r);
      x[j + 8] = a0;
      ++j;
    }

    done8: ;
  }

top124:

  transpose(y,pos,x,n);

  merge_to(x,y,pos[2],pos[1]);
  merge_to(x + pos[2],y + pos[2],pos[4] - pos[2],pos[3] - pos[2]);
  merge_to(y,x,pos[4],pos[2]);

  merge_to(x + pos[4],y + pos[4],pos[6] - pos[4],pos[5] - pos[4]);
  merge_to(x + pos[6],y + pos[6],n - pos[6],pos[7] - pos[6]);
  merge_to(y + pos[4],x + pos[4],n - pos[4],pos[6] - pos[4]);
 
  merge_to(x,y,n,pos[4]);
}
