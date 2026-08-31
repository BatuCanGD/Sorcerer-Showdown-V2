#pragma once
#include <iostream>
#include <limits>
#include <type_traits>

template <typename T>
[[nodiscard]] T get_input(){
    T t{};
    if constexpr (std::is_same_v<T, std::string>) {
        getline(std::cin, t);
    }else{
        while(!(std::cin >> t)){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return t;
}