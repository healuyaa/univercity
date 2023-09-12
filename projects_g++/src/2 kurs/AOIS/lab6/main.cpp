#include <iostream>
#include <vector>
#include <string>

struct Data
{
    int key;
    std::string value;
};

class AssociativeMemory
{
private:
    std::vector<Data> memory;

public:
    void insertData(int key, const std::string& value)
    {
        memory.push_back({ key, value });
    }

    void printMemory()
    {
        std::cout << "Ассоциативная память:\n";
        for (const auto& data : memory)
        {
            std::cout << "Ключ: " << data.key << ", Значение: " << data.value << "\n";
        }
        std::cout << std::endl;
    }

    void compareByValue(const std::string& value)
    {
        std::cout << "Результаты сравнения со значением \"" << value << "\":\n";
        for (const auto& data : memory)
        {
            if (data.value == value)
            {
                std::cout << "Ключ: " << data.key << ", Значение: " << data.value << "\n";
            }
        }
        std::cout << std::endl;
    }
};

int main()
{
    AssociativeMemory memory;

    // Вставка данных в ассоциативную память
    memory.insertData(1, "Значение 1");
    memory.insertData(2, "Значение 2");
    memory.insertData(3, "Значение 1");
    memory.insertData(4, "Значение 3");
    memory.insertData(5, "Значение 1");

    // Вывод данных из ассоциативной памяти
    memory.printMemory();

    // Сравнение со значением "Значение 1"
    memory.compareByValue("Значение 1");

    return 0;
}
