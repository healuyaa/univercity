#include <iostream>
#include <cstring> // добавляем для использования memcpy

const int BLOCK_SIZE = 7;

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
    for (int i = 0; i < size; i += BLOCK_SIZE) {
        char block[BLOCK_SIZE];
        memcpy(block, &data[i], BLOCK_SIZE);

        // XOR с предыдущим блоком
        for (int j = 0; j < BLOCK_SIZE; j++) {
            block[j] ^= prev_block[j];
        }

        // Шифрование блока
        scramble(block, BLOCK_SIZE, key, key_size);

        // Копирование зашифрованного блока в выходной массив
        memcpy(&data[i], block, BLOCK_SIZE);

        // Сохранение текущего блока для использования при следующем шифровании
        memcpy(prev_block, block, BLOCK_SIZE);
    }
}

void cfb_decrypt(char* data, int size, char* key, int key_size, char* iv, int iv_size) {
    // Генерация начального вектора
    char prev_block[BLOCK_SIZE];
    memcpy(prev_block, iv, iv_size);

    // Расшифрование каждого блока
    for (int i = 0; i < size; i += BLOCK_SIZE) {
        char block[BLOCK_SIZE];
        memcpy(block, &data[i], BLOCK_SIZE);

        // Шифрование предыдущего блока
        scramble(prev_block, BLOCK_SIZE, key, key_size);

        // XOR с предыдущим блоком
        for (int j = 0; j < BLOCK_SIZE; j++) {
            block[j] ^= prev_block[j];
        }

        // Копирование расшифрованного блока в выходной массив
        memcpy(&data[i], block, BLOCK_SIZE);

        // Сохранение текущего блока для использования при следующем расшифровании
        memcpy(prev_block, &data[i], BLOCK_SIZE);
    }
}

int main() {
    
    char data[] = "Hello";
    int size = sizeof(data) - 1; // не включаем символ '\0'
    char key[] = "secret";
    int key_size = sizeof(key) - 1; // не включаем символ '\0'
    char iv[] = "initvec";
    int iv_size = sizeof(iv) - 1; // не включаем символ '\0'

    std::cout << "Original data: " << data << std::endl;

    // Шифрование данных
    cfb_encrypt(data, size, key, key_size, iv, iv_size);
    std::cout << "Encrypted data: ";
    for (int i = 0; i < size; i++) {
        std::cout << (int)data[i] << "";
    }
    std::cout << std::endl;
    // Расшифрование данных
    cfb_decrypt(data, size, key, key_size, iv, iv_size);
    for (size_t i = 0; i < sizeof(data); i++)
    {
        std::cout << "Decrypted data: " << data[i];   
    }

    return 0;
}

