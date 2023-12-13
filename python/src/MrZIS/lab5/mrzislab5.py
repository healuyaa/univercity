import time
from matplotlib import pyplot as plt
import numpy as np
from scipy.special import expit

E_arr_arr = []
E_arr = []
f = lambda x: 0.1 * np.cos(0.1 * x) + 0.05 * np.sin(0.1 * x)  # Updated function

def normalize_data(data, min_val, max_val):
    min_data = np.min(data)
    max_data = np.max(data)
    normalized_data = (data - min_data) / (max_data - min_data) * (max_val - min_val) + min_val
    return normalized_data

class Network:
    def __init__(self, input_size, hidden_size, output_size, learning_rate=0.1):
        self.input_size = input_size
        self.hidden_size = hidden_size
        self.output_size = output_size
        self.learning_rate = learning_rate
        self.start_learn_rate = learning_rate

        self.weights_input_hidden = np.random.randn(self.input_size, self.hidden_size)
        self.bias_hidden = np.random.randn(1, self.hidden_size)
        self.weights_hidden_output = np.random.randn(self.hidden_size, self.output_size)
        self.bias_output = np.random.randn(1, self.output_size)

    def sigmoid(self, x):
        return expit(x)

    def sigmoid_derivative(self, x):
        return x * (1 - x)

    def forward(self, inputs):
        self.hidden_input = np.dot(inputs, self.weights_input_hidden) + self.bias_hidden
        self.hidden_output = self.sigmoid(self.hidden_input)
        self.output = np.dot(self.hidden_output, self.weights_hidden_output) + self.bias_output
        return self.output

    def predict(self, inputs):
        output = self.forward(inputs)
        result = [el[0] for el in output]
        return normalize_data(result, -0.13446, 0.13446)

    def backward(self, inputs, target, output):
        error = target - output
        delta_hidden = error.dot(self.weights_hidden_output.T) * self.sigmoid_derivative(self.hidden_output)

        self.weights_hidden_output += self.hidden_output.T.dot(error) * self.learning_rate
        self.bias_output += np.sum(error, axis=0, keepdims=True) * self.learning_rate
        self.weights_input_hidden += inputs.T.dot(delta_hidden) * self.learning_rate
        self.bias_hidden += np.sum(delta_hidden, axis=0, keepdims=True) * self.learning_rate

    def backwardBatch(self, inputs, targets, outputs):
        error = targets - outputs
        mse_batch = np.sum(error) / len(error)
        inputs_cut = 0
        for j in range(len(error)):
            inputs_cut += error[j] * inputs[j]
        mse_batch = np.array(mse_batch)
        inputs_cut = np.array(inputs_cut).reshape(1, -1)
        delta_hidden = mse_batch.dot(self.weights_hidden_output.T) * self.sigmoid_derivative(self.hidden_output)
        self.weights_hidden_output += self.hidden_output.T.dot(mse_batch) * self.learning_rate
        self.bias_output += np.sum(mse_batch, axis=0, keepdims=True) * self.learning_rate
        self.weights_input_hidden += inputs_cut.T.dot(delta_hidden) * self.learning_rate
        self.bias_hidden += np.sum(delta_hidden, axis=0, keepdims=True) * self.learning_rate

    def train(self, inputs, targets, epochs: int, isAdapt: bool = False):
        global E_arr
        for epoch in range(epochs):
            e_arr = []
            for i in range(len(inputs)):
                input_data = np.array([inputs[i]])
                target_data = np.array([targets[i]])
                output = self.forward(input_data)
                e_arr.append(targets[i] - output)
                self.backward(input_data, target_data, output)
            E2 = np.sum(np.array(e_arr) ** 2) / 2
            E_arr.append(E2)
            self.learning_rate = min(0.2, max(self.learning_rate * E2, 0.0046)) if isAdapt else self.learning_rate
            print(f"Online: Epoch: {epoch} MSE: {E2}")

    def trainBatch(self, inputs, targets, epochs: int, batchsize: int, isAdapt: bool = False):
        global E_arr
        if (len(inputs) % batchsize != 0):
            return ValueError
        inputspack = [inputs[i - batchsize:i] for i in range(batchsize, len(inputs), batchsize)]
        targetspack = [targets[i - batchsize:i] for i in range(batchsize, len(targets), batchsize)]
        for epoch in range(epochs):
            e_arr = []
            for i in range(len(inputspack)):
                outputs = [self.forward(batchElem).item() for batchElem in inputspack[i]]
                e_arr.append(np.mean(targetspack[i] - outputs))
                self.backwardBatch(inputspack[i], targetspack[i], outputs)
            E2 = np.sum(np.array(e_arr) ** 2) / 2
            E_arr.append(E2)
            self.learning_rate = min(0.2, max(self.learning_rate * E2, 0.0046)) if isAdapt else self.learning_rate
            print(f"Batch: Epoch: {epoch} MSE: {E2}")

