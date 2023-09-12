#include <iostream> 
#include <math.h>
#include <string>
#include <vector>
#include <cstdint>

#define n 13
#define g 171

class DiffieHellman {
    public: 
    int calculation_mod(int secretNumber) {
        int mod = (int) pow(g, secretNumber) % n;
        return mod;
    }

    int calculation_key(int degree, int secretNumber) {
        int base = calculation_mod(secretNumber);
        int key = 0;
        return key = (int) pow(base, degree) % n;
    }
};

class RC2 {
    public:
    const int R_Size = 4;
    std::vector<int> S = {1, 2, 3, 5};

    std::vector<uint64_t> convert_string_uint(std::string message) {
        std::vector<uint64_t> words(R_Size, 0);

        if(message.size() < R_Size * sizeof(uint64_t)) {
            std::cout << "Stroke too short";
            return;
        }
        for(size_t i{0}; i < R_Size; ++i) {
            memcpy(&words[i], message.data() + i * sizeof(uint64_t), sizeof(uint64_t));
        }
        return words;
    }

    std::string convert_uint_string(std::vector<uint64_t> words) {
        std::string message(words.size() * sizeof(uint64_t), '\0');

        for(size_t i {0}; i < words.size(); ++i) {
            memcpy(&message[i*sizeof(uint64_t)], &message[i], sizeof(uint64_t));
        }
        return message;
    }

    std::vector<uint64_t> key_extansion(int key) {
        std::string key_s = std::to_string(key);
        std::vector<uint64_t> L;
        for(auto k : key_s) {
            L.push_back(k);
        }
        int T = sizeof(int);
        int T1 = key_s.size();
        int T8 = (T1 + 7) / 8;
        uint64_t TM = 255 % (int) pow(2, (8 + T1 - 8 * T8));

        std::vector<uint64_t> Pitable = {0xd9, 0x78, 0xf9, 0xc4, 0x19, 0xdd, 0xb5, 0xed, 0x28, 0xe9, 0xfd, 0x79, 0x4a, 0xa0, 0xd8, 0x9d,
                                        0xc6, 0x7e, 0x37, 0x83, 0x2b, 0x76, 0x53, 0x8e, 0x62, 0x4c, 0x64, 0x88, 0x44, 0x8b, 0xfb, 0xa2,
                                        0x17, 0x9a, 0x59, 0xf5, 0x87, 0xb3, 0x4f, 0x13, 0x61, 0x45, 0x6d, 0x8d, 0x09, 0x81, 0x7d, 0x32,
                                        0xbd, 0x8f, 0x40, 0xeb, 0x86, 0xb7, 0x7b, 0x0b, 0xf0, 0x95, 0x21, 0x22, 0x5c, 0x6b, 0x4e, 0x82,
                                        0x54, 0xd6, 0x65, 0x93, 0xce, 0x60, 0xb2, 0x1c, 0x73, 0x56, 0xc0, 0x14, 0xa7, 0x8c, 0xf1, 0xdc,
                                        0x12, 0x75, 0xca, 0x1f, 0x3b, 0xbe, 0xe4, 0xd1, 0x42, 0x3d, 0xd4, 0x30, 0xa3, 0x3c, 0xb6, 0x26,
                                        0x6f, 0xbf, 0x0e, 0xda, 0x46, 0x69, 0x07, 0x57, 0x27, 0xf2, 0x1d, 0x9b, 0xbc, 0x94, 0x43, 0x03,
                                        0xf8, 0x11, 0xc7, 0xf6, 0x90, 0xef, 0x3e, 0xe7, 0x06, 0xc3, 0xd5, 0x2f, 0xc8, 0x66, 0x1e, 0xd7,
                                        0x08, 0xe8, 0xea, 0xde, 0x80, 0x52, 0xee, 0xf7, 0x84, 0xaa, 0x72, 0xac, 0x35, 0x4d, 0x6a, 0x2a,
                                        0x96, 0x1a, 0xd2, 0x71, 0x5a, 0x15, 0x49, 0x74, 0x4b, 0x9f, 0xd0, 0x5e, 0x04, 0x18, 0xa4, 0xec,
                                        0xc2, 0xe0, 0x41, 0x6e, 0x0f, 0x51, 0xcb, 0xcc, 0x24, 0x91, 0xaf, 0x50, 0xa1, 0xf4, 0x70, 0x39,
                                        0x99, 0x7c, 0x3a, 0x85, 0x23, 0xb8, 0xb4, 0x7a, 0xfc, 0x02, 0x36, 0x5b, 0x25, 0x55, 0x97, 0x31,
                                        0x2d, 0x5d, 0xfa, 0x98, 0xe3, 0x8a, 0x92, 0xae, 0x05, 0xdf, 0x29, 0x10, 0x67, 0x6c, 0xba, 0xc9,
                                        0xd3, 0x00, 0xe6, 0xcf, 0xe1, 0x9e, 0xa8, 0x2c, 0x63, 0x16, 0x01, 0x3f, 0x58, 0xe2, 0x89, 0xa9,
                                        0x0d, 0x38, 0x34, 0x1b, 0xab, 0x33, 0xff, 0xb0, 0xbb, 0x48, 0x0c, 0x5f, 0xb9, 0xb1, 0xcd, 0x2e,
                                        0xc5, 0xf3, 0xdb, 0x47, 0xe5, 0xa5, 0x9c, 0x77, 0x0a, 0xa6, 0x20, 0x68, 0xfe, 0x7f, 0xc1, 0xad};

        for (size_t i{T}; i < 127; ++i) {
            L[i] = Pitable[(L[i - 1] + L[i - T]) % 256];
        }
        L[128 - T8] = Pitable[L[128 - T8] & TM];
        for (size_t i = 128 - T8; i >= 0; --i)
        {
            L[i] = Pitable[L[i + 1] ^ L[i + T8]];
        }
        std::vector<uint64_t> K(64, 0);
        for(size_t i{0}; i < 64; ++i) {
            K[i] = L[2 * i] + 256 * L[2 * i + 1];
        }
    }

