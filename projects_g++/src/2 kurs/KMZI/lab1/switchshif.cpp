#include <iostream>
#include <string>
#include <fstream>

// std::string readFileText() {
//     std::fstream file("./src/KMZI/lab1/textSwitchShift.txt");
//     if(!file)
//         std::cerr << "Error open file";
    
//     std::string str;
//     getline(file, str);
//     return str;
// }

// std::string readFileKey() {
//     std::fstream file("./src/KMZI/lab1/keySwitchShift.txt");
//     if(!file)
//         std::cerr << "Error open file";
    
//     std::string str;
//     getline(file, str);
//     return str;
// }

std::string encrypt(std::string message, std::string key) {
    std::string result = "";

    // перебираем каждый символ в сообщении
    for (int i = 0; i < message.length(); i++) {
        char c = message[i];
        if (isalpha(c)) { 
            int index = tolower(c) - 'a';
            char newChar = key[index];
            result += isupper(c) ? toupper(newChar) : newChar;
        } else {
            result += c;
        }
    }
    return result;
}

std::string decrypt(std::string message, std::string key) {
    std::string result = "";

    // перебираем каждый символ в сообщении
    for (int i = 0; i < message.length(); i++) {
        char c = message[i];
        if (isalpha(c)) {            
            int index = key.find(tolower(c));            
            char newChar = 'a' + index;
            result += isupper(c) ? toupper(newChar) : newChar;
        } else {
            result += c;
        }
    }
    return result;
}

int main() {
    std::string text = "In scholarly societies and on the pages of scholarly journals, polemics between believers and nonbelievers were endlessly fought.";
    std::string key = "qwertyuiopasdfghjklzxcvbnm";;

    std::string encrypted = encrypt(text, key);
    std::string decrypted = decrypt(encrypted, key);

    std::cout << "Original message: " << text << std::endl;
    std::cout << "Encrypted message: " << encrypted << std::endl;
    std::cout << "Decrypted message: " << decrypted << std::endl;

    return 0;
}
