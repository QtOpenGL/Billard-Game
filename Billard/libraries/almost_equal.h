#ifndef Simulateur_Particule_almost_equal_h
#define Simulateur_Particule_almost_equal_h

#include <cmath> //max
#include <limits>//numeric_limits

template<class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
inline almost_equal(T x, T y, unsigned long unit_last_place)
{
    // see : http://en.cppreference.com/w/cpp/types/numeric_limits/epsilon
    // the machine epsilon has to be scaled to the magnitude of the values used
    // and multiplied by the desired precision in ULPs (units in the last place)
    return std::abs(x-y) < std::numeric_limits<T>::epsilon() * std::abs(x+y) * unit_last_place
    // unless the result is subnormal
    || std::abs(x-y) < std::numeric_limits<T>::min();
}

#endif
