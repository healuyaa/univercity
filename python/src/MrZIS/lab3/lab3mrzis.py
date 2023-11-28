import numpy as np
import random
import time
from matplotlib import pyplot as plt

E_arr = []

class SingleLayerPerceptron:
    def __init__(self, input_size, learning_rate=0.0046):
        self.input_size = input_size
        self.weights = np.random.randn(input_size)
        self.bias = np.random.randn(1)
        self.learning_rate = learning_rate
        self.start_rate = learning_rate

    def reset(self):
        global E_arr
        E_arr.clear()
        self.weights = np.random.randn(self.input_size)
        self.bias = np.random.randn(1)
        self.learning_rate = self.start_rate

    def predict(self, x):
        linear_output = np.dot(self.weights, x) - self.bias
        sigmoid_output = 1 / (1 + np.exp(-linear_output))
        return 1 if sigmoid_output >= 0.5 else 0

    def train_constant_learning_rate(self, X, y, epochs: int, isAdapt: bool = False):
        start_time = time.time()
        global E_arr

        for epoch in range(epochs):
            e_arr = []

            for i in range(len(X)):
                prediction = self.predict(X[i])
                error = prediction - y[i]
                e_arr.append(error)

                self.weights = self.weights - self.learning_rate * error * np.array(X[i])
                self.bias = self.bias + self.learning_rate * error

            self.learning_rate = self.start_rate * (1.0 / (1.0 + epoch)) if isAdapt else self.learning_rate

            E_arr.append(np.sum(abs(np.array(e_arr))))

        end_time = time.time()
        training_time = end_time - start_time
        return training_time

    def train_batch_learning(self, X, y, epochs: int = 100, batch_size: int = 10, isAdapt: bool = False):
        start_time = time.time()
        global E_arr

        for epoch in range(epochs):
            e_arr = []

            for i in range(0, len(X), batch_size):
                X_batch = X[i:i + batch_size]
                y_batch = y[i:i + batch_size]

                predictions = [self.predict(x) for x in X_batch]
                errors = np.array(predictions) - np.array(y_batch)

                self.weights = self.weights - self.learning_rate * np.dot(errors, X_batch)
                self.bias = self.bias + self.learning_rate * np.sum(errors)              
                
                for err in errors:
                    e_arr.append(err)

            self.learning_rate = self.start_rate * (1.0 / (1.0 + epoch)) if isAdapt else self.learning_rate

            E_arr.append(np.sum(abs(np.array(e_arr))))

        end_time = time.time()
        training_time = end_time - start_time
        return training_time

def generate_dataset():
    X = []
    Y = []

    for _ in range(500):
        x1 = random.uniform(-10, 10)
        x2 = 2 * x1 - 3 + random.uniform(-10, 10)
        X.append([x1, x2])
        Y.append(1 if x2 > 2 * x1 - 3 else 0)

    for _ in range(40):
        index = int(random.uniform(0, len(Y)))
        Y[index] = 1 - Y[index]

    return X, Y

def split_dataset(X, y, split_ratio=0.8):
    split_index = int(len(X) * split_ratio)
    X_train = X[:split_index]
    y_train = y[:split_index]
    X_test = X[split_index:]
    y_test = y[split_index:]
    return X_train, y_train, X_test, y_test

def calculate_test_error(model, X_test, y_test):
    predictions = [model.predict(x) for x in X_test]
    errors = [1 if p != y else 0 for p, y in zip(predictions, y_test)]
    test_error = sum(errors) / len(errors)
    return test_error

X, Y = generate_dataset()

X_test, Y_test = [], []
for _ in range(500):
    x1 = random.uniform(-10, 10)
    x2 = 2 * x1 - 3 + random.uniform(-10, 10)
    X_test.append([x1, x2])
    Y_test.append(1 if x2 > 2 * x1 - 3 else 0)

epochs = 100
model = SingleLayerPerceptron(2)

# online adapt false
training_time = model.train_constant_learning_rate(X, Y, epochs=epochs, isAdapt=False)
test_error = calculate_test_error(model, X_test, Y_test)
print(f"{training_time}, ms,    err: {test_error},    sum min err: {min(E_arr)}")
plt.plot(range(epochs), E_arr, '-')
model.reset()

# online adapt true
training_time = model.train_constant_learning_rate(X, Y, epochs=epochs, isAdapt=True)
test_error = calculate_test_error(model, X_test, Y_test)
print(f"{training_time}, ms,    err: {test_error},    sum min err: {min(E_arr)}")
plt.plot(range(epochs), E_arr, '--')
model.reset()

# batch adapt false
training_time = model.train_batch_learning(X, Y, epochs=epochs, isAdapt=False)
test_error = calculate_test_error(model, X_test, Y_test)
print(f"{training_time}, ms,    err: {test_error},    sum min err: {min(E_arr)}")
plt.plot(range(epochs), E_arr, '-.')
model.reset()

# batch adapt true
training_time = model.train_batch_learning(X, Y, epochs=epochs, isAdapt=True)
test_error = calculate_test_error(model, X_test, Y_test)
print(f"{training_time}, ms,    err: {test_error},    sum min err: {min(E_arr)}")
plt.plot(range(epochs), E_arr, ':')

plt.legend(['Online-Const', 'Online-Adapt', 'Batch-Const', 'Batch-Adapt'], loc="upper right")
plt.show()
