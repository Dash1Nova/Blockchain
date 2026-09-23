#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <vector>
#include "time_benchmark.h"


struct Current {
    std::array<uint64_t, 4> value = {
        0x0B0B0B0B0B0B0B0BULL,
        0x0C0C0C0C0C0C0C0CULL,
        0x0D0D0D0D0D0D0D0DULL,
        0x0E0E0E0E0E0E0E0EULL
    };
};

Current hashBucket(const std::vector<uint8_t>& bucket, Current& startBucket) {
    Current currentBucket = startBucket;
    const uint64_t newWeight = 7;
    const uint64_t oldWeight = 4;
    const uint64_t totalWeight = newWeight + oldWeight;

    for (size_t i = 0; i < bucket.size(); i++) {
        int part = (i + bucket[i]) % 4;
        uint64_t newValue = static_cast<uint64_t>(bucket[i]);
        uint64_t oldValue = currentBucket.value[part];
        currentBucket.value[part] = (newValue * newWeight + oldValue * oldWeight) / totalWeight;
    }
    return currentBucket;
}

std::vector<uint8_t> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    std::vector<uint8_t> data;

    if(!file) {
        throw std::runtime_error("Nepavyko atidaryti failo.");
    }

    char c;
    while (file.get(c)) {
        data.push_back(static_cast<uint8_t>(c));
    }
    return data;
}

std::vector<uint8_t> convertTextTB(const std::string& text) {
    std::vector<uint8_t> converted(text.begin(), text.end());
    return converted;
}

std::string hash(const std::vector<uint8_t>& data) {
    Current currentMain;
    size_t bucketSize = 8;

    for (size_t i = 0; i < data.size(); i += bucketSize) {
        size_t end = std::min(i + bucketSize, data.size());
        std::vector<uint8_t> bucket( data.begin() + i, data.begin() + end);
        currentMain = hashBucket(bucket, currentMain);
    }

    std::string result;
    static const char hexDigits[] = "0123456789abcdef";
    result.reserve(64);

    for (int i = 3; i >= 0; i--) {
        for (int shift = 60; shift >= 0; shift -= 4) {
            result += hexDigits[(currentMain.value[i] >> shift) & 0xF];
        }
    }
    return result;
}


int main(int argc, char* argv[]) {
    if (argc == 2) {
        std::string filename = argv[1];
        std::cout << "Rezimas: failo turinio maisa (argumentas)\n";
        try {
            std::vector<uint8_t> data = readFile(filename);
            std::string result = hash(data);
            std::cout << "Hash: " << result << "\n";
        } catch (const std::exception& e) {
            std::cerr << "Klaida: " << e.what() << "\n";
            return 1;
        }
        return 0;
    }

    while (true) {
        int choice;
        
        std::cout << "Pasirinkite, ka noretumete padaryti:\n";
        std::cout << "1 - suhashuoti teksta\n";
        std::cout << "2 - suhashuoti faila\n";
        std::cout << "3 - baigti darba\n";
        std::cin >> choice;
        std::cin.ignore();
        
        if (choice == 1) {
            std::string input;
            std::cout << "Iveskite teksta: ";
            std::getline(std::cin, input);

            std::vector<uint8_t> data = convertTextTB(input);
            std::string result = hash(data);
            std::cout << "Hash: " << result << "\n";
        } else if (choice == 2) {
            std::string filename;
            std::cout << "Iveskite failo pavadinima arba kelia iki jo: ";
            std::getline(std::cin, filename);

            try {
            std::vector<uint8_t> data = readFile(filename);
            std::string result = hash(data);
            std::cout << "Hash: " << result << "\n";
            } catch (const std::exception& e) {
                std::cerr << "Klaida: " << e.what() << "\n";
            }
        } else if (choice == 4) timeBenchmark(hash);
        else if (choice == 3) break;
    }

    return 0;
}