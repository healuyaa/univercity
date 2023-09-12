#include <iostream>
#include <math.h>
#include <algorithm>

void fermat_factor(long long n, long long &p, long long &q, int k) {
    long long a = std::ceil(sqrt(n));
    long long b2 = a * a - n;
    long long b = sqrt(b2);

    while (b2 != b * b) {
        a += 1;
        b2 = a * a - n;
        if (b2 < 0) {
            b = 0;
            break;
        }
        b = sqrt(b2);
    }

    if (b != 0) {
        p = a + b;
        q = a - b;
        return;
    }

    int i = 1;
    long long j = 2;

    while (i <= k) {
        long long x = a * a - n;

        for (int s = 1; s <= i; s++) {
            x = x * x - n;
        }

        long long y = sqrt(x);

        if (y * y == x) {
            p = a - y;
            q = a + y;
            return;
        }

        i += 1;

        if (i > j) {
            a = (a + j) / 2;
            j *= 2;
        }
    }

    p = -1;
    q = -1;
    return;
}

int main() {
    long long n = 401;
    long long p, q;

    fermat_factor(n, p, q, 100);

    if (p == -1 && q == -1) {
        std::cout << "Failed to factorize " << n << std::endl;
    } else {
        std::cout << "n = " << n << std::endl;
        std::cout << "p = " << p << std::endl;
        std::cout << "q = " << q << std::endl;
    }

    return 0;
}

// для нечётных чисел только