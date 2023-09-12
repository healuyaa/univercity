#include<iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <set>
using namespace std;

class combinatoric{
    public:
        vector<int> reading_file(string input){
            ifstream input_file(input); // открыть файл для чтения
            vector<int> numbers; // вектор для хранения чисел
            string line; // создать строку для чтения из файла

            while (getline(input_file, line)) { // прочитать строку из файла
                stringstream ss(line); // создать поток для чтения из строки

                char ch; // символ-разделитель между числами
                int number; // переменная для хранения числа

                ss >> ch;
                if (ch != '{') {
                    cerr << "Error: expected '{', found '" << ch << "'\n";
                    break;
                }

                // прочитать числа, разделенные запятой
                while (ss >> number) {
                    numbers.push_back(number); // добавить число в вектор

                    ss >> ch; // прочитать следующий символ
                    if (ch == '}') { break;}// если символ - '}', то закончить чтение
                    else if (ch != ',') { // иначе проверить, что символ - ','
                    cerr << "Error: expected ',' or '}', found '" << ch << "'\n";
                    break;
                    }
                }

                // проверить, что строка заканчивается символом '}'
                if (ch != '}') {
                    cerr << "Error: expected '}', found '" << ch << "'\n";
                    break;
                }
            }

            return numbers;
        }


        vector<vector<int>> get_cycles(vector<int> permutation) {//получение независимых циклов в перестановке
            vector<vector<int>> cycles;
            vector<bool> visited(permutation.size(), false);

            for (int i = 0; i < permutation.size(); i++) {
                if (!visited[i]) {
                    int current = i;
                    vector<int> cycle;
                    do {
                        visited[current] = true;
                        cycle.push_back(current);
                        current = permutation[current] - 1;
                    } while (current != i);
                cycles.push_back(cycle);
                }
            }
            return cycles;
        }



        int gcd(int a, int b) { // функция для нахождения НОД
            if (b == 0) {
                return a;
            } else {
                return gcd(b, a % b);
            }
        }
        int lcm(int a, int b) { // функция для нахождения НОК
            return a / gcd(a, b) * b;
        }
        int order(vector<vector<int>> cycles){
            int power = 1; // начальная степень равна 1
            for (int i = 0; i < cycles.size(); i++) {
                power = lcm(power, cycles[i].size()); // находим НОК длин циклов
            }
            return power;
        }



        void inversions(vector<int> permutation){
            vector<int> invers;
            int index_min = 0;
            int count_more_num = 0;

            do{
                int min_element = 100000;//переменная для нахождения минимального числа в подстановке

                for (int i = 0; i < permutation.size(); i++) { // Перебираем все элементы вектора
                    if (permutation[i] < min_element) { // Если текущий элемент меньше минимального
                        min_element = permutation[i]; // Обновляем значение минимального элемента
                        index_min = i;
                    }
                }

                for(int i = 0; i < index_min; i++){//нахождение инверсий 
                    if(permutation[i] > min_element){
                        count_more_num += 1;
                    }
                }

                invers.push_back(count_more_num);//добавляем количество инверсий
                auto it = permutation.begin() + index_min;// Получаем итератор на элемент по индексу
                permutation.erase(it);// Удаляем элемент из вектора 
                count_more_num = 0; //обнуляем количество инверсий, для дальнейшего поиска новых инверсий

            }while(permutation.size()!=0);

            int sum = 0;
            for(int i = 0; i < invers.size(); i++){//нахождение количества всех инверсий в подстановке
                sum += invers[i];
            }

            cout<<"количество инверсий: "<<sum<<",  ";
            if(sum % 2 == 0) cout<<"подстановка чётная"<<endl<<endl;
            else cout<<"подстановка нечётная"<<endl<<endl;
        
        }

