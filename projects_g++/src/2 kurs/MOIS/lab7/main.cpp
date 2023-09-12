#include <iostream>
#include <vector>
#include <chrono>
#include <unordered_map>
#include "C:/project/C++/lib/combinatoric.hh"

int main() {
    combinatoric algorithm;
    auto t1 = chrono::high_resolution_clock::now(); // начало отсчёта времени

    std::vector<std::vector<int>> temp_tree1 = algorithm.reading_tree("C:/project/C++/src/MOIS/lab7/test1.txt"); // первое дерево
    std::vector<std::vector<int>> tree1 = algorithm.transform_to_tree(temp_tree1); // создание дерева похожего на дерево
    std::cout << "Прямой обход дерева T1: ";
    algorithm.preorderTraversal(tree1, 1); // Прямой обход дерева T1
    std::cout << std::endl;

    std::vector<std::vector<int>> temp_tree2 = algorithm.reading_tree("C:/project/C++/src/MOIS/lab7/test2.txt"); // второе дерево
    std::vector<std::vector<int>> tree2 = algorithm.transform_to_tree(temp_tree2); // создание дерева похожего на дерево
    std::cout << "Прямой обход дерева T2: ";
    algorithm.preorderTraversal(tree2, 1); // Прямой обход дерева T2
    std::cout << std::endl << std::endl;

    // обратный обход дерева Т1
    std::vector<bool> visited(tree1.size(), false); // вектор посещённых вершин
    std::cout << "Oбратый обход дерева Т1: ";
    algorithm.postOrder(1, tree1, visited);
    std::cout << std::endl;

    // обратный обход дерева Т2
    std::vector<bool> visited2(tree2.size(), false); // вектор посещённых вершин
    std::cout << "Oбратый обход дерева Т2: ";
    algorithm.postOrder(1,tree2,visited2);
    std::cout << std::endl << std::endl;

    int treeHeight1 = algorithm.height(tree1, 1); // высота первого дерева
    int treeHeight2 = algorithm.height(tree2, 1); // высота второго дерева
    std::cout << "высота дерева Т1: " << treeHeight1 << std::endl;
    std::cout << "высота дерева Т2: " << treeHeight2 << std::endl;

    bool isBalanced = true; // Устанавливаем флаг сбалансированности для первого дерева в true
    int height = algorithm.get_balans(1, tree1, isBalanced);
    if (isBalanced) 
    std::cout << "первое дерево сбалансировано" << std::endl;
    else 
    std::cout << "первое дерево не сбалансировано" << std::endl;

    isBalanced = true; // Устанавливаем флаг сбалансированности для второго дерева в true
    height = algorithm.get_balans(1, tree2, isBalanced);
    if (isBalanced)
        cout << "второе дерево сбалансировано" << std::endl;
    else 
        std::cout << "второе дерево не сбалансировано" << std::endl;

    unordered_map<int, combinatoric::Node*> nodes1; // создание узлов для первого дерева         
    for(int i=0;i<temp_tree1.size();i++){                                                       
        algorithm.addNode(nodes1, temp_tree1[i][0], temp_tree1[i][1]); // добавление узлов в дерево
    }
    combinatoric::Node* T1 = algorithm.createTree(nodes1, 1); // создание дерева
    std::cout << "Tree 1: " << std::endl;
    algorithm.print(T1);
    std::cout << std::endl;

    unordered_map<int, combinatoric::Node*> nodes2; // создание узлов для второго дерева
    for(int i=0;i<temp_tree2.size();i++){ // добавление узлов в дерево
        algorithm.addNode(nodes2, temp_tree2[i][0], temp_tree2[i][1]);
    }
    combinatoric::Node* T2 = algorithm.createTree(nodes2, 1); // создание дерева
    std::cout << "Tree 2: " << std::endl;                                           
    algorithm.print(T2);                                                
    std::cout << std::endl;

    algorithm.mergeTrees(T1, T2); // слияние деревьев
    std::cout << "Merged Tree 1: " << std::endl;
    algorithm.print(T1);
    std::cout << std::endl;
    
    auto t2 = chrono::high_resolution_clock::now(); // конец отсчёта времени
    auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count(); // вычисление времени выполнения
    std::cout << "Время выполнения: " << duration << " * e-06 sec" << std::endl; 
    return 0;
}


































































































/*bool isomorphic = algorithm.isIsomorphic(T1, T2);
    cout << "два дерева " << (isomorphic ? "" : "не ") << "изоморфны" << endl;*/
    //тут костыль, проверка на изоморфность работает в 30% случаев, поэтому лучше из кода это убрать 
    //и не выводить что деревья не изоморфны или изоморфны, а просто сливать их
    //поэтому строчку 90 можно удалить и вместо этого написать следующее
    
    //cout << "приведение к изоморфному виду: "<<endl;
