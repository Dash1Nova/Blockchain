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

std::string hash(const std::vector<uint8_t>& data) {

}





int main() {
    while (true) {
        int choice;
        
        std::cout << "Pasirinkite, ką norėtumėte padaryti:\n";
        std::cout << "1 - suhashuoti tekstą\n";
        std::cout << "2 - suhashuoti failą\n";
        std::cout << "3 - baigti darbą\n";
        std::cin >> choice;
        
        if (choice == 1) {
            std::string input;
            std::cout << "Įveskite tekstą: ";
            std::cin >> input;

            std::vector<uint8_t> data = convertTextTB(input);
            std::string result = hash(data);
            std::cout << "Hash: " << result << "\n";
        } else if (choice == 2) {
            std::string filename;
            std::cout << "Įveskite failo pavadinimą arba kelią iki jo: ";
            std::getline(std::cin, filename);

            std::vector<uint8_t> data = readFile(filename);
            std::string result = hash(data);
            std::cout << "Hash: " << result << "\n";
        } else if (choice == 3) break;
    }

    return 0;
}