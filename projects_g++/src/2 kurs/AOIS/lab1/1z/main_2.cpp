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
#define exitFromProgram 6

class row {
    public:
    std::string key = "{none}";
    std::string name = "{none}";
    std::string age = "{none}";
    bool mark = true;

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

void readFileToTable(row &T, row *table) { // чтение из файла данных для таблице 
    std::ifstream file("./src/AOIS/lab1/row.txt");
    if (!file)
    {
        std::cerr << "File read error";
    }

    std::string key, name, age;
    size_t i = 0;
    while(file >> key >> name >> age) {
        table[i].key = key;
        table[i].name = name;
        table[i].age = age;
        table[i].mark = false;
        i++;
    }    
    file.close();
}

int searchByKey(std::string strKey, row *table) { // поиск по ключу(перебором), возвращает номер ячейки в массиве
    for (size_t i = 0; i < sizeTable; i++)
    {
        if (table[i].key == strKey)
        {
            return i;
        }           
    }
    return -1;    
}

void addElementInTable(row &T, row *table) { // добавляет элемент в массив
    size_t i = 0;
    while(table[i].mark == false && (i < 32)) {
        i++;  
    }
    table[i].set_KEY(T.key);
    table[i].set_NAME(T.name);
    table[i].set_AGE(T.age);
    table[i].set_MARK(false);          
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

void menu(int inputNum, row *table, row Temp) {
    std::cout << "1. Viewing the table" << std::endl;
    std::cout << "2. Find table element" << std::endl;
    std::cout << "3. Adding an element" << std::endl;
    std::cout << "4. Table editing" << std::endl;
    std::cout << "5. Delete table element" << std::endl;
    std::cout << "6. Exiting the program" << std::endl;

    std::cin >> inputNum;
    std::string tempStr;
    int tempIter;
    int indexEdit;

    switch (inputNum) {
    case watchTable:
        showTable(table);
        system("Pause");
        break;

    case findElemnet:
        std::cout << "Enter search key" << std::endl;
        std::cin >> tempStr;
        tempIter = searchByKey(tempStr, table);

        if (tempIter == -1) {
            std::cout << "You entered the wrong key";
        } else {
            std::cout << setiosflags(std::ios::right) << std::setw(2) << "ID";
            std::cout << std::setw(16) << "KEY";
            std::cout << std::setw(16) << "NAME";
            std::cout << std::setw(16) << "AGE" << std::endl;

            std::cout << setiosflags(std::ios::right) << std::setw(2) << tempIter;
            std::cout << '|' << std::setw(15) << table[tempIter].get_KEY();
            std::cout << '|' << std::setw(15) << table[tempIter].get_NAME();
            std::cout << '|' << std::setw(15) << table[tempIter].get_AGE();
            std::cout << '|' <<std::endl;
        }
        system("Pause");       
        break;

        case addElement:
        std::cout << "Enter key:" << std::endl;
        std::cin >> Temp.key;
        std::cout << "Enter name:" << std::endl;
        std::cin >> Temp.name;
        std::cout << "Enter age:" << std::endl;
        std::cin >> Temp.age;

        addElementInTable(Temp, table);
        showTable(table);

        system("Pause");
        break;

    case editingElement:
        showTable(table);
        std::cout << "Enter array index" << std::endl;
        std::cin >> indexEdit;
        std::cout << "Enter key:" << std::endl;
        std::cin >> Temp.key;
        std::cout << "Enter name:" << std::endl;
        std::cin >> Temp.name;
        std::cout << "Enter age:" << std::endl;
        std::cin >> Temp.age;

        editFieldElement(table, indexEdit, Temp);
        system("Pause");
        break;

    case deleteElement:
        showTable(table);

        std::cout << "Enter array index" << std::endl;
        std::cin >> tempIter;

        deleteElementInRow(table, tempIter);
        system("Pause"); 
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
        menu(inputNum, table, Temp);
    }
}

int main() {

    row *table = new row[sizeTable];
    size_t inputNum;
    row examp;
    readFileToTable(examp, table);
    menu(inputNum, table, examp);

    delete [] table;
}