#ifndef RANDOM_H
#define RANDOM_H

#include <algorithm>
#include <vector>

namespace Random {

/**
 * Return a value between min included and max excluded
 */
template<class T>
T randomRange(T min, T max) {
    int value = std::rand();
    return min + T(value) % (max - min);
}

/**
 * Return a value between 0 included and max excluded
 */
template<class T>
T randomRange(T max) {
    return randomRange(T(0), max);
}

inline bool randomBool() {
    return randomRange(2) == 1;
}

template<class T>
T& randomChoice(std::vector<T>& items) {
    auto idx = randomRange(items.size());
    return items[idx];
}

template<class T>
const T& randomChoice(const std::vector<T>& items) {
    auto idx = randomRange(items.size());
    return items.at(idx);
}

}; // namespace Random

#endif // RANDOM_H
