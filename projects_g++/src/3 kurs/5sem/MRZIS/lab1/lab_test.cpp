#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <fstream>

class Perceptron {
private:
    std::vector<double> weights;
    double bias;

public:
    Perceptron(int input_size) {
        weights.resize(input_size);
        bias = (double)rand() / RAND_MAX;
        for (int i = 0; i < input_size; i++) {
            weights[i] = (double)rand() / RAND_MAX;
        }
    }

    int activation_function(double x) {
        return (x <= 0.5) ? 0 : 1;
    }

    int predict(const std::vector<int>& inputs) {
        double weighted_sum = 0;
        for (int i = 0; i < inputs.size(); i++) {
            weighted_sum += inputs[i] * weights[i];
        }
        weighted_sum += bias;
        return activation_function(weighted_sum);
    }

    void train(const std::vector<std::vector<int>>& training_inputs, const std::vector<int>& labels, double learning_rate, int epochs) {
        std::vector<double> errors;
        for (int epoch = 0; epoch < epochs; epoch++) {
            double total_error = 0;
            for (int i = 0; i < training_inputs.size(); i++) {
                int prediction = predict(training_inputs[i]);
                int error = labels[i] - prediction;
                total_error += error;
                for (int j = 0; j < weights.size(); j++) {
                    weights[j] += learning_rate * error * training_inputs[i][j];
                }
                bias += learning_rate * error;
            }
            errors.push_back(total_error);
        }
        std::cout << "Epochs: ";
        for (int i{0}; i < epochs; ++i) {
            std::cout << i + 1 << " ";
        }
        std::cout << std::endl << "Errors: ";
        for (int i{0}; i < errors.size(); ++i) {
            std::cout << errors[i] << " ";
        }
        std::cout << std::endl;
    }

    void plot_decision_boundary(const std::vector<std::vector<int>>& training_inputs, const std::vector<int>& labels) {
        int x_min = training_inputs[0][0];
        int x_max = training_inputs[0][0];
        int y_min = labels[0];
        int y_max = labels[0];

        for (const auto& input : training_inputs) {
            x_min = std::min(x_min, input[0]);
            x_max = std::max(x_max, input[0]);
        }

        for (int label : labels) {
            y_min = std::min(y_min, label);
            y_max = std::max(y_max, label);
        }


        for (int y = y_max; y >= y_min; y--) {
            for (int x = x_min; x <= x_max; x++) {
                std::vector<int> point = {x, y};
                int prediction = predict(point);
                if (prediction == 0) {
                    std::cout << ".";
                } else {
                    std::cout << "#";
                }
            }
            std::cout << std::endl;
        }
    }

    void plot_errors(const std::vector<double>& errors) {
        std::ofstream errorFile("errors.txt");
        for (int i = 0; i < errors.size(); ++i) {
            errorFile << i + 1 << " " << errors[i] << std::endl;
        }
        errorFile.close();
        system("gnuplot -persist -e \"plot 'errors.txt' with lines title 'Errors'\"");
    }
};

int main() {
    std::vector<std::vector<int>> training_inputs = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    std::vector<int> labels = {0, 0, 0, 1};

    Perceptron perceptron(2);

    double learning_rate = 0.2;
    int epochs = 10;
    perceptron.train(training_inputs, labels, learning_rate, epochs);

    std::vector<std::vector<int>> test_inputs = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    for (const auto& inputs : test_inputs) {
        int prediction = perceptron.predict(inputs);
        std::cout << "Input: [" << inputs[0] << ", " << inputs[1] << "], Prediction: " << prediction << std::endl;
    }
    perceptron.plot_decision_boundary(training_inputs, labels);

    std::ofstream dataFile("data.txt");
    for (int i = 0; i < training_inputs.size(); ++i) {
        dataFile << training_inputs[i][0] << " " << training_inputs[i][1] << " " << labels[i] << std::endl;
    }
    dataFile.close();
    system("gnuplot -persist -e \"plot 'data.txt' using 1:2:3 with points palette title 'Training Data', 0.5 lt 1 lw 2 notitle\"");

    return 0;
}
