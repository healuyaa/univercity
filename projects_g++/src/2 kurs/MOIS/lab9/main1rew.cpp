#include <iostream>
#include <vector>

void factorize(int n, std::vector<int> &coefficients, std::vector<int> &factors) {
    int i = 2;
    factors.push_back(1);
    while(n != 0) {
        coefficients.push_back((n % i));
        factors.push_back(i);
        n /= i;
        i++;
    }
}

int factorial(int n) {
    int factor = 1;
    if(n == 0 || n == 1)
        return factor;
    else {
        for (size_t i = 0; i < n; i++)
        {
            factor *= (i + 1);
        }
    }
    return factor;
}

int main() {
    int n;
    std::vector<int> coef;
    std::vector<int> fact;

    std::cout << "Decimal number: ";
    std::cin >> n;

    factorize(n, coef, fact);
    int sum = 0;
    std::cout << "Factorial form of a number: ";
    for (size_t i = 0; i < fact.size() - 1; i++)
    {
        sum += coef[i] * factorial(fact[i]);
        std::cout << coef[i] << "*" << fact[i] << "!";
        if (i == fact.size() - 2) {
            continue;
        } else {
            std::cout << "+";
        }
    } std::cout << std::endl << "Sum of the factorial form of a number: " << sum;
    
    return 0;
}
// max int 2147483647