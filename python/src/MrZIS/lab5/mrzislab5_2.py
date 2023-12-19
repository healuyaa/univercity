import numpy as np
from scipy.special import expit
import matplotlib.pyplot as plt
import time

f = lambda x: 0.1 * np.cos(0.1 * x) + 0.05 * np.sin(0.1 * x)

def normalize_data(data, min_val, max_val):
    min_data = np.min(data)
    max_data = np.max(data)
    normalized_data = (data - min_data) / (max_data - min_data) * (max_val - min_val) + min_val
    return normalized_data

class Network:
    def __init__(self, input_size, hidden_size, output_size, learning_rate=0.003):
        self.input_size = input_size
        self.hidden_size = hidden_size
        self.output_size = output_size
        self.learning_rate = learning_rate
        self.E = 0
        self.weights_input_hidden = np.random.randn(self.input_size, self.hidden_size)
        self.bias_hidden = np.zeros((1, self.hidden_size))
        self.weights_hidden_output = np.random.randn(self.hidden_size, self.output_size)
        self.bias_output = np.zeros((1, self.output_size))

    def sigmoid(self, x):
        return expit(x)

    def sigmoid_derivative(self, x):
        return x * (1 - x)

    def backward(self, inputs, target, output):
        for i in range(len(target)):
            error = target[i] - output[i]
            self.E += np.sum(error ** 2)
            delta_hidden = error.dot(self.weights_hidden_output.T) * self.sigmoid_derivative(
                np.array([self.hidden_output[i]]))

            self.weights_hidden_output += np.array([self.hidden_output[i]]).T.dot(
                np.array([error])) * self.learning_rate
            self.bias_output += np.sum(error, axis=0, keepdims=True) * self.learning_rate
            self.weights_input_hidden += np.array([inputs[i]]).T.dot(delta_hidden) * self.learning_rate
            self.bias_hidden += np.sum(delta_hidden, axis=0, keepdims=True) * self.learning_rate

    def train(self, inputs, targets, epochs, batch_size=4):
        start_time = time.time()
        for epoch in range(epochs):
            for i in range(0, len(inputs), batch_size):
                batch_inputs = inputs[i:i + batch_size]
                batch_targets = targets[i:i + batch_size]
                output = self.forward(batch_inputs)
                self.backward(batch_inputs, batch_targets, output)
            self.E **= (1 / len(inputs))
            print(f"Epoch: {epoch} MSE: {self.E}")
            self.E = 0
        end_time = time.time()
        elapsed_time = end_time - start_time
        print(f"Total training time: {elapsed_time:.4f} seconds")

    def forward(self, inputs):
        self.hidden_input = np.dot(inputs, self.weights_input_hidden) + self.bias_hidden
        self.hidden_output = self.sigmoid(self.hidden_input)
        self.output = np.dot(self.hidden_output, self.weights_hidden_output) + self.bias_output
        return self.output

    def backwardAdapt(self, inputs, target, output):
        for i in range(len(target)):
            error = target[i] - output[i]
            self.E += np.sum(error ** 2)
            delta_hidden = error.dot(self.weights_hidden_output.T) * self.sigmoid_derivative(
                np.array([self.hidden_output[i]]))

            adaptive_learning_rate = np.clip((4 * np.sum(delta_hidden * self.hidden_output * (
                1 - self.hidden_output))) / ((1 + np.sum(self.hidden_output ** 2)) * np.sum(
                    delta_hidden * self.hidden_output * (1 - self.hidden_output) ** 2)), -0.2, 0.1
                )

            self.weights_hidden_output += np.outer(self.hidden_output[i], error) * adaptive_learning_rate
            self.bias_output += np.sum(error, axis=0, keepdims=True) * adaptive_learning_rate
            self.weights_input_hidden += np.outer(inputs[i], delta_hidden) * adaptive_learning_rate
            self.bias_hidden += np.sum(delta_hidden, axis=0, keepdims=True) * adaptive_learning_rate

    def trainAdapt(self, inputs, targets, epochs, batch_size=4):
        start_time = time.time()
        for epoch in range(epochs):
            for i in range(0, len(inputs), batch_size):
                batch_inputs = inputs[i:i + batch_size]
                batch_targets = targets[i:i + batch_size]
                output = self.forward(batch_inputs)
                self.backwardAdapt(batch_inputs, batch_targets, output)
            self.E **= (1 / len(inputs))
            print(f"Epoch: {epoch} MSE: {self.E}")
            self.E = 0
        end_time = time.time()
        elapsed_time = end_time - start_time
        print(f"Total training time: {elapsed_time:.4f} seconds")

    def predict(self, inputs):
        output = self.forward(inputs)
        result = [el[0] for el in output]
        return result

def get_train_data(all_points, input_size):
    result_X = [all_points[i:i + input_size] for i in range(len(all_points) - input_size)]
    result_Y = [all_points[i] for i in range(input_size, len(all_points), 1)]
    return np.array(result_X), np.array(result_Y)

input_size = 6
hidden_size = 2
output_size = 1
all_train_points = f(np.arange(0, 200.4, 0.2))
all_test_points = f(np.arange(181.6, 380.4, 0.2))
X_train, Y_train = get_train_data(all_train_points, input_size)
X_test, Y_test = get_train_data(all_test_points, input_size)
X_train = normalize_data(X_train, 0, 1)
Y_train = normalize_data(Y_train, 0, 1)

# Constant Learning Rate
NN = Network(input_size, hidden_size, output_size)
NN.train(X_train, Y_train, epochs=100, batch_size=4)

plt.plot(np.arange(0, len(all_train_points) * 0.2, 0.2), all_train_points)
plt.plot(np.arange(181.6, 181.6 + len(NN.predict(X_test)) * 0.2, 0.2)[:988], NN.predict(X_test), ":")
plt.title("Constant Learning Rate")
plt.show()

# Adaptive Learning Rate
NN1 = Network(input_size, hidden_size, output_size)
NN1.trainAdapt(X_train, Y_train, epochs=100, batch_size=4)

plt.plot(np.arange(0, len(all_train_points) * 0.2, 0.2), all_train_points)
plt.plot(np.arange(181.6, 181.6 + len(NN1.predict(X_test)) * 0.2, 0.2)[:988], NN1.predict(X_test), ":")
plt.title("Adaptive Learning Rate")
plt.show()
