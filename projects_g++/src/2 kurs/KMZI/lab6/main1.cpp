#include <iostream>
#include <bitset>

bool parity_bit(std::bitset<8> &bits){
    bool parity = 0;
    for(int i=0; i<8; i++)
        parity ^= bits[i];
    return parity;
}

int main(){
    char data;
    while (true){
        std::cout << "Enter the data to be transmitted: ";
        std::cin >> data;

        std::bitset<8> bits(data);
        std::cout << "Data: " << bits << ", parity: " << parity_bit(bits) << std::endl;
    }
}