        vector<int> permutationToTranspositions(vector<int> permutation) {
            int n = permutation.size();
            vector<bool> used(n, false);    // Массив флагов, отмечающий использованные элементы
            vector<int> transpositions;     // Вектор транспозиций
            for (int i = 0; i < n; i++) {   
                if (used[i]) continue;      // Если элемент уже использован, то переходим к следующему

                int j = i;                  // Текущий элемент
                vector<int> cycle;          // Цикл, начинающийся с текущего элемента
                do {
                    cycle.push_back(j);     // Добавляем текущий элемент в цикл
                    used[j] = true;         // Отмечаем текущий элемент как использованный
                    j = permutation[j] - 1; // Переходим к следующему элементу в цикле
                } while (j != i);           // Пока не вернулись в начало цикла

            // Преобразуем цикл в произведение транспозиций
            for (int k = 0; k < cycle.size() - 1; k++) {
                transpositions.push_back(cycle[k]);     // Первый элемент транспозиции
                transpositions.push_back(cycle[k + 1]); // Второй элемент транспозиции
            }
        }   
        return transpositions;
    }




    vector<vector<int>> reading_tree(string path){
        ifstream file(path);
        vector<char> letters; //вектор для хранения букв

        string line;
        while (getline(file, line)) {
            for (int i = 0; i < line.size(); i++) {
                if (line[i] >= 'a' && line[i] <= 'z') { // если символ - буква
                    letters.push_back(line[i]); // добавляем букву в вектор
                }
            }
        }file.close();
        for(int i = 0;i<letters.size();i++){
            cout<<letters[i] << " ";
        }
        vector<vector<int>> numbers(letters.size()/2,vector<int>(2,0)); // новый вектор для хранения чисел
        for (int i = 0,k = 0; i < letters.size(); i+=2,k++) {
                numbers[k][0] = letters[i] - 'a' + 1;//преобразование букв в числа
                numbers[k][1] = letters[i + 1] - 'a' + 1;//преобразование букв в числа            
        }
        return numbers;

    }


    vector<vector<int>> transform_to_tree(vector<vector<int>>& temp_tree){
        int n=0;//переменная которая хранит максимальный узел
        for(int i=0;i<temp_tree.size();i++){//циклы для нахождения максимального узла
            for(int j=0;j<2;j++){
                if(temp_tree[i][j]>n){
                    n = temp_tree[i][j];
                }
            }
        }
        vector<vector<int>> tree1(n + 1);//создание дерева в виде дерева))) а не в виде списков рёбер
        for(int i=0;i<temp_tree.size();i++){
            tree1[temp_tree[i][0]].push_back(temp_tree[i][1]);
        }
        return tree1;
    }
    
    void preorderTraversal(vector<vector<int>>& tree, int node) {//прямой обход дерева
        cout << node << " ";  // посетить текущий узел

        for (int child : tree[node]) {
            preorderTraversal(tree, child);  // рекурсивно посетить левое и правое поддеревья
        }
    }

    int height(vector<vector<int>>& tree, int node) {//нахождение высоты дерева
        int h = 0;
        for(int child : tree[node]) {
            h = max(h, height(tree, child));
        }
        return h + 1;
    }

    int get_balans(int node, vector<vector<int>>& tree, bool& is_balanced) {//идеальный баланс)
        vector<int> heights;
        for (int child : tree[node]) {// Для каждого ребенка узла v вычисляем его высоту
            int child_height = get_balans(child, tree, is_balanced);// Рекурсивно вызываем функцию get_height для дочернего узла
            heights.push_back(child_height);// Добавляем высоту дочернего узла в вектор высот
        }

        if (heights.empty()) { // Если у узла node нет дочерних узлов, то его высота равна 1
            return 1;
        }
        sort(heights.begin(), heights.end());// Сортируем вектор высот по возрастанию
        int max_height = heights.back();// Находим максимальную и минимальную высоты дочерних узлов
        int min_height = heights.front();
        if (max_height - min_height > 1) {  // Если разность максимальной и минимальной высот больше 1, то дерево не сбалансировано
            is_balanced = false;
        }

        return max_height + 1; // Возвращаем высоту узла v
    }


    
    void postOrder(int node, vector<vector<int>> &tree, vector<bool> &visited) {// функция для обратного обхода дерева
        
        if (!visited[node]) {// Проверяем, был ли этот узел посещен ранее
            
            visited[node] = true;// Помечаем узел как посещенный
        
            for(int child : tree[node]){// Рекурсивно вызываем функцию для всех дочерних узлов
                postOrder(child, tree, visited);
            }
        
            cout << node << " ";// Выводим значение узла
        }
    }

