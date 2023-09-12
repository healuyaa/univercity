#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

struct Point {
    double x1, x2;
};

// Пороговая функция активации
int thresholdActivation(double sum, double threshold) {
    return sum >= threshold ? 1 : 0;
}

// Функция обучения нейрона
void trainPerceptron(std::vector<Point>& input, std::vector<int>& e, double& w1, double& w2, double& threshold) {
    bool converged = false;
    int maxEpochs = 1000; // Максимальное число эпох обучения
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

        std::cout << "Epoch " << epoch << ": w1 = " << w1 << ", w2 = " << w2 << ", threshold = " << threshold << std::endl;
    }

    if (converged) {
        std::cout << "Converged after " << epoch << " epochs." << std::endl;
    } else {
        std::cout << "Training did not converge within " << maxEpochs << " epochs." << std::endl;
    }
}

int main() {
    // Входные данные
    std::vector<Point> input = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};

    // Эталонные значения
    std::vector<int> e = {0, 0, 0, 1};

    // Инициализируем веса и порог случайными значениями
    double w1 = static_cast<double>(rand()) / RAND_MAX;
    double w2 = static_cast<double>(rand()) / RAND_MAX;
    double threshold = static_cast<double>(rand()) / RAND_MAX;

    // Обучаем нейронную сеть
    trainPerceptron(input, e, w1, w2, threshold);

    return 0;
}
