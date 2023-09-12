#include <iostream>
#include <string>
#include <windows.h>
#include <math.h>
#include <fstream>
#include <iomanip>
#include <conio.h>

#define sizeTable 32
#define watchTable 1
#define findElemnet 2
#define addElement 3
#define editingElement 4
#define deleteElement 5
#define statInformation 6 
#define exitFromProgram 7

class row {
    public:
    std::string key = "{none}";
    std::string name = "{none}";
    std::string age = "{none}";
    bool mark = true;

    unsigned int hash;
    unsigned int id;

    row() {
    }
    row(std::string name, std::string key, std::string age) {
        this->key = key;
        this->name = name;
        this->age = age;
    }
    row(row &T) {
        key = T.key;
        name = T.name;
        age = T.age;
    }

    void set_KEY(std::string key) {
        this->key = key;
    }
    void set_NAME(std::string name) {
        this->name = name;
    }
    void set_AGE(std::string age) {
        this->age = age;
    }
    void set_MARK(bool mark) {
        this->mark = mark;
    }
    void set_HASH(unsigned int hash) {
        this->hash = hash;
    }
    void set_ID(int id) {
        this->id = id;
    }

    std::string get_NAME() {
        return this->name;
    }
    std::string get_KEY() {
        return this->key;
    }
    std::string get_AGE() {
        return this->age;
    }
    bool get_MARK() {
        return this->mark;
    }
    unsigned char get_HASH() {
        return this->hash;
    }
    int get_ID() {
        return this->id;
    }
};

class InfAboutTable {
    public:
    int сollisionCounter = 0; // количество колизий
    int addTableCounter = 0; // количество записей в таблице
    int addTableFindCounter = 0; // количество обращений к таблице во время поиска места элемента

    double collisionChance() {
        return ((double)сollisionCounter / (double)addTableCounter);
    }

    double numberOfHits() {
        return ((double)addTableFindCounter / (double)addTableCounter);
    }
};

void showTable(row *table) { 
    std::cout << setiosflags(std::ios::right) << std::setw(2) << "ID";
    std::cout << std::setw(16) << "KEY";
    std::cout << std::setw(16) << "NAME";
    std::cout << std::setw(16) << "AGE";
    std::cout << std::endl;
    
    for (size_t i = 0; i < 32; i++)
    {
        std::cout << setiosflags(std::ios::right) << std::setw(2) << i;
        std::cout << '|' << std::setw(15) << table[i].get_KEY();
        std::cout << '|' << std::setw(15) << table[i].get_NAME();
        std::cout << '|' << std::setw(15) << table[i].get_AGE();
        std::cout << '|' << std::endl;
    }
}

unsigned int hash_XOR_shift_square(row &T) { // функция хэширования XOR со сдвигом + квадратичная
    T.hash = 0;
    int shift = 1;
    for(char c : T.key) {
        //T.hash ^= (c << shift) ^ (c >> shift);
        T.hash ^= (c << shift);
        T.hash *= T.hash; 
        T.hash = (T.hash >> 8);
    }
    T.id = T.hash % 32;
    return T.id;
}

void insertTableValues(row &T, row *table, InfAboutTable &K) { // вставка в таблицу данных по id, при условие, что оно не задано. Иначе следуем методу линейного зондирования, ищем свободное место
    T.id = hash_XOR_shift_square(T);
    K.addTableFindCounter++;
    if(table[T.id].mark == true) {
        table[T.id].key = T.key;
        table[T.id].name = T.name;
        table[T.id].age = T.age;
        table[T.id].mark = false;
        K.addTableCounter++;
    }
    else {
        K.сollisionCounter++;
        //std::cout << T.key << std::endl; 
        for (size_t i = T.id; i < sizeTable; i++)
        {
            K.addTableFindCounter++;
            if (table[i].mark == true) {
                table[i].key = T.key;
                table[i].name = T.name;
                table[i].age = T.age;
                table[i].mark = false;
                K.addTableCounter++;
                break;
            } else {
                continue;
            }            
        }        
    }
}

void readFileToTable(row &T, row *table, InfAboutTable &K) { // чтение из файла данных для таблице 
    std::ifstream file("./src/AOIS/lab1/row.txt");
    if (!file)
    {
        std::cerr << "File read error";
    }

    size_t i = 0;
    while(file >> T.key >> T.name >> T.age) {
        insertTableValues(T, table, K);
    }    
    file.close();
}

