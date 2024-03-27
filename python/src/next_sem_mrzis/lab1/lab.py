import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder, StandardScaler
from sklearn.neural_network import MLPRegressor
import matplotlib.pyplot as plt
import torch
import torch.nn as nn

class CustomPCA:
    def __init__(self, n_components):
        self.n_components = n_components
        self.components = None
        self.mean = None

    def fit_transform(self, X):
        self.mean = np.mean(X, axis=0)
        centered_data = X - self.mean
        covariance_matrix = np.cov(centered_data, rowvar=False)
        eigenvalues, eigenvectors = np.linalg.eigh(covariance_matrix)
        idx = np.argsort(eigenvalues)[::-1]
        self.components = eigenvectors[:, idx[:self.n_components]]
        return np.dot(centered_data, self.components)

    def transform(self, X):
        if self.mean is None:
            raise ValueError("Mean has not been calculated. Call fit_transform first.")
        centered_data = X - self.mean
        return np.dot(centered_data, self.components)

class CustomAutoencoder(nn.Module):
    def __init__(self, input_size, hidden_size):
        super(CustomAutoencoder, self).__init__()
        self.encoder = nn.Linear(input_size, hidden_size)
        self.decoder = nn.Linear(hidden_size, input_size)

    def forward(self, x):
        x = torch.tanh(self.encoder(x))
        x = torch.sigmoid(self.decoder(x))
        return x

# Загрузка данных
url = "https://archive.ics.uci.edu/ml/machine-learning-databases/mushroom/agaricus-lepiota.data"
columns = ["class", "cap-shape", "cap-surface", "cap-color", "bruises", "odor",
           "gill-attachment", "gill-spacing", "gill-size", "gill-color",
           "stalk-shape", "stalk-root", "stalk-surface-above-ring", "stalk-surface-below-ring",
           "stalk-color-above-ring", "stalk-color-below-ring", "veil-type", "veil-color",
           "ring-number", "ring-type", "spore-print-color", "population", "habitat"]

data = pd.read_csv(url, header=None, names=columns)

# Преобразование данных
label_encoder = LabelEncoder()
for column in data.columns:
    data[column] = label_encoder.fit_transform(data[column])

# Разделение на признаки и целевую переменную
X = data.drop("class", axis=1)
y = data["class"]

# Разделение на обучающий и тестовый наборы
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Масштабирование данных
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)

# Вычисление собственных значений и собственных векторов
covariance_matrix = np.cov(X_train_scaled.T)
eigenvalues, eigenvectors = np.linalg.eig(covariance_matrix)

# Визуализация объясненной дисперсии
cumulative_explained_variance = np.cumsum(eigenvalues) / np.sum(eigenvalues)
plt.plot(cumulative_explained_variance, marker='o')
plt.xlabel('Number of Principal Components')
plt.ylabel('Cumulative Explained Variance')
plt.title('Cumulative Explained Variance vs. Number of Principal Components')
plt.show()

# Определение оптимального числа компонент
num_components = np.argmax(cumulative_explained_variance >= 0.95) + 1
print(f'Optimal number of components: {num_components}')

# PCA (собственная реализация)
custom_pca = CustomPCA(n_components=num_components)
X_train_pca = custom_pca.fit_transform(X_train_scaled)
X_test_pca = custom_pca.transform(X_test_scaled)

# Автоэнкодер (с использованием CustomAutoencoder)
input_size = X_train_scaled.shape[1]
hidden_size = 10
autoencoder = CustomAutoencoder(input_size, hidden_size)
criterion = nn.MSELoss()
optimizer = torch.optim.SGD(autoencoder.parameters(), lr=0.001, momentum=0.9)

# Преобразование данных в тензоры PyTorch
X_train_tensor = torch.tensor(X_train_scaled, dtype=torch.float)
X_test_tensor = torch.tensor(X_test_scaled, dtype=torch.float)

# Обучение автоэнкодера
num_epochs = 1000
for epoch in range(num_epochs):
    # Forward pass
    outputs = autoencoder(X_train_tensor)
    loss = criterion(outputs, X_train_tensor)
    
    # Backward pass and optimization
    optimizer.zero_grad()
    loss.backward()
    optimizer.step()

# Преобразование данных с помощью автоэнкодера
with torch.no_grad():
    encoded_data = autoencoder.encoder(X_test_tensor).numpy()

# Визуализация результата PCA с добавлением вывода автоэнкодера
plt.scatter(X_train_pca[:, 0], X_train_pca[:, 1], c='blue', label='Train (PCA)')
plt.scatter(X_test_pca[:, 0], X_test_pca[:, 1], c='red', label='Test (PCA)')
plt.scatter(encoded_data[:, 0], encoded_data[:, 1], c='green', label='Autoencoder')
plt.xlabel('Dimension 1')
plt.ylabel('Dimension 2')
plt.title('PCA vs. Autoencoder Result')
plt.legend()
plt.show()

# Оценка ошибки для PCA
pca_reconstruction_error = np.sum((X_test_scaled - custom_pca.transform(X_test_scaled).dot(custom_pca.components.T)) ** 2) / X_test_scaled.shape[0]

# Оценка ошибки для автоэнкодера
autoencoder_reconstruction_error = np.sum((X_test_scaled - autoencoder(X_test_tensor).detach().numpy()) ** 2) / X_test_scaled.shape[0]

print("PCA Reconstruction Error:", pca_reconstruction_error)
print("Autoencoder Reconstruction Error:", autoencoder_reconstruction_error)
