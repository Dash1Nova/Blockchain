#include "time_benchmark.h"
#include <iostream>
#include <chrono>
#include <random>

void timeBenchmark(std::string (*hashFunc)(const std::vector<uint8_t>&)) {
    std::vector<size_t> sizes = {1000, 10000, 100000, 1000000};
    std::mt19937 rng(42);

    std::cout << "Dydis (baitais) | Laikas (ms)\n";

    for (size_t size : sizes) {
        std::vector<uint8_t> data(size);
        for (size_t i = 0; i < size; i++) {
            data[i] = static_cast<uint8_t>(rng() % 256);
        }

        auto start = std::chrono::high_resolution_clock::now();
        std::string result = hashFunc(data);
        auto end = std::chrono::high_resolution_clock::now();

        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << size << " | " << ms << " ms\n";
    }
}