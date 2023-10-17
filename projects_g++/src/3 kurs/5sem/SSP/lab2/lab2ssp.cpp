#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <functional>

#define ASC 1
#define DESC 2

bool ascendingOrder(int a, int b) {
    return a <= b;
}

bool descendingOrder(int a, int b) {
    return a >= b;
}

bool isSorted(const std::vector<int>& arr, std::function<bool(int, int)> compare_function) {
    for (size_t i{1}; i < arr.size(); ++i)
    {
        if (!compare_function(arr[i-1], arr[i]))
            return false;
    }
    return true;
}

int main() {
    std::fstream file("./src/3 kurs/5sem/SSP/lab2/input.txt");
    int length;
    file >> length;

    std::vector<int> numbers;
    for (int i = 0; i < length; ++i) {
        int num;
        file >> num;
        numbers.push_back(num);
    }
    file.close();

    std::cout << "sort: 1 - ASC, 2 - DESC" << std::endl;
    int sort_order;
    std::cin >> sort_order;

    bool (*compare_function)(int, int) = nullptr;
    if(sort_order == 1) {
        compare_function = ascendingOrder;
    } else if(sort_order == 2) {
        compare_function = descendingOrder;
    } else {
        std::cout << "idk" << std::endl;
    }

    std::fstream file1("./src/3 kurs/5sem/SSP/lab2/output.txt");
    std::sort(numbers.begin(), numbers.end(), compare_function);
    for (int num : numbers) {
        file1 << num << ' ';
    }

    if (isSorted(numbers, compare_function)) {
        std::cout << "Sort good" << std::endl;
    } else {
        std::cout << "Sort bad" << std::endl;
    }

    file1.close();

    return 0;
}
