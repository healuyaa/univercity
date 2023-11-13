import numpy as np
import matplotlib.pyplot as plt

class Perceptron:
    def __init__(self, size):
        self.weights = np.random.rand(size)
        self.bias = np.random.rand(1)

    def activation_function(self, x):
        y = (-1) * x + 1
        if y <= x: 
            return 1
        return 0

    def predict(self, inputs):
        weighted_sum = np.dot(inputs, self.weights) + self.bias
        return self.activation_function(weighted_sum)

    def train(self, training_inputs, labels, learning_rate, epochs):
        errors = []
        for _ in range(epochs):
            total_error = 0
            for inputs, label in zip(training_inputs, labels):
                prediction = self.predict(inputs)
                error = label - prediction
                total_error += abs(error)
                self.weights += learning_rate * error * inputs
                self.bias += learning_rate * error
            if(total_error == 0):
                errors.append(total_error)
                break
            errors.append(total_error)
            
            
        plt.plot(range(1, len(errors) + 1), errors)
        plt.xlabel('Epochs')
        plt.ylabel('Sum of errors')
        plt.title('Error change chart')
        plt.show()

    def plot_decision_boundary(self, training_inputs, labels):
        x_min, x_max = min(training_inputs[:, 0]) - 1, max(training_inputs[:, 0]) + 1
        y_min, y_max = min(training_inputs[:, 1]) - 1, max(training_inputs[:, 1]) + 1

        xx, yy = np.meshgrid(np.arange(x_min, x_max, 0.01),
                             np.arange(y_min, y_max, 0.01))

        Z = np.array([self.predict(np.array([x, y])) for x, y in zip(xx.ravel(), yy.ravel())])
        Z = Z.reshape(xx.shape)

        plt.contourf(xx, yy, Z, alpha=0.3, cmap=plt.cm.coolwarm)
        plt.scatter(training_inputs[:, 0], training_inputs[:, 1], c=labels, cmap=plt.cm.coolwarm)
        plt.xlabel('Feature 1')
        plt.ylabel('Feature 2')
        plt.title('Condition plot (decision boundary and points)')
        plt.show()


if __name__ == "__main__":
    training_inputs = np.array([[0, 0], [0, 1], [1, 0], [1, 1]])
    labels = np.array([0, 0, 0, 1])

    p1 = Perceptron(size = 2)
    learning_rate = 0.113
    epochs = 8

    p1.train(training_inputs, labels, learning_rate, epochs)

    test_inputs = np.array([[0, 0], [0, 1], [1, 0], [1, 1]])
    for inputs in test_inputs:
        prediction = p1.predict(inputs)
        print(f"Input: {inputs}, Prediction: {prediction}")

    p1.plot_decision_boundary(training_inputs, labels)