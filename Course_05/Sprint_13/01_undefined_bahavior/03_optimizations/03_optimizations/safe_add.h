#pragma once

#include <cstdint>
#include <iostream>
#include <optional>
#include <limits>

template<typename T>
std::optional<T> SafeAdd(const T l, const T r) {
    if ((l >= 0 && r <= 0) || (l <= 0 && r >= 0)) return l + r;

    constexpr T MAX = std::numeric_limits<T>::max();
    constexpr T MIN = std::numeric_limits<T>::min();

<<<<<<< HEAD
    //if (MAX < (l + r) ||
    //    MIN >(l + r))
    //    return std::nullopt;
    if (l < 0) {
        if (l < MIN - r) {
=======
    if (l < 0) {
        if (l > (MAX - r)) {
>>>>>>> c3178eff50a5752f07dc226c92055f9d175821f6
            return std::nullopt;
        }
    }
    else {
<<<<<<< HEAD
        if (l > MAX - r) {
            return std::nullopt;
        }
    }
=======
        if (l > (MIN + r))
            return std::nullopt;
    }
    

    //bool b1 = MAX < (l + r);
    //bool b2 = MIN > (l + r);

    //bool b1 = l > (MAX - r);
    //bool b2 = MIN > (l + r);

    //if (b1 || b2) return std::nullopt;

>>>>>>> c3178eff50a5752f07dc226c92055f9d175821f6
    return l + r;
}