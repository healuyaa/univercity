import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder, StandardScaler
from sklearn.neural_network import MLPRegressor
from sklearn.decomposition import PCA as SKPCA

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
        centered_data = X - self.mean
        return np.dot(centered_data, self.components)

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

# PCA (собственная реализация)
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)

custom_pca = CustomPCA(n_components=10)  # Выберите количество компонент
X_train_pca = custom_pca.fit_transform(X_train_scaled)
X_test_pca = custom_pca.transform(X_test_scaled)

# Автоэнкодер (с использованием scikit-learn)
autoencoder = MLPRegressor(hidden_layer_sizes=(10,), max_iter=1000, random_state=42)
autoencoder.fit(X_train_scaled, X_train_scaled)

# Оценка ошибки для PCA
pca_reconstruction_error = np.sum((X_test_scaled - custom_pca.transform(X_test_scaled).dot(custom_pca.components.T)) ** 2) / X_test_scaled.shape[0]

# Оценка ошибки для автоэнкодера
autoencoder_reconstruction_error = np.sum((X_test_scaled - autoencoder.predict(X_test_scaled)) ** 2) / X_test_scaled.shape[0]

print("PCA Reconstruction Error:", pca_reconstruction_error)
print("Autoencoder Reconstruction Error:", autoencoder_reconstruction_error)
