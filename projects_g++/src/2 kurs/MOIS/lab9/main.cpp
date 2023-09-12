#include <iostream>
#include <vector>
#include <ctime>

int main() {
    int n, m;
    std::cin >> n;
    m = n;
    double start = clock();
    std::vector<int> coefficients; // вектор для хранения коэффициентов a_i

    int i = 1;
    while (n > 0) {
        coefficients.push_back(n % i); // вычисляем коэффициент a_i
        n /= i; 
        i++; 
    }

    // выводим представление числа в факториальной системе счисления
    std::cout << m << '=';
    size_t iter = coefficients.size() - 1;
    for (int j = coefficients.size() - 1; j >= 0; j--) {
        std::cout << iter << '*' << coefficients[j] << '!';
        iter--;
    }
    std::cout << std::endl;

    double end = clock();
    std::cout << "Time processing: " << (double) (end - start) / CLOCKS_PER_SEC << " sec";
    return 0;
}
