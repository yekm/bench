#ifndef THRUST_SORT_HPP
#define THRUST_SORT_HPP

#include <vector>

// nvcc can't compile my sources, so...

template<typename T>
void thrust_sort_ex(std::vector<T> & data);

#endif // THRUST_SORT_HPP