    bool compare_trees(vector<vector<int>>& tree1, vector<vector<int>>& tree2) {//функция для сравнения деревьев
        vector<bool> visited1(tree1.size(), false);//создание вектора для хранения посещенных узлов
        vector<bool> visited2(tree2.size(), false);//создание вектора для хранения посещенных узлов
        postOrder(0, tree1, visited1);//обратный обход дерева
        cout << endl;
        postOrder(0, tree2, visited2);//обратный обход дерева
        cout << endl;
        for (int i = 0; i < visited1.size(); i++) {//проверка на совпадение посещенных узлов
            if (visited1[i] != visited2[i]) {
                return false;
            }
        }
        return true;
    }
    






    int tabs = 0;//количество табуляций(переменная которая нужна в функции print)
    struct Node {
        int value;//значение узла
        Node* left;
        Node* right;
    };

    Node* createTree(unordered_map<int, Node*>& nodes, int rootValue) {//создание дерева
        if (nodes.count(rootValue) == 0) {//если узел не существует, то создаем его
            Node* root = new Node{rootValue, nullptr, nullptr};//создание узла
            nodes[rootValue] = root;//добавление узла в список узлов
        }
        Node* root = nodes[rootValue];//получение узла из списка узлов
        return root;//
    }

    void addNode(unordered_map<int, Node*>& nodes, int parentValue, int childValue) {//добавление узла
        Node* parent = createTree(nodes, parentValue);//создание родительского узла
        Node* child = createTree(nodes, childValue);//создание дочернего узла
        if (parent->left == nullptr) {//если у родителя нет левого поддерева, то добавляем его
            parent->left = child;
        } else {//если есть, то добавляем правое поддерево
            parent->right = child;
        }
    }

    void traverseInOrder(Node* root) {//вывод дерева начиная с левого поддерева и заканчивая правым(эта функция нигде не используется, но можно использовать чтоб посмотреть обход дерева)
        if (root == nullptr) {//если дерево пустое, то выходим
            return;
        }
        traverseInOrder(root->left);//переход к левому поддереву
        cout << root->value << " ";
        traverseInOrder(root->right);//переход к правому поддереву
    }



    bool isIsomorphic(Node* root1, Node* root2) {//проверка на изоморфность
        if (root1 == nullptr && root2 == nullptr) {//если оба дерева пустые, то они изоморфны
            return true;
        }
        if (root1 == nullptr || root2 == nullptr) {//если одно из деревьев пустое, то они не изоморфны
            return false;
        }
        if (root1->value != root2->value) {//если значения узлов не равны, то деревья не изоморфны
            return false;
        }
        bool isLeftIsomorphic = isIsomorphic(root1->left, root2->left);//проверка левых поддеревьев
        bool isRightIsomorphic = isIsomorphic(root1->right, root2->right);//проверка правых поддеревьев
        if (isLeftIsomorphic && isRightIsomorphic) {//если оба поддерева изоморфны, то деревья изоморфны
            return true;
        }
        bool isFlippedIsomorphic = isIsomorphic(root1->left, root2->right) && isIsomorphic(root1->right, root2->left);//проверка на изоморфность, если поменять местами левое и правое поддеревья
            return isFlippedIsomorphic;
        }