    void Mix(std::vector<uint64_t>& R, const std::vector<uint64_t>& K, int& j) {
        for (int i = 0; i < R_Size; ++i) {
        int idx1 = (i + 1) % R_Size;
        int idx2 = (i + 2) % R_Size;
        int idx3 = (i + 3) % R_Size;

        R[i] = R[i] + K[j] + (R[idx1] & R[idx2]) + (~R[idx1] & R[idx3]);
        j = (j + 1) % K.size();
        R[i] = R[i] << S[i];
        }
    }
    void Mash(std::vector<uint64_t>& R, const std::vector<uint64_t>& K, int j) {
        for (int i = 0; i < R_Size; ++i) {
        R[i] = R[i] + K[R[i - 1] & 63];
        }
    }

    std::vector<uint64_t> encrypt(std::vector<uint64_t>& R, const std::vector<uint64_t>& K) {

        std::vector<uint64_t> R(R_Size, 0);
        int j = 0;

        for (int i = 0; i < 5; ++i) {
            Mix(R, K, j);
        }
        Mash(R, K, j);
        for (int i = 0; i < 6; ++i) {
            Mix(R, K, j);
        }
        Mash(R, K, j);
        for (int i = 0; i < 5; ++i) {
            Mix(R, K, j);
        }
        return R;
    }

    void RMash(std::vector<uint64_t>& R, const std::vector<uint64_t>& K, int j) {
        for (int i = R_Size - 1; i >= 0; --i) {
            R[i] = R[i] - K[R[i - 1] & 63];
        }
    }

    void RMix(std::vector<uint64_t>& R, const std::vector<uint64_t>& K, int& j, const std::vector<int>& S) {
        for (int i = R_Size - 1; i >= 0; --i) {
            int idx1 = (i + 1) % R_Size;
            int idx2 = (i + 2) % R_Size;
            int idx3 = (i + 3) % R_Size;

            R[i] = R[i] >> S[i];
            R[i] = R[i] - K[j] - (R[idx1] & R[idx2]) - (~R[idx1] & R[idx3]);
            j = (j - 1) % K.size();
        }
    }

