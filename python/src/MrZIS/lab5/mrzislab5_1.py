import time
from matplotlib import pyplot as plt
import numpy as np
from scipy.special import expit

E_arr_arr = []
f = lambda x: 0.1 * np.cos(0.1 * x) + 0.05 * np.sin(0.1 * x)

def normalize_data(data, min_val, max_val):
    min_data, max_data = np.min(data), np.max(data)
    normalized_data = (data - min_data) / (max_data - min_data) * (max_val - min_val) + min_val
    return normalized_data

class Network:
    def __init__(self, input_size, hidden_size, output_size, learning_rate=0.1):
        self.input_size, self.hidden_size, self.output_size = input_size, hidden_size, output_size
        self.learning_rate, self.start_learn_rate = learning_rate, learning_rate

        self.weights_input_hidden = np.random.randn(input_size, hidden_size)
        self.bias_hidden = np.random.randn(1, hidden_size)
        self.weights_hidden_output = np.random.randn(hidden_size, output_size)
        self.bias_output = np.random.randn(1, output_size)

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
        result = output[:, 0]  # Оптимизация получения первого столбца
        return normalize_data(result, -0.13446, 0.13446)

    def backward(self, inputs, target, output):
        error = target - output
        self.E = np.sum(error ** 2) / 2
        delta_hidden = error.dot(self.weights_hidden_output.T) * self.sigmoid_derivative(self.hidden_output)

        self.weights_hidden_output += self.hidden_output.T.dot(error.reshape(-1, 1)) * self.learning_rate
        self.bias_output += np.sum(error, axis=0, keepdims=True) * self.learning_rate
        self.weights_input_hidden += inputs.T.dot(delta_hidden) * self.learning_rate
        self.bias_hidden += np.sum(delta_hidden, axis=0, keepdims=True) * self.learning_rate

    def backward_adapt(self, inputs, targets, outputs):
        error = targets - outputs
        mse_batch = np.mean(error)
        inputs_cut = np.sum(error.reshape(-1, 1, 1) * inputs, axis=0) / len(error)
        delta_hidden = np.mean(error.dot(self.weights_hidden_output.T), axis=0) * self.sigmoid_derivative(self.hidden_output)

        self.weights_hidden_output += self.hidden_output.T.dot(error) * self.learning_rate
        self.bias_output += np.sum(error, axis=0, keepdims=True) * self.learning_rate
        self.weights_input_hidden += inputs_cut.T.dot(delta_hidden.reshape(1, -1)) * self.learning_rate
        self.bias_hidden += np.sum(delta_hidden, axis=0, keepdims=True) * self.learning_rate

        if is_adapt:
            self.learning_rate = np.clip(
                (4 * np.sum(delta_hidden * self.hidden_output * (1 - self.hidden_output))) /
                ((1 + np.sum(self.hidden_output ** 2)) * np.sum(
                    delta_hidden * self.hidden_output * (1 - self.hidden_output) ** 2)),
                0.0, 1.0
            )

            if np.isnan(self.learning_rate):
                self.learning_rate = 0.4

    def train(self, inputs, targets, epochs: int, is_adapt: bool = False):
        global E_arr
        for epoch in range(epochs):
            e_arr = []
            for i in range(len(inputs)):
                input_data, target_data = np.array([inputs[i]]), np.array([targets[i]])
                output = self.forward(input_data)
                e_arr.append(target_data - output)
                self.backward(input_data, target_data, output)
            e_arr = np.array(e_arr)
            E2 = np.sum(e_arr ** 2) / 2
            E_arr.append(E2)

            print(f"Online: Epoch: {epoch} MSE: {E2}")

    def train_adapt(self, inputs, targets, epochs: int, batchsize: int):
        global E_arr
        if len(inputs) % batchsize != 0:
            raise ValueError("Batch size should evenly divide the number of training samples.")

        inputspack = [inputs[i - batchsize:i] for i in range(batchsize, len(inputs) + 1, batchsize)]
        targetspack = [targets[i - batchsize:i] for i in range(batchsize, len(targets) + 1, batchsize)]

        for epoch in range(epochs):
            e_arr = []
            for i in range(len(inputspack)):
                outputs = self.forward(inputspack[i])
                e_arr.append(np.sum(targetspack[i] - outputs))
                self.backward_adapt(inputspack[i], targetspack[i], outputs)

            E2 = np.sum(np.array(e_arr) ** 2) / 2
            E_arr.append(E2)

            print(f"Batch: Epoch: {epoch} MSE: {E2}")

def get_train_data(all_points, input_size):
    result_X = [all_points[i:i + input_size] for i in range(len(all_points) - input_size)]
    result_Y = [all_points[i] for i in range(input_size, len(all_points), 1)]
    return np.array(result_X), np.array(result_Y)

input_size, hidden_size, output_size, epochs = 6, 2, 1, 100
all_train_points = f(np.arange(100, 200, 0.2))
all_test_points = f(np.arange(180, 280, 0.2))
X_train, Y_train = get_train_data(all_train_points, input_size)
X_test, Y_test = get_train_data(all_test_points, input_size)

def cut_ver(E_arr, epochs, expected_train_points, predicted, start, end):
    print(f"Time taken: {(end - start) * 10 ** 3:.03f}ms")
    print(min(E_arr))

    predicted = normalize_data(predicted, np.min(expected_train_points), np.max(expected_train_points))

    assert len(range(0, epochs)) == len(E_arr), "Dimensions mismatch"

    plt.plot(range(epochs), E_arr)
    plt.xlabel('Epochs')
    plt.ylabel('Error')
    plt.title('Error vs. Epochs')
    plt.show()

    plt.plot(np.arange(100, 200, 0.2), expected_train_points)
    plt.plot(np.arange(181.6, 280, 0.2), predicted[:492], ":")
    plt.show()

# Online-Const
E_arr = []
NN = Network(input_size, hidden_size, output_size)
start = time.time()
NN.train(X_train, Y_train, epochs, False)
E_arr_arr.append(E_arr)
end = time.time()
predicted = NN.predict(X_test)
cut_ver(E_arr_arr[0], epochs, all_train_points, predicted, start, end)

# Online-Adapt
E_arr = []
NN2 = Network(input_size, hidden_size, output_size)
start = time.time()
NN2.train(X_train, Y_train, epochs, True)
E_arr_arr.append(E_arr)
end = time.time()
predicted = NN2.predict(X_test)
cut_ver(E_arr_arr[1], epochs, all_train_points, predicted, start, end)
