#ifndef MPL_H
#define MPL_H

#include <math.h>

namespace mpl
{

template <int N, typename T = const float>
struct sumsq
{
    static inline T value(T * v) { return  v[N] * v[N] + sumsq<N-1, T>::value(v) ; }
};

template <typename T>
struct sumsq<0, T>
{
    static inline T value(T * v) { return v[0] * v[0]; }
};

template <int N, typename T = const float>
struct vlen
{
    static inline T value(T * v) { return sqrt(sumsq<N-1>::value(v)); }
};

} // ns mpl

#endif // MPL_H
