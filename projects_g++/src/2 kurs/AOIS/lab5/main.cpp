#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

std::vector<int> data; // Общий вектор данных
std::mutex dataMutex; // Мьютекс для синхронизации доступа к данным
std::condition_variable dataReady; // Условная переменная для сигнализации о готовности данных

bool isDataReady = false; // Флаг, указывающий на готовность данных

void readDataFromFile()
{
    std::ifstream file("./src/AOIS/lab5/1.txt");
    if (!file)
    {
        std::cout << "Ошибка при открытии файла." << std::endl;
        return;
    }

    int num;
    while (file >> num)
    {
        std::lock_guard<std::mutex> lock(dataMutex);
        data.push_back(num);
    }

    file.close();
    { // Сигнализируем о готовности данных
        std::lock_guard<std::mutex> lock(dataMutex);
        isDataReady = true;
    } 
    dataReady.notify_all();
}

void printOddNumbers()
{
    { // Ожидаем готовность данных
        std::unique_lock<std::mutex> lock(dataMutex);
        dataReady.wait(lock, [] { return isDataReady; });
    }

    {
        std::lock_guard<std::mutex> lock(dataMutex);
        std::cout << "Нечетные числа: ";
        for (int num : data)
        {
            if (num % 2 != 0)
                std::cout << num << " ";
        }
        std::cout << std::endl;
    }
}

void calculateStatistics()
{
    {
        std::unique_lock<std::mutex> lock(dataMutex);
        dataReady.wait(lock, [] { return isDataReady; });
    } // Ожидаем готовность данных

    {
        std::lock_guard<std::mutex> lock(dataMutex);
        if (data.empty())
        {
            std::cout << "Нет данных для вычислений." << std::endl;
            return;
        }

        int sum = 0;
        int max = data[0];
        int min = data[0];
        for (int num : data)
        {
            sum += num;
            if (num > max)
                max = num;
            if (num < min)
                min = num;
        } double average = static_cast<double>(sum) / data.size();

        std::cout << "Среднее значение: " << average << std::endl;
        std::cout << "Максимальное значение: " << max << std::endl;
        std::cout << "Минимальное значение: " << min << std::endl;
    }
}
int main()
{
    std::thread t1(readDataFromFile);
    std::thread t2(printOddNumbers);
    std::thread t3(calculateStatistics);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}


