#include <iostream>
#include <bitset>
#include <vector>

// Функция для скремблирования битов побитовым сдвигом вправо на n бит
std::bitset<8> scramble(std::bitset<8> input, int n) {
    std::bitset<8> output;
    for (int i = 0; i < 8; i++) {
        output[i] = input[(i + n) % 8];
    }
    return output;
}

// Функция для шифрования сообщения с помощью режима CFB с блоком размера 7 байт
std::vector<std::bitset<56>> encrypt_cfb(std::string message, std::bitset<56> key, std::bitset<56> iv) {
    std::vector<std::bitset<56>> encrypted_blocks;
    std::bitset<56> prev_ciphertext_block = iv;

    // Разбиваем сообщение на блоки по 7 байт
    for (int i = 0; i < message.length(); i += 7) {
        std::string block_str = message.substr(i, 7);
        std::bitset<56> block;
        // Заполняем блок нулями до 56 бит, если необходимо
        for (int j = 0; j < 7; j++) {
            if (i + j < message.length()) {
                block <<= 8;
                block |= std::bitset<8>(block_str[j]);
            } else {
                block <<= 8;
            }
        }

        // Шифруем текущий блок побитовым сдвигом вправо скремблера
        std::bitset<56> ciphertext_block = scramble(prev_ciphertext_block ^ block, 3) ^ key;
        encrypted_blocks.push_back(ciphertext_block);
        prev_ciphertext_block = ciphertext_block;
    }

    return encrypted_blocks;
}

// Функция для расшифрования сообщения с помощью режима CFB с блоком размера 7 байт
std::string decrypt_cfb(std::vector<std::bitset<56>> encrypted_blocks, std::bitset<56> key, std::bitset<56> iv) {
    std::string decrypted_message;

    // Расшифровываем каждый блок и конкатенируем их
    std::bitset<56> prev_ciphertext_block = iv;
    for (int i = 0; i < encrypted_blocks.size(); i++) {
        std::bitset<56> ciphertext_block = encrypted_blocks[i];
        std::bitset<56> plaintext_block = scramble(ciphertext_block ^ key, 3) ^ prev_ciphertext_block;
        prev_ciphertext_block = ciphertext_block;
        for (int j = 6; j >= 0; j--) {
            if (plaintext_block[j * 8] != 0) {
                decrypted_message += static_cast<char>(plaintext_block >> (j * 8) & 0xff);
            }
        }
    }

    return decrypted_message;
}

int main() {
    // Генерируем ключ и вектор инициализации
    std::bitset<56> key("101010101010101010101010101010101010101");
    std::bitset<56> iv("0101010101010101010101010101010101010101");
    // Шифруем сообщение
std::string message = "Hello, world!";
std::vector<std::bitset<56>> encrypted_blocks = encrypt_cfb(message, key, iv);

// Выводим зашифрованные блоки
std::cout << "Encrypted blocks: ";
for (auto block : encrypted_blocks) {
    std::cout << block.to_string() << " ";
}
std::cout << std::endl;

// Расшифровываем сообщение
std::string decrypted_message = decrypt_cfb(encrypted_blocks, key, iv);

// Выводим расшифрованное сообщение
std::cout << "Decrypted message: " << decrypted_message << std::endl;

return 0;
}