#include <iostream>
#include <string>
#include <bitset>
#include <fstream>

#define blockSize 56

std::string readFileText() {
    std::ifstream file("C:/project/C++/src/KMZI/lab2/test.txt");
    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла";
        return "";
    }

    std::string output;
    getline(file, output);
    std::cout << "Исходный текст: " << output << std::endl;

    file.close();
    return output;
}

std::bitset<blockSize> readFileKey() {
    std::ifstream file("C:/project/C++/src/KMZI/lab2/key.txt");
    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла";
        return std::bitset<blockSize>();
    }

    std::bitset<blockSize> key;
    file >> key;

    file.close();
    return key;
}


// Функция скремблирования
std::bitset<blockSize> scramble(std::bitset<blockSize> input) {
    std::bitset<blockSize> output;
    std::bitset<blockSize> mask("0000010101010101010101010101010101010101010101010101010101010101"); // маска для скремблирования

    for (int i = 0; i < blockSize; i++) {
        int pos = (i + 1) % blockSize;
        output[pos] = input[i] ^ input[pos];
    }
    for (int i = 0; i < blockSize; i++) {
        output[i] = output[i] ^ mask[i];
    }

    return output;
}

// Функция дескремблирования
std::bitset<blockSize> descramble(std::bitset<blockSize> input) {
    std::bitset<blockSize> output;
    std::bitset<blockSize> mask("0000010101010101010101010101010101010101010101010101010101010101"); // маска для дескремблирования

    for (int i = 0; i < blockSize; i++) {
        output[i] = input[i] ^ mask[i];
    }
    for (int i = 0; i < blockSize; i++) {
        int pos = (i + 1) % blockSize;
        output[pos] = output[i] ^ input[pos];
    }

    return output;
}

std::string CFB(std::string input, std::bitset<blockSize> key, std::bitset<blockSize> iv) {
    std::string output;
    std::bitset<blockSize> ivTemp = iv;
    for (size_t i = 0; i < input.size(); i += blockSize / 8) {
        std::bitset<blockSize> encryptedBlock = scramble(ivTemp);
        std::bitset<blockSize> block(input.substr(i, blockSize / 8));
        std::bitset<blockSize> ciphered = encryptedBlock ^ block;
        output += ciphered.to_string();
        ivTemp = ciphered;
    }
    return output;
}

std::string CFBD(std::string input, std::bitset<blockSize> key, std::bitset<blockSize> iv) {
    std::string output;
    std::bitset<blockSize> ivTemp = iv;
    for (size_t i = 0; i < input.size(); i += blockSize / 8) {
        std::bitset<blockSize> encryptedBlock = descramble(ivTemp);
        std::bitset<blockSize> block(input.substr(i, 7));
        std::bitset<blockSize> ciphered = encryptedBlock ^ block;
        output += block.to_string();
        ivTemp = ciphered;
    }
    return output;
}

std::string bitStringToCharString(const std::string& bitString) {
    std::string paddedBitString = bitString;
    if (bitString.length() % 8 != 0) {
        paddedBitString.append(8 - bitString.length() % 8, '0');
    }

    std::string charString;
    for (size_t i = 0; i < paddedBitString.length(); i += 8) {
        std::bitset<8> bits(paddedBitString.substr(i, 8));
        charString += static_cast<char>(bits.to_ulong());
    }
    return charString;
}


int main() {
    std::string trueText = readFileText();
    std::bitset<blockSize> key = readFileKey();
    std::bitset<blockSize> iv("1010101010101010");

    std::string test = CFB(trueText, key, iv);
    std::cout << test << std::endl;
    std::cout << CFBD(test, key, iv) << std::endl;
        
}