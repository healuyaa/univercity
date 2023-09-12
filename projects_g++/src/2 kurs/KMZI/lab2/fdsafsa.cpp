#include <iostream>
#include <cstring>

using namespace std;

const int BLOCK_SIZE = 7;

// Функция, реализующая скремблирование
void Scramble(unsigned char* buffer, int len, unsigned char* key)
{
    unsigned char shift_register = 0;
    int i, j;
    for (i = 0; i < len; ++i)
    {
        // Получаем бит, который будем вставлять в блок данных
        unsigned char feedback = 0;
        for (j = 0; j < 8; ++j)
        {
            feedback |= (shift_register & (1 << (7 - j))) >> (7 - j);
        }
        buffer[i] ^= feedback;

        // Сдвигаем регистр вправо и вставляем новый бит
        shift_register >>= 1;
        shift_register |= feedback << 7;

        // Обновляем ключ
        if ((i + 1) % BLOCK_SIZE == 0)
        {
            unsigned char temp_key[BLOCK_SIZE];
            for (j = 0; j < BLOCK_SIZE; ++j)
            {
                temp_key[j] = buffer[i - BLOCK_SIZE + 1 + j];
            }
            for (j = 0; j < BLOCK_SIZE; ++j)
            {
                key[j] = temp_key[j] ^ key[j];
            }
        }
        else if (i == len - 1)
        {
            // Обработка оставшейся части ключа, если данных осталось меньше, чем длина блока
            unsigned char temp_key[BLOCK_SIZE];
            int remaining_bytes = len % BLOCK_SIZE;
            if (remaining_bytes == 0) remaining_bytes = BLOCK_SIZE;
            for (j = 0; j < remaining_bytes; ++j)
            {
                temp_key[j] = buffer[i - remaining_bytes + 1 + j];
            }
            for (j = 0; j < remaining_bytes; ++j)
            {
                key[j] = temp_key[j] ^ key[j];
            }
        }
    }
}

int main()
{
    unsigned char data[] = "first";
    unsigned char key[BLOCK_SIZE] = {0x1F, 0x64, 0x5A, 0x9D, 0xC6, 0xF8, 0xFF};
    int len = strlen((const char*)data);

    cout << "Original data: " << data << endl;

    // Скремблируем данные
    Scramble(data, len, key);

    cout << "Scrambled data: " << data << endl;

    // Раскремблируем данные
    Scramble(data, len, key);

    cout << "Unscrambled data: " << data << endl;

    return 0;
}
