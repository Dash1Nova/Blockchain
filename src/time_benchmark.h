#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <vector>
#include <cstdint>
#include <string>

void timeBenchmark(std::string (*hashFunc)(const std::vector<uint8_t>&));

#endif