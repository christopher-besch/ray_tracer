#pragma once

#include <cstdlib>

// in [0, 1]
inline double random_double()
{
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max)
{
    return min + (max - min) * random_double();
}