    Node* copyTree(Node* root) {//копирование дерева для слияния
        if (root == nullptr) {//если дерево пустое, то выходим
            return nullptr;
        }
        Node* copyRoot = new Node{root->value, nullptr, nullptr};//создание копии узла
        copyRoot->left = copyTree(root->left);//копирование левого поддерева
        copyRoot->right = copyTree(root->right);//копирование правого поддерева
        return copyRoot;
    }
    void print(Node *root){//вывод бинарного дерева
        if (!root) return;
        tabs++;

        print(root->left);
        
        for (int i = 0; i < tabs; i++){
            cout<<" | ";
        }
        cout << root->value <<endl;
        
        print(root->right);
        tabs--;
        return;
    }
    void mergeTrees(Node* tree1, Node* tree2) {// слияние двух деревьев в одно, если они изоморфны, а если нет, то просто удаляем всё ненужное и делаем изоморфное дерево
        if (tree1 == nullptr && tree2 == nullptr) {//если оба дерева пустые, то выходим
            return;
        }
        if (tree1 == nullptr) {//если первое дерево пустое, то копируем второе
            tree1 = copyTree(tree2);
            return;
        }
        if (tree2 == nullptr) {//если второе дерево пустое, то выходим
            return;
        }
        if (tree1->value == tree2->value) {//если значения узлов равны, то сливаем их
            mergeTrees(tree1->left, tree2->left);
            mergeTrees(tree1->right, tree2->right);
        } else {//если значения узлов не равны, то удаляем ненужные деревья и копируем второе дерево
        mergeTrees(tree1, tree2->left);
        mergeTrees(tree1, tree2->right);
        }
    }
    //алгоритм всех перестановок из n чисел
    void print_array(int arr[], int n) {
        for (int i = 0; i < n; i++) {
            cout << arr[i] << " ";
        }
        cout << endl;
    }

        // Функция генерации перестановок
    void generate_permutations(int arr[], int n) {
        // Сортируем исходный массив
        sort(arr, arr + n);
        while (true) {
            print_array(arr, n);
            int i = n - 2;// Ищем индекс первого элемента, который можно заменить
            while (i >= 0 && arr[i] >= arr[i+1]) {
                i--;
            }
            // Если такого элемента нет, завершаем цикл
            if (i < 0) {
                break;
            }
            // Ищем индекс первого элемента справа от arr[i], который меньше arr[i]
            int j = n - 1;
            while (arr[i] >= arr[j]) {
                j--;
            }
            // Меняем местами arr[i] и arr[j]
            swap(arr[i], arr[j]);
            // Переворачиваем массив справа от arr[i]
            reverse(arr + i + 1, arr + n);
        }
    }
    
    
    vector<vector<int>> generate_permutations_from_N_to_M(int n, int m){

        vector<vector<int>> res;
        vector<int> perm(n);// Создаем вектор для хранения текущей перестановки
        for (int i = 0; i < n; i++) {// Заполняем его начальной перестановкой [0,1,2,...,n-1]
            perm[i] = i;
        }
        int count_perm = 0;
        // Генерируем перестановки до тех пор, пока не будут сгенерированы все перестановки
        do {
            int count = 0;
            for (int i = 0; i < n; i++) {// Считаем количество элементов на своих местах
                if (perm[i] == i) {
                    count++;
                }
            }
            if (count == m) {// Если количество элементов на своих местах равно m, добавляем перестановку в результирующий вектор
                res.push_back(perm);
                count_perm+=1;
            }
        } while (next_permutation(perm.begin(), perm.end())); // Генерируем следующую перестановку с помощью функции next_permutation
        // Выводим количество перестановок, в которых m элементов на своих местах
        cout<<"количесвто перестановок из "<<n<<" элементов в которых " <<m<<" элементов на своём месте: "<<count_perm<<endl;
        // Возвращаем вектор с перестановками
        return res;
    }
    

    
    vector<pair<int, int>> find_shortest_path(vector<vector<int>>& maze, pair<int, int>& start, pair<int, int>& end) {

        
        // Определяем размеры поля
        int rows = maze.size();
        int cols = maze[0].size();

        // Проверяем, что начальная и конечная точки не являются препятствиями
        if (maze[start.first][start.second] == 1 || maze[end.first][end.second] == 1) {
            // Если начальная или конечная точки являются препятствиями, возвращаем пустой путь и выводим сообщение об ошибке
            cout<<" начальная или конечная точки являются препятствиями"<<endl;
            return vector<pair<int, int>>();
        }

        // Создаем очередь для BFS и множество обработанных вершин
        queue<pair<pair<int, int>, vector<pair<int, int>>>> q;
        set<pair<int, int>> processed;// для хранения координат уже обработанных вершин в лабиринте
        vector<pair<int, int>> startPath = { start };
        q.push(make_pair(start, startPath));
        processed.insert(start);//добавляем в кратчайший путь начальную координату

        while (!q.empty()) {// Начинаем цикл поиска

            auto vertex = q.front().first;// Извлекаем вершину из очереди и получаем путь до нее
            auto path = q.front().second;
            q.pop();
            int i = vertex.first;
            int j = vertex.second;

            
            if (vertex == end) {// Проверяем, является ли текущая вершина конечной
                return path;// Если текущая вершина является конечной, возвращаем найденный путь
            }

            
            for (auto dir : vector<pair<int, int>>{ {-1, 0}, {1, 0}, {0, -1}, {0, 1} }) {
                int x = i + dir.first;
                int y = j + dir.second;

                // Проверяем, что соседняя вершина не является препятствием и не была обработана ранее добавляем её в очередь и помечаем как обработанную
                if (x >= 0 && x < rows && y >= 0 && y < cols && maze[x][y] != 1 && processed.find(make_pair(x, y)) == processed.end()) {

                    vector<pair<int, int>> newPath = path;// Создание нового пути newPath на основе существующего path
                    newPath.push_back(make_pair(x, y));// Добавление новой координаты (x, y) в конец пути newPath
                    q.push(make_pair(make_pair(x, y), newPath));// Добавление в очередь новой пары координат (x, y) и нового пути newPath, начиная от вершины (x, y)
                    processed.insert(make_pair(x, y));// Добавление координаты (x, y) в множество processed, которое хранит уже обработанные вершины
                }
            }
        }

        cout<<"Путь до конечной точки не найден"<<endl;
        return vector<pair<int, int>>();
    }


