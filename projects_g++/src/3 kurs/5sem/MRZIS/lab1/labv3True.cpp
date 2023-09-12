#include <iostream>
#include <vector>

struct Point {
    double x1, x2;
};

// Пороговая функция активации
int thresholdActivation(double sum, double threshold) {
    return sum >= threshold ? 1 : 0;
}

int main() {
    // Входные данные
    std::vector<Point> input = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};

    // Эталонные значения
    std::vector<int> e = {0, 0, 0, 1};

    // Параметры нейрона
    double w1 = 1.0; // Вес 1
    double w2 = 1.0; // Вес 2
    double threshold = 0.9; // Порог

    // Обучаем нейронную сеть
    bool converged = false;
    int maxEpochs = 6; // Максимальное число эпох обучения
    int epoch = 0;

    while (!converged && epoch < maxEpochs) {
        converged = true;
        epoch++;

        for (size_t i = 0; i < input.size(); ++i) {
            double sum = w1 * input[i].x1 + w2 * input[i].x2 + threshold;
            int output = thresholdActivation(sum, threshold);

            if (output != e[i]) {
                // Обновляем веса и порог
                w1 += (e[i] - output) * input[i].x1;
                w2 += (e[i] - output) * input[i].x2;
                threshold += e[i] - output;
                converged = false;
            }
        }
    }

    if (converged) {
        std::cout << "Converged after " << epoch << " epochs." << std::endl;
    } else {
        std::cout << "Training did not converge within " << maxEpochs << " epochs." << std::endl;
    }

    // Выводим обученные параметры
    std::cout << "Trained weights: w1 = " << w1 << ", w2 = " << w2 << ", threshold = " << threshold << std::endl;

    // Тестируем нейронную сеть
    std::cout << "Testing..." << std::endl;
    bool allCorrect = true;
    for (size_t i = 0; i < input.size(); ++i) {
        double sum = w1 * input[i].x1 + w2 * input[i].x2 + threshold;
        int output = thresholdActivation(sum, threshold);

        std::cout << "Input: (" << input[i].x1 << ", " << input[i].x2 << "), Output: " << output << ", Expected: " << e[i] << std::endl;

        if (output != e[i]) {
            allCorrect = false;
        }
    }

    if (allCorrect) {
        std::cout << "All test cases are correctly classified. Stopping training." << std::endl;
    }

    return 0;
}