int searchByKey(std::string searchKey, row *table) { // поиск по ключу(перебором), возвращает номер ячейки в массиве
    for (size_t i = 0; i < sizeTable; i++)
    {
        if (table[i].key == searchKey)
        {
            return i;
        }           
    }
    return -1;    
}

void deleteElementInRow(row *table, int indexDel) {
    table[indexDel].set_KEY("{none}");
    table[indexDel].set_NAME("{none}");
    table[indexDel].set_AGE("{none}");
    table[indexDel].set_MARK(true);
}

void editFieldElement(row *table, int indexEdit, row &T) {
    table[indexEdit].set_KEY(T.key);
    table[indexEdit].set_NAME(T.name);
    table[indexEdit].set_AGE(T.age);
}

void menu(int inputNum, row *table, row &T, InfAboutTable &K) {
    std::cout << "1. Viewing the table" << std::endl;
    std::cout << "2. Find table element" << std::endl;
    std::cout << "3. Adding an element" << std::endl;
    std::cout << "4. Table editing" << std::endl;
    std::cout << "5. Delete table element" << std::endl;
    std::cout << "6. Statistic information" << std::endl;
    std::cout << "7. Exiting the program" << std::endl;

    std::cin >> inputNum;
    std::string tempStr;
    int tempIndex;

    switch (inputNum) {
    case watchTable:
        showTable(table);
        system("Pause");
        break;

    case findElemnet:
        std::cout << "Enter search key" << std::endl;
        std::cin >> tempStr;
        tempIndex = searchByKey(tempStr, table);

        if (tempIndex == -1) {
            std::cout << "You entered the wrong key";
        } else {
            std::cout << setiosflags(std::ios::right) << std::setw(2) << "ID";
            std::cout << std::setw(16) << "KEY";
            std::cout << std::setw(16) << "NAME";
            std::cout << std::setw(16) << "AGE" << std::endl;

            std::cout << setiosflags(std::ios::right) << std::setw(2) << tempIndex;
            std::cout << '|' << std::setw(15) << table[tempIndex].get_KEY();
            std::cout << '|' << std::setw(15) << table[tempIndex].get_NAME();
            std::cout << '|' << std::setw(15) << table[tempIndex].get_AGE();
            std::cout << '|' <<std::endl;
        }
        system("Pause");       
        break;

        case addElement:
        std::cout << "Enter key:" << std::endl;
        std::cin >> T.key;
        std::cout << "Enter name:" << std::endl;
        std::cin >> T.name;
        std::cout << "Enter age:" << std::endl;
        std::cin >> T.age;

        insertTableValues(T, table, K);
        showTable(table);

        system("Pause");
        break;

    case editingElement:
        showTable(table);
        std::cout << "Enter array index" << std::endl;
        std::cin >> tempIndex;
        std::cout << "Enter key:" << std::endl;
        std::cin >> T.key;
        std::cout << "Enter name:" << std::endl;
        std::cin >> T.name;
        std::cout << "Enter age:" << std::endl;
        std::cin >> T.age;

        editFieldElement(table, tempIndex, T);
        system("Pause");
        break;

    case deleteElement:
        showTable(table);

        std::cout << "Enter array index" << std::endl;
        std::cin >> tempIndex;

        deleteElementInRow(table, tempIndex);
        system("Pause"); 
        break;

    case statInformation:
        std::cout << "Number of records in the table: " << K.addTableCounter << std::endl;
        std::cout << "Number of collisions: " << K.сollisionCounter << std::endl;
        std::cout << "Number of table accesses: " << K.addTableFindCounter << std::endl;
        std::cout << "Chance of a collision: " << K.collisionChance() << " / " << (K.collisionChance() * 100.0) << "%" << std::endl;
        std::cout << "Number of table accesses: " << K.numberOfHits() << std::endl;
        break;    

    default:
        if (inputNum != exitFromProgram)
        {
            std::cout << "Check if the entered value is correct" << std::endl;
        }
        break;
    }
    if (inputNum == exitFromProgram)
    {
        std::cout << ":)";
    } else {
        menu(inputNum, table, T, K);
    }
}

int main() {

    row *table = new row[sizeTable];
    size_t inputNum;
    row examp;
    InfAboutTable stat;
    readFileToTable(examp, table, stat);
    menu(inputNum, table, examp, stat);

    delete [] table;
}