    void print_traveling_salesman(int arr[], vector<vector<int>> matrix, int start) {
        int size_path = 0;
        vector<int> size;//вектор содержащий стоимость от точки до точки
        cout << "path: ";
        for(int j = 0;j<matrix.size(); j++){
            if(j != matrix.size() - 1){//идём от начала массива arr до предпоследнего элемента
                size_path += matrix[ arr[j]-1 ][ arr[j+1]-1 ];//с помощью точки и следующей ей вычисляем общее расстояние
                size.push_back(matrix[ arr[j]-1 ][ arr[j+1]-1 ]);
            }
            else{//возвращение в стартовую точку
                size_path += matrix[ arr[j]-1 ][ arr[0] - 1];
                size.push_back(matrix[ arr[j]-1 ][ arr[0] - 1 ]);
            }
            cout << arr[j] <<" ";
        }
        cout<<start + 1;
        cout<<"   size_path: "<<size_path;
        cout<<"   price: ";
        for(auto elem: size){
            cout<<elem<<" ";
        }cout<<endl;
        
    }

    

        // Функция генерации перестановок
    void traveling_salesman(int arr[], int n, vector<vector<int>> matrix,int start,int k = 0) {//перестановки для комивояжёра
        int temp = arr[start];//ставим элемент под индекcом start, тоесть элемент start+1, в начало массива, удаляя его из своей позиции
        for (int i = start; i > 0; i--) {
            arr[i] = arr[i-1];
        }
        arr[0] = temp;
        while (true) {//генерация перестановок
            if(arr[0] == start + 1){//если стартовая точка стоит в начале перестановок, то переставляем следующие 5 элементов

                print_traveling_salesman(arr, matrix,start);//вызов поиска всех путей проходя каждую точки вурнувшись обратно в стартовую

                int i = n - 2;// Ищем индекс первого элемента, который можно заменить
                while (i >= 0 && arr[i] >= arr[i+1]) {
                    i--;
                }
                // Если такого элемента нет, завершаем цикл
                if (i < 0) {
                    break;
                }
                // Ищем индекс первого элемента справа от arr[i], который меньше arr[i]
                int j = n - 1;
                while (arr[i] >= arr[j]) {
                    j--;
                }
                // Меняем местами arr[i] и arr[j]
                swap(arr[i], arr[j]);
                // Переворачиваем массив справа от arr[i]
                reverse(arr + i + 1, arr + n);
            }
            else break;// чтобы не делать лишние расчеты перестановок, когда стартовая точка не в начале
        }
    }        
};
