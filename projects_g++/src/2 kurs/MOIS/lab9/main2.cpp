#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>

void readFileTable(std::vector<int>& inv) {
    std::fstream file("./src/MOIS/lab9/table.txt");
    if(!file.is_open())
       std::cerr << "Error open file" << std::endl;

    int streamInt;    
    while(file >> streamInt)
        inv.push_back(streamInt);        
}

int main() {
    double start = clock();
    std::vector<int> table;
    readFileTable(table);
    int n = table.size();

    size_t tts = 0;
    std::vector<int> result;
    for (size_t j = 0; j < n; j++)
    {
        for (size_t i = 0; i < n; i++) {
        if (table[i] == tts)
            result.push_back(i);       
    }
    tts++;
    }    
    
    for (size_t i = 0; i < n; i++)
        std::cout << result[i] + 1 << ' ';
        
    std::cout << std::endl;
    double end = clock();
    std::cout << "Time processing: " << (double) (end - start) / CLOCKS_PER_SEC;
    return 0;      
}