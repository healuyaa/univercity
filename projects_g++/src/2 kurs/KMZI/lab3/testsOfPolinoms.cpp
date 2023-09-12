#include <iostream>
#include <bitset>

// примитивные многочлены
const int poly1 = 0b10001111;  // x^7 + x^3 + x^2 + x + 1 
const int poly2 = 0b10000011;  // x^7 + x + 1
const int poly3 = 0b10001001;  // x^7 + x^3 + 1

int main() {
    int seed = 0b10101010;  // начальное значение
    for (int i = 0; i < 10; i++) {
        std::cout << seed << ": " << std::bitset<8>(seed) << std::endl;
        bool drop_bit = seed & 0b10000000;      
        seed = seed << 1;
        if (drop_bit) {
            seed = seed ^ poly1;
            seed = seed ^ poly2;
            seed = seed ^ poly3;
        }
    }
    return 0;
}
