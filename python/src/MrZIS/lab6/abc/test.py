from matplotlib import pyplot as plt
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score, classification_report
from math import isnan
from scipy.special import expit
import csv
import time

E_arr = []

class CategoricalPerceptron:
    def __init__(self, input_size, hidden_size, output_size, learning_rate=0.4):
        self.input_size = input_size
        self.hidden_size = hidden_size
        self.output_size = output_size
        self.learning_rate = learning_rate
        self.start_rate = learning_rate
        self.weights_input_hidden = np.random.randn(self.input_size, self.hidden_size)
        self.bias_hidden = np.zeros((1, self.hidden_size))
        self.weights_hidden_output = np.random.randn(self.hidden_size, self.output_size)
        self.bias_output = np.zeros((1, self.output_size))

    def sigmoid(self, x):
        return 1 / (1 + np.exp(-x))

    def sigmoid_derivative(self, y):
        return y * (1 - y)

    def forward(self, inputs):
        self.hidden_input = np.dot(inputs, self.weights_input_hidden) + self.bias_hidden
        self.hidden_output = self.sigmoid(self.hidden_input)
        self.output = np.dot(self.hidden_output, self.weights_hidden_output) + self.bias_output
        return self.sigmoid(self.output)

    def backward(self, inputs, target, output, isAdapt: bool = False):
        error = target - output
        deltaOutput = error * self.sigmoid_derivative(output)
        errorHiddenLayer = deltaOutput.dot(self.weights_hidden_output.T)
        deltaHiddenLayer = errorHiddenLayer * self.sigmoid_derivative(self.hidden_output)

        if isAdapt:
            self.learning_rate = (4*np.sum(deltaOutput**2 * output * (1-output)))/(
                (1+np.sum(output**2)) * np.sum(deltaOutput**2 * output**2 * (1-output)**2))/10 % 10
            if np.isnan(self.learning_rate):
                self.learning_rate = 0.4

        self.weights_hidden_output += self.hidden_output.T.dot(deltaOutput) * self.learning_rate
        self.bias_output += np.sum(deltaOutput, axis=0) * self.learning_rate

        if isAdapt:
            self.learning_rate = (4*np.sum(deltaHiddenLayer**2 * self.hidden_output * (1-self.hidden_output)))/((1+np.sum(
                self.hidden_output**2)) * np.sum(deltaHiddenLayer**2 * self.hidden_output**2 * (1-self.hidden_output)**2))/10 % 10
            if np.isnan(self.learning_rate):
                self.learning_rate = 0.4

        self.weights_input_hidden += inputs.T.dot(deltaHiddenLayer) * self.learning_rate
        self.bias_hidden += np.sum(deltaHiddenLayer, axis=0) * self.learning_rate

    def train(self, inputs, targets, epochs: int, isAdapt: bool = False):
        global E_arr
        for epoch in range(epochs):
            e_arr = []
            for i in range(len(inputs)):
                input_data = np.array([inputs[i]])
                target_data = np.array([targets[i]])
                output = self.forward(input_data)
                e_arr.append(target_data - output)
                self.backward(input_data, target_data, output, isAdapt)
            E2 = np.sum(np.array(e_arr)**2)/ len(inputs)
            E_arr.append(E2)

    def backwardBatch(self, inputs, targets, outputs, isAdapt: bool = False):
        for i in range(len(outputs)):
            error = targets[i] - outputs[i]
            deltaOutput = np.array(error * self.sigmoid_derivative(outputs[i]))
            errorHiddenLayer = deltaOutput.dot(self.weights_hidden_output.T)
            deltaHiddenLayer = errorHiddenLayer * self.sigmoid_derivative(self.hidden_output)

            if isAdapt:
                self.learning_rate = (4*np.sum(deltaOutput**2 * outputs[i] * (1-outputs[i])))/(
                    (1+np.sum(outputs[i]**2)) * np.sum(deltaOutput**2 * outputs[i]**2 * (1-outputs[i])**2))/10 % 10
                if np.isnan(self.learning_rate):
                    self.learning_rate = 0.4

            s = self.hidden_output.T.dot(deltaOutput) * self.learning_rate
            s = np.array([[s[0]], [s[1]]])
            self.weights_hidden_output += s
            self.bias_output += np.sum(deltaOutput, axis=0) * self.learning_rate

            if isAdapt:
                self.learning_rate = (4*np.sum(deltaHiddenLayer**2 * self.hidden_output * (1-self.hidden_output)))/((1+np.sum(
                    self.hidden_output**2)) * np.sum(deltaHiddenLayer**2 * self.hidden_output**2 * (1-self.hidden_output)**2))/10 % 10
                if np.isnan(self.learning_rate):
                    self.learning_rate = 0.4

            self.weights_input_hidden += inputs[i].reshape(-1, 1).dot(deltaHiddenLayer) * self.learning_rate
            self.bias_hidden += np.sum(deltaHiddenLayer, axis=0) * self.learning_rate

    def trainBatch(self, inputs, targets, epochs: int, batchsize: int, isAdapt: bool = False):
        global E_arr
        if (len(inputs) % batchsize != 0):
            print("Плохое значение пакета")
            return ValueError
        inputspack = [inputs[i-batchsize:i]
                      for i in range(batchsize, len(inputs), batchsize)]
        targetspack = [targets[i-batchsize:i]
                       for i in range(batchsize, len(targets), batchsize)]
        for epoch in range(epochs):
            e_arr = []
            for i in range(len(inputspack)):
                outputs = [self.forward(batchElem).item()
                           for batchElem in inputspack[i]]
                for j in range(len(targetspack[i])):
                    e_arr.append(targetspack[i][j] - outputs[j])
                self.backwardBatch(inputspack[i], targetspack[i], outputs)
            E2 = np.sum(np.array(e_arr)**2)/ len(inputspack)  # fix this line
            E_arr.append(E2)

    def predict(self, inputs):
        output = self.forward(inputs)
        return output

