#include <iostream>
#include <cstring>

const int BLOCK_SIZE = 7; // Размер блока
const int KEY_SIZE = 7; // Размер ключа
const int SHIFT_AMOUNT = 3; // Количество бит, на которое происходит сдвиг

// Скрэмблер, осуществляющий сдвиг вправо
class Scrambler {
private:
    uint8_t state;
public:
    Scrambler(uint8_t initialState) : state(initialState) {}
    uint8_t scramble() {
        uint8_t output = state >> SHIFT_AMOUNT;
        state = ((output << (BLOCK_SIZE - SHIFT_AMOUNT)) | (state >> SHIFT_AMOUNT));
        return output;
    }
};

// Шифрование/дешифрование в режиме CFB
void cfb(uint8_t* key, uint8_t* input, uint8_t* output, int length, uint8_t iv) {
    Scrambler scrambler(iv);
    for (int i = 0; i < length; i += BLOCK_SIZE) {
        uint8_t feedback[BLOCK_SIZE];
        uint8_t keystream[BLOCK_SIZE];
        memcpy(feedback, &iv, BLOCK_SIZE);

        // Генерация потока ключей
        for (int j = 0; j < BLOCK_SIZE; j++) {
            keystream[j] = scrambler.scramble() ^ key[j];
        }
        // XOR-шифрование
        for (int j = 0; j < BLOCK_SIZE; j++) {
            output[i+j] = input[i+j] ^ keystream[j];
        }
        memcpy(feedback, &output[i], BLOCK_SIZE);
        iv = feedback[0];
    }
}

int main() {
    uint8_t key[KEY_SIZE] = { 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde };
    std::string plaintext = "In scholarly societies";
    std::cout << "Initial data: " << plaintext << std::endl; 
    int length = plaintext.length();

    int modNum = length % 7;
    if (modNum != 0)
    {
        for (size_t i = 0; i < (BLOCK_SIZE - modNum); i++)
        {
            plaintext.push_back(' ');
        }        
    }    

    uint8_t* input = reinterpret_cast<uint8_t*>(const_cast<char*>(plaintext.c_str()));
    uint8_t* output = new uint8_t[length];
    uint8_t iv = 0x00;

    cfb(key, input, output, length, iv);
    std::cout << "Encrypted ciphertext: ";
    for (int i = 0; i < length; i++) {
        std::cout << std::hex << static_cast<int>(output[i]);
    }
    std::cout << std::endl;

    uint8_t* decrypted = new uint8_t[length];
    iv = 0x00;
    cfb(key, output, decrypted, length, iv);
    std::string plaintext2(reinterpret_cast<char*>(decrypted), length + (BLOCK_SIZE - modNum));
    std::cout << "Decrypted plaintext: " << plaintext2 << std::endl;

    delete[] output;
    delete[] decrypted;
    return 0;
}

