#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <vector>

struct Pair {
    uint64_t inputByte;
    uint64_t convertedByte;
};

struct Current {
    std::array<uint64_t, 4> value = {0, 0, 0, 0};
};

Current hashBucket(const std::vector<uint8_t>& bucket) {
    Current currentBucket;
    for (size_t i = 0; i < bucket.size(); i++) {
        Pair pair;
        pair.inputByte = bucket[i];
        pair.convertedByte = pair.inputByte;

        int part = i % 4;
        currentBucket.value[part] ^= pair.convertedByte;

        if (i % 2 == 0) {
            currentBucket.value[part] = (currentBucket.value[part] << 13) | (currentBucket.value[part] << (64 - 13));
        } else {
            currentBucket.value[part] = (currentBucket.value[part] << 17) | (currentBucket.value[part] << (64 - 17));
        }
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



std::string hash(const std::string& input) {
    unsigned currentValue = 0; // su tipu dar pasvarstyti
    
    try {
        std::fstream file(input);

        if (!file.is_open()) {
            throw std::runtime_error("Nepavyko atidaryti failo");
        }
        file.close();
    }
    catch () {

    }
    
    for (size_t i = 0; i < input.size(); i++) {
        if (i % 2 == 0) {
           currentValue = currentValue << 5;
        }
        else {
            currentValue = currentValue >> 3;
        }
    }

    return hash;
}

int main() {
    while (true) {
        std::string input;

        std::cout << "Įveskite tekstą arba kelią iki failo: ";
        std::cin >> input;
    
        result = hash(input);

        std::cout << "Hash: " << result << "\n";
    }

    return 0;
}