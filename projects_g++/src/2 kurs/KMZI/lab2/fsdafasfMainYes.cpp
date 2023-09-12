#include <iostream>
#include <cstring>
#include <cmath>

const int BLOCK_SIZE = 7;
const int quntitiesBlocks = 64;

void scramble(char* data, int size, char* key, int key_size) {
    int index = 0;
    for (int i = 0; i < size; i++) {
        data[i] ^= key[index];
        index = (index + 1) % key_size;
    }
}

void cfb_encrypt(char* data, int size, char* key, int key_size, char* iv, int iv_size) {
    // Генерация начального вектора
    char prev_block[BLOCK_SIZE];
    memcpy(prev_block, iv, iv_size);

    // Шифрование каждого блока
    char* block = new char[BLOCK_SIZE];
    for (int i = 0; i < size; i += BLOCK_SIZE) {
        memcpy(block, &data[i], BLOCK_SIZE);

        for (int j = 0; j < BLOCK_SIZE; j++) {
            block[j] ^= prev_block[j];
        }
        scramble(block, BLOCK_SIZE, key, key_size);
        memcpy(&data[i], block, BLOCK_SIZE);
        memcpy(prev_block, block, BLOCK_SIZE);
    }
    delete[] block;
}

void cfb_decrypt(char* data, int size, char* key, int key_size, char* iv, int iv_size) {
    // Генерация начального вектора
    char prev_block[BLOCK_SIZE];
    memcpy(prev_block, iv, iv_size);

    // Расшифрование каждого блока
    char* block = new char[BLOCK_SIZE];
    for (int i = 0; i < size; i += BLOCK_SIZE) {
        memcpy(block, &data[i], BLOCK_SIZE);
        scramble(prev_block, BLOCK_SIZE, key, key_size);

        for (int j = 0; j < BLOCK_SIZE; j++) {
            block[j] ^= prev_block[j];
        }
        memcpy(&data[i], block, BLOCK_SIZE);
        memcpy(prev_block, block, BLOCK_SIZE);
    }
    delete[] block;
}

class str {
public: 
    char string1[BLOCK_SIZE];
    char key[BLOCK_SIZE];
    char iv[BLOCK_SIZE];
    str() {
    memset(string1, 0, BLOCK_SIZE);
    memset(key, 0, BLOCK_SIZE);
    memset(iv, 0, BLOCK_SIZE);
    }

    void set_string(char* str) {
        memcpy(string1, str, BLOCK_SIZE);
    }

    void set_key(char* key_data) {
        memcpy(key, key_data, BLOCK_SIZE);
    }

    void set_iv(char* iv_data) {
        memcpy(iv, iv_data, BLOCK_SIZE);
    }

    void encrypt() {
        cfb_encrypt(string1, BLOCK_SIZE, key, BLOCK_SIZE, iv, BLOCK_SIZE);
    }

    void decrypt() {
        cfb_decrypt(string1, BLOCK_SIZE, key, BLOCK_SIZE, iv, BLOCK_SIZE);
    }

    void print() {
        std::cout << string1 << std::endl;
    }
};

int main() {
    str message;
    char string[] = "something was wrong";
    char key[] = "Secret!";
    char iv[] = "Vector!";

    message.set_string(string);
    message.set_key(key);
    message.set_iv(iv);

    message.print();
    message.encrypt();
    message.print();
    message.decrypt();
    message.print();

return 0;
}
