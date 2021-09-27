#pragma once

#include <cstdint>
#include <iostream>
#include <optional>
#include <limits>

template<typename T>
std::optional<T> SafeAdd(const T l, const T r) {
    if ((l >= 0 && r <= 0) || (l <= 0 && r >= 0)) return l + r;

    const T MAX = std::numeric_limits<T>::max();
    const T MIN = std::numeric_limits<T>::min();

    //if (MAX < (l + r) ||
    //    MIN >(l + r))
    //    return std::nullopt;
    if (l < 0) {
        if (l < MIN - r) {
            return std::nullopt;
        }
    }
    else {
        if (l > MAX - r) {
            return std::nullopt;
        }
    }
    return l + r;
}