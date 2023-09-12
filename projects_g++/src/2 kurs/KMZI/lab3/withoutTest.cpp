#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

// Функция, генерирующая комбинации из заданного списка элементов
std::vector<std::vector<int>> generate_combinations(std::vector<int> elements, int num_combinations) {
    std::vector<std::vector<int>> combinations;
    std::random_device rd;
    std::mt19937 gen(rd());
    
    for (int i = 0; i < num_combinations; i++) {
        std::vector<int> combination;
        std::uniform_int_distribution<> dis(0, elements.size() - 1);
        
        while (combination.size() < elements.size() / 2) {
            int index = dis(gen);
            if (std::find(combination.begin(), combination.end(), elements[index]) == combination.end()) {
                combination.push_back(elements[index]);
            }
        }
        combinations.push_back(combination);
    }
    
    return combinations;
}

int main() {
    std::vector<int> elements = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int num_combinations = 5;
    
    std::vector<std::vector<int>> combinations = generate_combinations(elements, num_combinations);
    
    std::cout << "Generated " << combinations.size() << " combinations:" << std::endl;
    for (auto combination : combinations) {
        std::cout << "{ ";
        for (auto element : combination) {
            std::cout << element << "";
        }
        std::cout << " }" << std::endl;
    }
    
    return 0;
}
