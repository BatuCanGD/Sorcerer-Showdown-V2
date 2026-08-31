#pragma once
#include <random>
#include <algorithm>
#include <type_traits>
#include <concepts>

template<typename T>
requires (std::integral<T> && !std::same_as<T, bool>) || std::floating_point<T>
[[nodiscard]] T get_random(T mn, T mx) {
    if (mn == mx) return mn;
    T min = std::min(mn, mx), max = std::max(mn, mx);

    thread_local std::mt19937 gen([] {
        std::random_device rd;
        std::seed_seq seed{rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()};
        return std::mt19937(seed);
    }());

    if constexpr (std::is_floating_point_v<T>) {
        std::uniform_real_distribution<T> dist(min, max);
        return dist(gen);
    } else {
        using distro = std::conditional_t<std::is_signed_v<T>, long long, unsigned long long>;
        std::uniform_int_distribution<distro> dist(static_cast<distro>(min), static_cast<distro>(max));
        return static_cast<T>(dist(gen));
    }
}