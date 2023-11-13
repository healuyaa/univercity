import numpy as np
import matplotlib.pyplot as plt
import time

def generate_data(num_points):
    np.random.seed(0)
    X = np.sort(5 * np.random.rand(num_points, 1), axis=0)
    y = np.sin(X * 5) + 0.1 * np.random.randn(num_points, 1)
    return X, y

def initialize_weights(input_size, output_size):
    weights = np.array([[0.1], [0.2]])
    # weights = np.random.randn(input_size, output_size) * 0.01
    bias = np.zeros((1, output_size))
    return weights, bias

def train_slp(X, y, learning_rate=0.013, epochs=100, beta=0.9):
    input_size = X.shape[1]
    output_size = y.shape[1]

    weights, bias = initialize_weights(input_size, output_size)
    velocity_weights = np.zeros_like(weights)
    velocity_bias = np.zeros_like(bias)

    start_time = time.time()
    errors = []

    for epoch in range(epochs):
        # Forward pass
        output = np.dot(X, weights) + bias

        # Calculate error
        error = y - output

        # Update weights and bias using adaptive learning rate and momentum
        velocity_weights = beta * velocity_weights + (1 - beta) * learning_rate * np.dot(X.T, error)
        velocity_bias = beta * velocity_bias + (1 - beta) * learning_rate * np.sum(error, axis=0)

        weights += velocity_weights
        bias += velocity_bias

        # Calculate test error
        test_output = np.dot(X, weights) + bias
        test_error = np.mean((y - test_output)**2)
        errors.append(test_error)

        print(f"Epoch {epoch + 1}/{epochs}, Test Error: {test_error}")

    end_time = time.time()
    training_time = end_time - start_time

    # Save results to a file
    with open("./src/MrZIS/lab2/results_btc_a_l.txt", "w") as file:
    # with open("./src/MrZIS/lab2/results_btc_a_l.txt", "w") as file:
        file.write(f"Final Test Error: {test_error}\n")
        file.write(f"Epochs: {epochs}\n")
        file.write(f"Training Time: {training_time} seconds\n")

    # Plot the learning curve
    plt.plot(range(1, epochs + 1), errors, label='Test Error')
    plt.xlabel('Epochs')
    plt.ylabel('Error')
    plt.title('Learning Curve')
    plt.legend()
    plt.show()

num_points = 100
X, y = generate_data(num_points)

# Добавляем столбец единиц для учета смещения (bias)
X_bias = np.hstack((X, np.ones_like(X)))

train_slp(X_bias, y)