    std::vector<uint64_t> decrypt(std::vector<uint64_t>& R, const std::vector<uint64_t>& K) {
        int j = 63; 
        for (int i = 0; i < 5; ++i) {
            RMix(R, K, j, S);
        }
        RMash(R, K, j);
        for (int i = 0; i < 6; ++i) {
            RMix(R, K, j, S);
        }
        RMash(R, K, j);
        for (int i = 0; i < 5; ++i) {
            RMix(R, K, j, S);
        }
        return R;
    }
};

class User : public DiffieHellman, public RC2 {
    public:
    User() {}
    User(int number) : number(number) {}

    void gen_number() {
        this->number = std::abs(std::rand() % 20 + 10);
    }

    void init_secret_number(int secretNumber) {
        this->secretNumber = secretNumber;
    }
    void send_secret_number(User *reciever) {
        reciever->init_secret_number(this->number);
    }

    void calculate_key() {
        key = DiffieHellman::calculation_key(number, secretNumber);
    }
    void encrypt() {
        std::vector<uint64_t> extansionKey = RC2::key_extansion(this->key);
        std::vector<uint64_t> convertMessage = RC2::convert_string_uint(message);
        std::vector<uint64_t> ecnryptMessage = RC2::encrypt(convertMessage, extansionKey);
        for (auto e : ecnryptMessage) {
            std::cout << e << ' ';
        }
        std::string str_enc = RC2::convert_uint_string(ecnryptMessage);
        this->message = str_enc;        
    }
    void decrypt() {
        std::vector<uint64_t> extansionKey = RC2::key_extansion(this->key);
        std::vector<uint64_t> convertMessage = RC2::convert_string_uint(message);
        std::vector<uint64_t> ecnryptMessage = RC2::decrypt(convertMessage, extansionKey);
        std::string str_enc = RC2::convert_uint_string(ecnryptMessage);
        this->message = str_enc;
    }

    void gen_message() {
        message.resize(sizeof(number) + sizeof(secretNumber));
        message = std::to_string(number) + std::to_string(secretNumber);
    }
    void init_message(std::string sendingMessage) {
        recievedMessage = sendingMessage;
    }
    void send_message(User *reciever) {
        reciever->init_message(message);
    }

    class Subscription {
        public:
        void subscription(User *user) {
            std::cout << "Subscription was successful according to the key: " << user->key << std::endl;
        }
        void verify_subscription(User *user) {
            std::cout << "Key verification was successful: " << user->key << std::endl;
        }        
    };

    private:
    int number;
    int secretNumber;
    int key;
    std::string message;
    std::string recievedMessage;
};

// void PPProtocol() {
//     User Alisa, Bob;
//     User::Subscription thirdSide;
//
//     Alisa.gen_number();
//     Alisa.send_secret_number(&Bob);
//     // 1
//     Bob.gen_number();
//     Bob.calculate_key();
//     Bob.gen_message();
//     thirdSide.subscription(&Bob);
//     Bob.send_secret_number(&Alisa);
//     Bob.encrypt();
//     Bob.send_message(&Alisa);
//     // 2
//     Alisa.calculate_key();
//     Alisa.decrypt();
//     thirdSide.verify_subscription(&Bob);
//     thirdSide.subscription(&Alisa);
//     Alisa.gen_message();
//     Alisa.encrypt();
//     // 3
//     Bob.decrypt();
//     thirdSide.verify_subscription(&Alisa);
//     // 4
// }

int main() {
    User Alisa, Bob;
    User::Subscription thirdSide;

    Alisa.gen_number();
    Alisa.send_secret_number(&Bob);
    // 1
    Bob.gen_number();
    Bob.calculate_key();
    Bob.gen_message();
    thirdSide.subscription(&Bob);
    Bob.send_secret_number(&Alisa);
    Bob.encrypt();
    Bob.send_message(&Alisa);
    // 2
    Alisa.calculate_key();
    Alisa.decrypt();
    thirdSide.verify_subscription(&Bob);
    thirdSide.subscription(&Alisa);
    Alisa.gen_message();
    Alisa.encrypt();
    // 3
    Bob.decrypt();
    thirdSide.verify_subscription(&Alisa);
    // 4

    // PPProtocol();
}