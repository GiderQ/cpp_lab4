#pragma once

#include <chrono>
#include <utility>

template <typename F, typename... Args>
auto Timer(F&& func, int repeat = 1, Args&&... args)
{
    auto start = std::chrono::high_resolution_clock::now();

    while (repeat-- > 0) {
        func(std::forward<Args>(args)...);
    }

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start);
}
