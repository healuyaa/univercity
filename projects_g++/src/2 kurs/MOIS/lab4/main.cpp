#include <iostream>
#include <vector>
#include <chrono>
#include "C:/project/C++/lib/combinatoric.hh"

int main() {
    combinatoric algorithm;
    auto t1 = chrono::high_resolution_clock::now();

    vector<int> permutation = algorithm.reading_file("C:/project/C++/src/MOIS/lab4/test.txt"); //подстановка
    for(int i =0;i<permutation.size();i++){
        std::cout << permutation[i] << std::endl;
    }

    vector<vector<int>> cycles = algorithm.get_cycles(permutation);
    std::cout << "Произведение независимых циклов: ";
    for (vector<int> cycle : cycles) { //вывод циклов
        std::cout << "(";
        for (int i = 0; i < cycle.size(); i++) {
            std::cout << cycle[i] + 1;
            if (i != cycle.size() - 1) {
                std::cout << " ";
            }
        }
        std::cout << ")";
    } std::cout << std::endl << std::endl;

    std::cout << "Степень подстановки: " << algorithm.order(cycles) << std::endl; //подсчёт и вывод степени подстановкi
    algorithm.inversions(permutation); //определение чётности подстановок через число инверсий

    vector<int> transpositions = algorithm.permutationToTranspositions(permutation); //разложение перестановки на произведение транспозиций
    std::cout << "Разложение подстановки на произведение транспозиций" << std::endl;
    for (int i = 0; i < transpositions.size(); i += 2) { //вывод транспозиций 
        std::cout << "(" << transpositions[i] + 1 << " " << transpositions[i + 1] + 1 << ") ";
    }
    if(transpositions.size() / 2 % 2 == 0) 
        std::cout << "Транспозиций = " << transpositions.size() / 2 << " подстановка чётная"<<std::endl;
    else
        std::cout << "Транспозиций = " << transpositions.size() / 2 << " подстановка чётная"<<std::endl;
    std::cout << std::endl;

    auto t2 = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
    std::cout << "Время выполнения: " << duration << " * e-06 sec" << endl;    
    return 0;
}