import numpy as np
import pandas as pd
from sklearn.preprocessing import StandardScaler
import matplotlib.pyplot as plt

# URL для загрузки данных
url = "https://archive.ics.uci.edu/ml/machine-learning-databases/00236/seeds_dataset.txt"

# Загрузка данных
data = pd.read_csv(url, header=None, delim_whitespace=True)

# Выделение признаков
X = data.iloc[:, :-1].values

# Масштабирование данных
scaler = StandardScaler()
X = scaler.fit_transform(X)

class AutoencoderCombined():
    def __init__(self, input_neuron, encoding_neuron, learning_rate, rule_type, alpha=None):
        if rule_type not in ['oja', 'cumulative']:
            raise ValueError("Invalid rule type. Choose either 'oja' or 'cumulative'.")

        self.input_neuron = input_neuron
        self.encoding_neuron = encoding_neuron
        self.learning_rate = learning_rate
        self.rule_type = rule_type
        self.alpha = alpha

        self.weights_input_encoding = np.random.rand(input_neuron, encoding_neuron)
        self.weights_encoding_input = np.random.rand(encoding_neuron, input_neuron)
        self.bias_encoding = np.random.rand(1, encoding_neuron)
        self.bias_input = np.random.rand(1, input_neuron)

        if rule_type == 'cumulative':
            self.prev_delta_weights_input_encoding = np.zeros((input_neuron, encoding_neuron))
            self.prev_delta_weights_encoding_input = np.zeros((encoding_neuron, input_neuron))
            self.prev_delta_bias_encoding = np.zeros((1, encoding_neuron))
            self.prev_delta_bias_input = np.zeros((1, input_neuron))

    def sigmoid(self, x):
        return 1 / (1 + np.exp(-x))

    def sigmoid_derivative(self, x):
        return x * (1 - x)

    def encode(self, inputs):
        self.encoding_layer_input = np.dot(inputs, self.weights_input_encoding) + self.bias_encoding
        self.encoding_layer_output = self.sigmoid(self.encoding_layer_input)

    def decode(self, encoding_output):
        self.input_layer_input = np.dot(encoding_output, self.weights_encoding_input) + self.bias_input
        self.input_layer_output = self.sigmoid(self.input_layer_input)

    def backward_pass(self, inputs):
        encoding_error = inputs - self.input_layer_output
        encoding_delta = encoding_error * self.sigmoid_derivative(self.input_layer_output)

        delta_weights_encoding_input = self.learning_rate * np.dot(self.encoding_layer_output.T, encoding_delta) / inputs.shape[0]

        if self.rule_type == 'cumulative':
            delta_weights_encoding_input += self.alpha * self.prev_delta_weights_encoding_input
            delta_bias_input = self.learning_rate * np.mean(encoding_delta, axis=0, keepdims=True) + self.alpha * self.prev_delta_bias_input

            self.weights_encoding_input += delta_weights_encoding_input
            self.bias_input += delta_bias_input

            decoding_error = encoding_delta.dot(self.weights_encoding_input.T)
            decoding_delta = decoding_error * self.sigmoid_derivative(self.encoding_layer_output)

            delta_weights_input_encoding = self.learning_rate * np.dot(inputs.T, decoding_delta) / inputs.shape[0] + self.alpha * self.prev_delta_weights_input_encoding
            delta_bias_encoding = self.learning_rate * np.mean(decoding_delta, axis=0, keepdims=True) + self.alpha * self.prev_delta_bias_encoding

            self.weights_input_encoding += delta_weights_input_encoding
            self.bias_encoding += delta_bias_encoding

            self.prev_delta_weights_input_encoding = delta_weights_input_encoding
            self.prev_delta_weights_encoding_input = delta_weights_encoding_input
            self.prev_delta_bias_encoding = delta_bias_encoding
            self.prev_delta_bias_input = delta_bias_input

        elif self.rule_type == 'oja':
            for i in range(self.encoding_neuron):
                delta_w = self.learning_rate * (self.encoding_layer_output[:, i][:, np.newaxis] * (inputs - self.encoding_layer_output[:, i][:, np.newaxis] * self.weights_input_encoding[:, i])).mean(axis=0)
                self.weights_input_encoding[:, i] += delta_w

        self.weights_encoding_input += delta_weights_encoding_input

        self.bias_input += self.learning_rate * np.mean(encoding_delta, axis=0, keepdims=True)

    def train(self, inputs, epochs=10):
        self.errors = []
        for _ in range(epochs):
            self.encode(inputs)
            self.decode(self.encoding_layer_output)
            self.backward_pass(inputs)

            # Вычисление и сохранение среднеквадратичной ошибки
            mse = np.mean((inputs - self.input_layer_output) ** 2)
            self.errors.append(mse)

    def encode_data(self, inputs_data):
        self.encode(inputs_data)
        return self.encoding_layer_output

    def decode_data(self, encoded_data):
        self.decode(encoded_data)
        return self.input_layer_output

# Используем Oja's Rule
autoencoder_oja = AutoencoderCombined(input_neuron=X.shape[1], encoding_neuron=5, learning_rate=0.093, rule_type='oja')
autoencoder_oja.train(X, epochs=10000)

# Используем Cumulative Delta Rule
autoencoder_cumulative = AutoencoderCombined(input_neuron=X.shape[1], encoding_neuron=7, learning_rate=0.033, rule_type='cumulative', alpha=0.9)
autoencoder_cumulative.train(X, epochs=2000)

# Получение восстановленных данных
X_decode_data_oja_uci = autoencoder_oja.decode_data(autoencoder_oja.encode_data(X))
X_decode_data_cumulative_uci = autoencoder_cumulative.decode_data(autoencoder_cumulative.encode_data(X))

# Построение графиков
plt.figure(figsize=(12, 6))

# График восстановленных данных
plt.subplot(1, 2, 1)
plt.scatter(X_decode_data_oja_uci[:, 0], X_decode_data_oja_uci[:, 1], c='r', label='Ойя', alpha=0.5)
plt.scatter(X_decode_data_cumulative_uci[:, 0], X_decode_data_cumulative_uci[:, 1], c='g', label='Кумулятивное дельта', alpha=0.5)
plt.xlabel('Признак 1')
plt.ylabel('Признак 2')
plt.title('Сравнение восстановленных данных по Ойя и Кумулятивному дельта')
plt.legend()

# График ошибок
plt.subplot(1, 2, 2)
plt.plot(autoencoder_oja.errors, label='Ойя', color='r')
plt.plot(autoencoder_cumulative.errors, label='Кумулятивное дельта', color='g')
plt.xlabel('Эпохи')
plt.ylabel('Среднеквадратичная ошибка')
plt.title('График ошибок')
plt.legend()

plt.tight_layout()
plt.show()
