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

    if (l < 0) {
        if (l > (MAX - r)) {
            return std::nullopt;
        }
    }
    else {
        if (l > (MIN + r))
            return std::nullopt;
    }
    

    //bool b1 = MAX < (l + r);
    //bool b2 = MIN > (l + r);

    //bool b1 = l > (MAX - r);
    //bool b2 = MIN > (l + r);

    //if (b1 || b2) return std::nullopt;

    return l + r;
}