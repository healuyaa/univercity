#include <iostream>
#include <cstring>
#include <math.h>

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
    for (int i = 0; i < size; i += BLOCK_SIZE) {
        char block[BLOCK_SIZE];
        memcpy(block, &data[i], BLOCK_SIZE);

        for (int j = 0; j < BLOCK_SIZE; j++) {
            block[j] ^= prev_block[j];
        }
        scramble(block, BLOCK_SIZE, key, key_size);
        memcpy(&data[i], block, BLOCK_SIZE);
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
        scramble(prev_block, BLOCK_SIZE, key, key_size);

        for (int j = 0; j < BLOCK_SIZE; j++) {
            block[j] ^= prev_block[j];
        }
        memcpy(&data[i], block, BLOCK_SIZE);
        memcpy(prev_block, &data[i], BLOCK_SIZE);
        scramble(block, BLOCK_SIZE, key, key_size);
    }
}

class str {
    public: 
        char string1[BLOCK_SIZE] = {' ',' ',' ',' ',' ',' ',' '};
};

size_t reduceStr(char data[], int size, str* arr) {
    int comp = size % BLOCK_SIZE;
    int blockNum = size / BLOCK_SIZE + (comp > 0 ? 1 : 0);
    
    size_t k = 0;
    for (size_t i = 0; i < size; i += BLOCK_SIZE) {
        size_t j = 0;
        for (j; j < BLOCK_SIZE && i + j < size; j++) {
            arr[k].string1[j] = data[i + j];
        }
        for (j; j < BLOCK_SIZE; j++) {
            arr[k].string1[j] = ' ';
        }
        k++;
    }

    return blockNum;
}

int main() {
    char data[] = "Something was wrong";
    int size = strlen(data);
    std::cout << "Start data: " << data << std::endl;
    char key[] = "secret";
    int key_size = strlen(key);
    char iv[BLOCK_SIZE] = "initia";

    str arr[quntitiesBlocks];
    size_t blocks = reduceStr(data, size, arr);

    std::cout << "Encrypted: ";
    for (size_t i = 0; i < blocks; i++)
    {
        cfb_encrypt(arr[i].string1, BLOCK_SIZE, key, key_size, iv, BLOCK_SIZE);
        std::cout << arr[i].string1 << ' ';
    } std::cout << '\n';

    std::cout << "Decrypted: ";
    for (size_t i = 0; i < blocks; i++)
    {
        cfb_decrypt(arr[i].string1, BLOCK_SIZE, key, key_size, iv, BLOCK_SIZE);
        std::cout << arr[i].string1 << ' ';
    } std::cout << '\n'; 
}

// size_t reduceStr(char data[], int size, str* arr) {
//     int comp = size % BLOCK_SIZE;
//     float sse = (float) size / (float) BLOCK_SIZE;
//     int blockNum = std::ceil(sse);    

//     if (comp == 0)
//     {
//         size_t k = 0;
//         while (k != blockNum)
//         {
//             size_t i = 0;
//             for (i; i < BLOCK_SIZE * (k + 1); i++)
//             {
//                 arr[k].string1[i] = data[i];
//             }
//             k++;
//         }       
//     } else {
//         size_t k = 0;
//         while (k != blockNum - 1)
//         {
//             for (size_t i = 0; i < BLOCK_SIZE * (k + 1); i++)
//             {
//                 arr[k].string1[i] = data[i];
//             }
//             k++;
//         } 
//         while (k != blockNum)
//         {
//             for (size_t i = 0; i < comp - 1; i++)
//             {
//                 arr[k].string1[i] = data[i];
//             }
//             for (size_t i = comp; i < BLOCK_SIZE * (k + 1); i++)
//             {
//                  arr[k].string1[i] = ' ';
//             }
//             k++;                        
//         }    
//     }
//     return blockNum; 
// }
