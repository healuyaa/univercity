import numpy as np
import matplotlib.pyplot as plt
import time

# Функция для создания набора данных
def generate_data(num_points):
    np.random.seed(0)
    X = np.sort(5 * np.random.rand(num_points, 1), axis=0)
    y = np.sin(X * 5) + 0.1 * np.random.randn(num_points, 1)
    return X, y

# Функция для создания и обучения Single Layer Perceptron
def train_slp(X, y, learning_rate=0.013, epochs=100):
    input_size = X.shape[1]
    output_size = y.shape[1]

    weights = np.random.rand(input_size, output_size)
    # weights = np.array([[0.1]])
    bias = np.random.rand(output_size)

    start_time = time.time()
    errors = []

    for epoch in range(epochs):
        for i in range(X.shape[0]):
            # Forward pass
            output = np.dot(X[i], weights) + bias

            # Calculate error
            error = y[i] - output

            # Update weights and bias using adaptive learning rate
            weights += learning_rate * error * X[i].reshape(-1, 1)
            bias += learning_rate * error

        # Calculate test error
        test_output = np.dot(X, weights) + bias
        test_error = np.mean((y - test_output)**2)
        errors.append(test_error)

        print(f"Epoch {epoch + 1}/{epochs}, Test Error: {test_error}")

    end_time = time.time()
    training_time = end_time - start_time

    # Save results to a file
    with open("./src/MrZIS/lab2/results_onl_a_l.txt", "w") as file:
    # with open("./src/MrZIS/lab2/results_onl_s_l.txt", "w") as file:
        file.write(f"Test Error: {test_error}\n")
        file.write(f"Epochs: {epochs}\n")
        file.write(f"Training Time: {training_time} seconds\n")

    # Plot the learning curve
    plt.plot(range(1, epochs + 1), errors, label='Test Error')
    plt.xlabel('Epochs')
    plt.ylabel('Error')
    plt.title('Learning Curve')
    plt.legend()
    plt.show()

# Генерация данных
num_points = 100
X, y = generate_data(num_points)

# Обучение Single Layer Perceptron
train_slp(X, y)