def get_train_data(all_points, input_size):
        result_X = [all_points[i:i + input_size] for i in range(len(all_points) - input_size)]
        result_Y = [all_points[i] for i in range(input_size, len(all_points), 1)]
        return np.array(result_X), np.array(result_Y)

# Example usage
input_size = 6
hidden_size = 2
output_size = 1
epochs = 100

# Генерация ожидаемых значений для обучающего и тестового наборов данных
all_train_points = f(np.arange(100, 200, 0.2))
all_test_points = f(np.arange(180, 280, 0.2))

# Создание обучающего и тестового наборов данных
X_train, Y_train = get_train_data(all_train_points, input_size)
X_test, Y_test = get_train_data(all_test_points, input_size)

def cutVer(E_arr, epochs, expected_train_points, predicted, start, end):
    print(f"Time taken: {(end - start) * 10 ** 3:.03f}ms")
    print(min(E_arr))

    # Inverse normalize the predicted values
    predicted = normalize_data(predicted, np.min(expected_train_points), np.max(expected_train_points))

    # Проверка размерностей
    assert len(range(0, epochs)) == len(E_arr), "Dimensions mismatch"

    # Построение графика функции ошибки
    plt.plot(range(epochs), E_arr)
    plt.xlabel('Epochs')
    plt.ylabel('Error')
    plt.title('Error vs. Epochs')
    plt.show()

    # Другие графики
    plt.plot(np.arange(100, 200, 0.2), expected_train_points)
    plt.plot(np.arange(180, 280, 0.2)[0:494], predicted, ":")
    plt.show()

# Online-Const
E_arr = []
NN = Network(input_size, hidden_size, output_size)
start = time.time()
NN.train(X_train, Y_train, epochs, False)
E_arr_arr.append(E_arr)
end = time.time()
predicted = NN.predict(X_test)
cutVer(E_arr_arr[0], epochs, all_train_points, predicted, start, end)

# Online-Adapt
E_arr = []
NN2 = Network(input_size, hidden_size, output_size)
start = time.time()
NN2.train(X_train, Y_train, epochs, True)
E_arr_arr.append(E_arr)
end = time.time()
predicted = NN2.predict(X_test)
cutVer(E_arr_arr[1], epochs, all_train_points, predicted, start, end)  # Use E_arr_arr[1]

# Batch-Const
E_arr = []
NN3 = Network(input_size, hidden_size, output_size)
start = time.time()
NN3.trainBatch(X_train, Y_train, epochs, 2, False)
E_arr_arr.append(E_arr)
end = time.time()
predicted = NN3.predict(X_test)
cutVer(E_arr_arr[2], epochs, all_train_points, predicted, start, end)  # Use E_arr_arr[2]

# Batch-Adapt
E_arr = []
NN4 = Network(input_size, hidden_size, output_size)
start = time.time()
NN4.trainBatch(X_train, Y_train, epochs, 2, True)
E_arr_arr.append(E_arr)
end = time.time()
predicted = NN4.predict(X_test)
cutVer(E_arr_arr[3], epochs, all_train_points, predicted, start, end)

print(E_arr_arr)

# Проверка на одинаковую длину списков
lengths = set(len(arr) for arr in E_arr_arr)
if len(lengths) == 1:
    # Все списки имеют одинаковую длину, поэтому можно строить графики
    plt.plot(range(epochs), E_arr_arr[0], "-")
    plt.plot(range(epochs), E_arr_arr[1], "--")
    plt.plot(range(epochs), E_arr_arr[2], "-.")
    plt.plot(range(epochs), E_arr_arr[3], ":")
    plt.legend(['Online-Const', 'Online-Adapt', 'Batch-Const', 'Batch-Adapt'], loc="upper right")
    plt.show()
else:
    print("Ошибка: Списки имеют разную длину.")

