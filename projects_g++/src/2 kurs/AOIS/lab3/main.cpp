#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

class HopfieldNet {
private:
    vector<vector<double>> weights;
    int n;
    // функция активации
    double sigmoid(double x) {
        return 1 / (1 + exp(-x));
    }

public:
    HopfieldNet(int n) {
        this->n = n;
        weights.resize(n, vector<double>(n, 0));//инициализ. матрицы весов
    }

    void learn(vector<double>& pattern) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j) {
                    weights[i][j] = 0;
                }
                else {
                    weights[i][j] += pattern[i] * pattern[j];                    
                }
                weights[i][j] = weights[i][j]/pattern.size();
            }
        }
    }
    // синхронный метод режима работы сети
    vector<double> recall_sync(vector<double>& pattern) {
        vector<double> y(n);
        vector<double> sum(n);

        int iter = 0;
        bool converged = false;
        while (iter < 100 && !converged) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    sum[i] += weights[i][j] * pattern[j];
                }
                y[i] = sigmoid(sum[i]);
            }
            bool converged = true;
            for (int i = 0; i < n; i++) {
                if (round(y[i]) != pattern[i]) {
                    converged = false;
                    break;
                }
            }
            iter++;
        }
        return y;
    }

    // асинхронный
    vector<double> recall_async(vector<double>& pattern) {
        vector<double> y(pattern);
        vector<double> prev_y;
        vector<int> order(n);
        for (int i = 0; i < n; i++) {
            order[i] = i;
        }
        random_shuffle(order.begin(), order.end());

        int iter = 0;
        bool converged = false;
        while (iter < 100 && !converged) {
            prev_y = y;
            for (int i : order) {
                double sum = 0;
                for (int j = 0; j < n; j++) {
                    sum += weights[i][j] * y[j];
                }
                double new_val = sigmoid(sum);
                y[i] = new_val;
            }
            converged = true;
            for (int i = 0; i < n; i++) {
                if (round(y[i]) != round(prev_y[i])) {
                    converged = false;
                    break;
                }
            }
            iter++;
        }
        return y;
    }

    int dist(vector<double>& v1, vector<double>& v2) {
        int d = 0;
        for (int i = 0; i < v1.size(); i++) {
            if (v1[i] != v2[i]) {
                d++;
            }
        }
        return d;
    }

    void show(vector<double>& pattern, vector<double>& distorted_pattern) {
        cout << "Исходное значение: " << endl;
        for (double val : pattern) {
            cout << val << " ";
        }
        cout << endl;
        cout << dist(pattern, distorted_pattern) << endl;
    }
};

void res_show(vector<double>& recovered_pattern, vector<double>& recovered_pattern_2) {
    cout << "Результат (синхр. метод): " << endl;
    for (double val : recovered_pattern) {
        val = static_cast<int>(val + 0.5 - numeric_limits<double>::epsilon()); // округление
        cout << val << " ";
    }
    cout << endl;
    cout << "Результат (асинхр. метод): " << endl;
    for (double val : recovered_pattern_2) {
        val = static_cast<int>(val + 0.5 - numeric_limits<double>::epsilon()); // округление
        cout << val << " ";
    }
    cout << endl;
}

int main() {
    vector<double> pattern = { 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0 };
    vector<double> pattern2 = { 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0 };
    vector<double> pattern3 = { 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1 };

    vector<double> distorted_pattern = { 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0 };
    cout << "Искажённое значение: " << endl;
    for (double val : distorted_pattern) {
        cout << val << " ";
    }
    cout << endl;

    HopfieldNet net1(pattern.size());
    net1.learn(pattern);
    vector<double> recovered_pattern_sync = net1.recall_sync(distorted_pattern);
    vector<double> recovered_pattern_async = net1.recall_async(distorted_pattern);
    net1.show(pattern, distorted_pattern);
    res_show(recovered_pattern_sync, recovered_pattern_async);

    HopfieldNet net2(pattern2.size());
    net2.learn(pattern2);
    vector<double> recovered_pattern_sync2 = net2.recall_sync(distorted_pattern);
    vector<double> recovered_pattern_async2 = net2.recall_async(distorted_pattern);
    net2.show(pattern2, distorted_pattern);
    res_show(recovered_pattern_sync2, recovered_pattern_async2);
    
    HopfieldNet net3(pattern3.size());
    net3.learn(pattern3);
    vector<double> recovered_pattern_sync3 = net3.recall_sync(distorted_pattern);
    vector<double> recovered_pattern_async3 = net3.recall_async(distorted_pattern);
    net3.show(pattern3, distorted_pattern);
    res_show(recovered_pattern_sync3, recovered_pattern_async3);
    return 0;
}