def load_mushroom_data(file_path):
    X, Y = [], []
    with open(file_path, newline='') as file:
        reader = csv.reader(file)
        next(reader)  # Skip header
        for row in reader:
            X.append([1.0] + [float(ord(value)) for value in row[1:]])
            Y.append([1.0 if row[0] == 'p' else 0.0])
    return np.array(X), np.array(Y)

if __name__ == "__main__":
    epochs = 10
    E_arr = []

    X, Y = load_mushroom_data("src/MrZIS/lab6/abc/mushrooms.csv")
    X_train, X_test, y_train, y_test = train_test_split(X, Y, test_size=0.2, random_state=42)

    perceptron = CategoricalPerceptron(input_size=X.shape[1], hidden_size=2, output_size=1)
    start = time.time()
    perceptron.train(X_train, y_train, epochs)
    end = time.time()
    temp = perceptron.predict(X_test)
    y_pred = (temp > 0.5).astype(int)
    accuracy = accuracy_score(y_test, y_pred)
    print(f"Время выполнения: {(end-start):.03f}s")
    print(f"Точность модели : {accuracy:.2f}")
    print(f"Точность модели (MSE) : {min(E_arr):.10f}")
    plt.plot(range(epochs), E_arr)
    plt.show()
    E_arr.clear()
    print(classification_report(y_test, y_pred, zero_division=1))

    perceptron = CategoricalPerceptron(input_size=X.shape[1], hidden_size=2, output_size=1)
    start = time.time()
    perceptron.train(X_train, y_train, epochs, True)
    end = time.time()
    temp = perceptron.predict(X_test)
    y_pred = (temp > 0.5).astype(int)
    accuracy = accuracy_score(y_test, y_pred)
    print(f"Время выполнения: {(end-start):.03f}s")
    print(f"Точность модели: {accuracy:.2f}")
    print(f"Точность модели (MSE) : {min(E_arr):.10f}")
    plt.plot(range(epochs), E_arr)
    plt.show()
    E_arr.clear()
    print(classification_report(y_test, y_pred, zero_division=1))

    perceptron = CategoricalPerceptron(input_size=X.shape[1], hidden_size=2, output_size=1)
    start = time.time()
    perceptron.trainBatch(X_train, y_train, epochs, 2, False)
    end = time.time()
    temp = perceptron.predict(X_test)
    y_pred = (temp > 0.5).astype(int)
    accuracy = accuracy_score(y_test, y_pred)
    print(f"Время выполнения: {(end-start):.03f}s")
    print(f"Точность модели: {accuracy:.2f}")
    print(f"Точность модели (MSE) : {min(E_arr):.10f}")
    plt.plot(range(epochs), E_arr)
    plt.show()
    E_arr.clear()
    print(classification_report(y_test, y_pred, zero_division=1))

    perceptron = CategoricalPerceptron(input_size=X.shape[1], hidden_size=2, output_size=1)
    start = time.time()
    perceptron.trainBatch(X_train, y_train, epochs, 2, True)
    end = time.time()
    temp = perceptron.predict(X_test)
    y_pred = (temp > 0.5).astype(int)
    accuracy = accuracy_score(y_test, y_pred)
    print(f"Время выполнения: {(end-start):.03f}s")
    print(f"Точность модели: {accuracy:.2f}")
    print(f"Точность модели (MSE) : {min(E_arr):.10f}")
    plt.plot(range(epochs), E_arr)
    plt.show()
    print(classification_report(y_test, y_pred, zero_division=1))
