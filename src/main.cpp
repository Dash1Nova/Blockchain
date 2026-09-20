#include <iostream>
#include <string>
#include <fstream>

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