#pragma once

#include <cstdint>
#include <iostream>
#include <optional>
#include <limits>

template<typename T>
std::optional<T> SafeAdd(const T l, const T r) {
    if ((l >= 0 && r <= 0) || (l <= 0 && r >= 0)) return l + r;
    const T MAX = std::numeric_limits<T>::max();
    if (!(l > MAX - r) && !(r > MAX - l)) return l + r;
    const T MIN = std::numeric_limits<T>::min();
    if (!(l < MIN - r) && !(r < MIN - l)) return l + r;
    return std::nullopt;
}