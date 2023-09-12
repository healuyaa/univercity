#include <iostream>
#include <cstring>

// Функция-скремблер (сдвиг вправо)
void scrambler(unsigned char* buffer, int size, unsigned char key)
{
    unsigned char prev = key;
    for (int i = 0; i < size; i++)
    {
        unsigned char curr = buffer[i];
        buffer[i] = (curr >> 1) | (prev << 7);
        prev = buffer[i];
    }
}

// Функция CFB с длиной блока 7 байт
void cfb(unsigned char* buffer, int size, unsigned char key, unsigned char* iv, bool encrypt)
{
    unsigned char prev = key;
    for (int i = 0; i < size; i++)
    {
        scrambler(iv, 7, key); // Генерация следующего вектора инициализации
        if (encrypt)
        {
            prev = buffer[i] ^= prev;
        }
        else
        {
            unsigned char next = buffer[i];
            buffer[i] ^= iv[0]; // Используем текущий вектор инициализации для дешифрации
            prev = next;
        }
    }
}


int main()
{
    // Генерируем ключ
    const unsigned char key = 0xF;

    // Создаем объект CFB
    unsigned char iv[7] = {0};
    memcpy(iv, "abcdefg", 8);

    // Шифруем сообщение с помощью CFB
    const unsigned char* message = (const unsigned char*)"privet";
    unsigned char encrypted_message[7] = {0};
    int message_length = strlen((const char*)message);
    memcpy(encrypted_message, message, message_length); // Копируем сообщение в зашифрованное

    cfb(encrypted_message, message_length, key, iv, true);

    // Дешифруем сообщение с помощью CFB
    unsigned char decrypted_message[7] = {0};
    memcpy(decrypted_message, encrypted_message, message_length); // Копируем зашифрованное сообщение в расшифрованное

    cfb(decrypted_message, message_length, key, iv, false);

    // Выводим результаты
    std::cout << "Original message: " << message << std::endl;
    std::cout << "Encrypted message: ";
    for (int i = 0; i < strlen((const char*)message); i++)
        std::cout << std::hex << (int)encrypted_message[i];
    std::cout << std::endl;
    std::cout << "Decrypted message: " << decrypted_message << std::endl;

    return 0;
}
