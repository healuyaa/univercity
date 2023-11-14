import time
import numpy as np
from math import sin
import matplotlib.pyplot as plt

f = lambda x: 1 * sin(5 * x) + 0.1

class Network:
    def __init__(self, weights=np.random.rand(4, 1)/5, T=0.1, a=0.0046) -> None:
        self.weights = weights
        self.T = T
        self.a = a
        self.E_arr = []

    def predict(self, input: list) -> float:
        return np.sum(input @ self.weights) - self.T

    def reset(self):
        self.weights = np.random.rand(4, 1)/5
        self.T = 0.1
        self.a = 0.0046
        self.E_arr = []

    def train_online_const(self, inputs, targets, epoch=1000) -> None:
        for i in range(epoch):
            for index, input_data in enumerate(inputs):
                prediction = self.predict(input_data)
                for j, w in enumerate(self.weights):
                    w[0] = w[0] - self.a * (prediction - targets[index]) * input_data[j]
                self.T = self.T + self.a * (prediction - targets[index])

            E, error_arr = 0, []
            for input_data, target in zip(inputs, targets):
                error_arr.append((self.predict(input_data) - target) ** 2)
            E = 0.5 * np.sum(error_arr)
            if i == epoch - 1:
                print("Epoch:", i, "Mean Squared Error:", E)
            self.E_arr.append(E)

    def train_online_adaptive(self, inputs, targets, epoch=1000) -> None:
        alpha = 0
        for i in range(epoch):
            for index, input_data in enumerate(inputs):
                prediction = self.predict(input_data)
                for j, w in enumerate(self.weights):
                    w[0] = w[0] - alpha * (prediction - targets[index]) * input_data[j]
                self.T = self.T + alpha * (prediction - targets[index])

            E, error_arr = 0, []
            for input_data, target in zip(inputs, targets):
                error_arr.append((self.predict(input_data) - target) ** 2)
            E = 0.5 * np.sum(error_arr)
            alpha = self.a * E
            if i == epoch - 1:
                print("Epoch:", i, "Mean Squared Error:", E, "Alpha:", alpha)
            self.E_arr.append(E)

    def train_batch_const(self, inputs, targets, epoch=1000, batch_size=4) -> None:
        for i in range(epoch):
            input_batches = [inputs[i-batch_size:i] for i in range(batch_size, len(inputs), batch_size)]
            target_batches = [targets[i-batch_size:i] for i in range(batch_size, len(targets), batch_size)]
            error_arr = []
            for input_batch, target_batch in zip(input_batches, target_batches):
                predictions = np.array([self.predict(data) for data in input_batch])
                error = predictions - target_batch
                delta = np.zeros_like(self.weights.transpose())
                for j in range(error.size):
                    delta = delta + (error[j] * input_batch[j])
                E2 = 0
                for err in error:
                    E2 += err ** 2
                E2 /= 2
                error_arr.append(E2)
                self.weights = self.weights - self.a * delta.transpose()
                self.T = self.T + self.a * np.sum(error)
            self.E_arr.append(np.mean(error_arr))

    def train_batch_adapt(self, inputs, targets, epoch=1000) -> None:
        for i in range(epoch):
            predictions = np.array([self.predict(data) for data in inputs])
            error = predictions - targets
            delta = np.zeros_like(self.weights.transpose())
            for j in range(error.size):
                delta = delta + (error[j] * inputs[j])
            E2 = 0
            for err in error:
                E2 += err ** 2
            E2 /= 2
            alpha = min(self.a * E2, 0.0046)
            self.E_arr.append(E2)
            if i == epoch - 1:
                print("Epoch:", i, "Mean Squared Error:", E2, "Alpha:", alpha)
            self.weights = self.weights - alpha * delta.transpose()
            self.T = self.T + alpha * np.sum(error)

x_100 = [el for el in np.arange(0, 1, 0.01)]  # Разбиение на 100 точек
data = [f(el) for el in x_100]  # Заполнение массива реальных значений функции
inputs = np.array([data[i-4:i] for i in range(4, len(data))])  # Создание массива входных значений
targets = data[4:]  # Создание массива целевых значений

x_60 = [el for el in np.arange(0.9, 1.5, 0.01)]  # Разбиение на 60 точек
data1 = [f(el) for el in x_60]  # Заполнение массива реальных значений функции
inputs1 = np.array([data1[i-4:i] for i in range(4, len(data1))])
targets1 = data1[4:]  # Создание массива целевых значений

epoch = 100
NN = Network()

for i in range(1):
    NN.reset()
    start = time.time()
    
    print("1. Online Learning with Constant Learning Rate")
    print("2. Online Learning with Adaptive Learning Rate")
    print("3. Batch Learning with Constant Learning Rate")
    print("4. Batch Learning with Adaptive Learning Rate")
    
    choice = int(input("Choose the training mode (1-4): "))
    
    if choice == 1:
        errors = NN.train_online_const(inputs, targets, epoch)
        end = time.time()
    elif choice == 2:
        errors = NN.train_online_adaptive(inputs, targets, epoch)
        end = time.time()
    elif choice == 3:
        errors = NN.train_batch_const(inputs, targets, epoch)
        end = time.time()
    elif choice == 4:
        errors = NN.train_batch_adapt(inputs, targets, epoch)
        end = time.time()
    
    print(f"Time taken: {(end-start)*10**3:.03f}ms")

    result = [NN.predict(input_data) for input_data in inputs1]
    
    # График ошибки
    plt.plot(range(epoch), NN.E_arr)
    plt.title('Training Error over Epochs')
    plt.xlabel('Epochs')
    plt.ylabel('Mean Squared Error')
    plt.show()
    
    # График предсказанных и реальных значений
    plt.plot(x_100, data, x_60[4:], result, '--')
    plt.title('Sine Function and Predicted Points')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.legend(['Actual', 'Predicted'])
    plt.show()