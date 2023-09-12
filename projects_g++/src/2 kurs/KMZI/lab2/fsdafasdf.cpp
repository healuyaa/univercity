#include <iostream>
#include <bitset>
#include <vector>

using namespace std;

// Функция скремблирования
void scrambler(vector<unsigned char>& data, unsigned char seed) {
    // Инициализация регистра сдвига
    bitset<8> reg(seed);
    // Проход по всем байтам входных данных
    for (unsigned int i = 0; i < data.size(); i++) {
        // Вычисление маски для текущего байта
        bitset<8> mask;
        for (unsigned int j = 0; j < 7; j++) {
            mask[j] = reg[j];
        }
        // Применение маски к текущему байту
        data[i] ^= mask.to_ulong();
        // Сдвиг регистра вправо
        reg >>= 1;
        // Регистр - обратная связь
        if (data[i] & 0x01) {
            reg[7] = 1;
        } else {
            reg[7] = 0;
        }
    }
}

// Функция де-скремблирования
void descrambler(vector<unsigned char>& data, unsigned char seed) {
    // Инициализация регистра сдвига
    bitset<8> reg(seed);
    // Проход по всем байтам входных данных
    for (unsigned int i = 0; i < data.size(); i++) {
        // Вычисление маски для текущего байта
        bitset<8> mask;
        for (unsigned int j = 0; j < 7; j++) {
            mask[j] = reg[j];
        }
        // Применение маски к текущему байту
        data[i] ^= mask.to_ulong();
        // Сдвиг регистра вправо
        reg >>= 1;
        // Регистр - обратная связь
        if (data[i] & 0x01) {
            reg[7] = 1;
        } else {
            reg[7] = 0;
        }
    }
}

int main() {
    // Входные данные
    vector<unsigned char> data = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    // Сид для скремблирования
    unsigned char seed = 0x7F;
    // Скремблирование
    scrambler(data, seed);
    // Вывод зашифрованных данных
    cout << "Зашифрованные данные: ";
    for (unsigned int i = 0; i < data.size(); i++) {
        cout << hex << static_cast<int>(data[i]) << " ";
    }
    cout << endl;
    // Де-скремблирование
    descrambler(data, seed);
    // Вывод расшифрованных данных
    cout << "Расшифрованные данные: ";
    for (unsigned int i = 0; i < data.size(); i++) {
        cout << hex << static_cast<int>(data[i]) << " ";
}
cout << endl;
return 0;
}
