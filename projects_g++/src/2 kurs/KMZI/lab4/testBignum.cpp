#include <iostream>
#include <cmath>
#include "Bignumber.h"

using namespace pr0crustes;
using namespace std;

BigNumber gcd(BigNumber a, BigNumber b) {
    if (b == 0) 
        return a;
    return gcd(b, a % b);
}

pair<BigNumber, BigNumber> fermat_factorization(BigNumber n) {
    BigNumber a, x, y;
    while (true) {
        a = (BigNumber) rand() % (n - 1) + 1;
        x = a * a - n;
        y = x.pow((BigNumber) 1/2);
        if (y * y == x) {
            BigNumber p = gcd(n, a - y);
            BigNumber q = gcd(n, a + y);
            if (p != 1 && q != 1) {
                return make_pair(p, q);
            }
        }
    }
}

int main() {
    BigNumber n(26);
    pair<BigNumber, BigNumber> factors = fermat_factorization(n);
    cout << "The factors of " << n << " are " << factors.first << " and " << factors.second << endl;
    return 0;
}

// для всех чисел
// С либой не работает