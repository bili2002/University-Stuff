import numpy as np

class NeuralNetwork:
    def __init__(self, input_size, hidden_layers, output_size, activation):
        self.input_size = input_size
        self.hidden_layers = hidden_layers
        self.output_size = output_size
        self.activation = self.sigmoid if activation == 0 else self.tanh
        self.activation_derivative = self.sigmoid_derivative if activation == 0 else self.tanh_derivative

        self.weights = []
        self.biases = []

        layer_sizes = [input_size] + hidden_layers + [output_size]
        for i in range(len(layer_sizes) - 1):
            self.weights.append(np.random.randn(layer_sizes[i], layer_sizes[i + 1]) * 0.1)
            self.biases.append(np.zeros((1, layer_sizes[i + 1])))

    def sigmoid(self, x):
        return 1 / (1 + np.exp(-x))

    def sigmoid_derivative(self, x):
        return x * (1 - x)

    def tanh(self, x):
        return np.tanh(x)

    def tanh_derivative(self, x):
        return 1 - x ** 2

    def forward(self, x):
        activations = [x]
        for w, b in zip(self.weights, self.biases):
            x = self.activation(np.dot(x, w) + b)
            activations.append(x)
        return activations

    def backward(self, activations, y):
        deltas = [activations[-1] - y]
        
        for i in reversed(range(len(self.weights) - 1)):
            delta = np.dot(deltas[0], self.weights[i + 1].T) * self.activation_derivative(activations[i + 1])
            deltas.insert(0, delta)

        return deltas

    def update_weights(self, activations, deltas, learning_rate):
        for i in range(len(self.weights)):
            self.weights[i] -= learning_rate * np.dot(activations[i].T, deltas[i])
            self.biases[i] -= learning_rate * np.sum(deltas[i], axis=0, keepdims=True)

    def train(self, x, y, epochs, learning_rate):
        for _ in range(epochs):
            activations = self.forward(x)
            deltas = self.backward(activations, y)
            self.update_weights(activations, deltas, learning_rate)

    def predict(self, x):
        return self.forward(x)[-1]

boolean_functions = {
    "AND": lambda x: np.array([[a & b] for a, b in x]),
    "OR": lambda x: np.array([[a | b] for a, b in x]),
    "XOR": lambda x: np.array([[a ^ b] for a, b in x])
}

inputs = np.array([[0, 0], [0, 1], [1, 0], [1, 1]])

def test_neural_network(function, activation, hidden_layers):
    nn = NeuralNetwork(input_size=2, hidden_layers=hidden_layers, output_size=1, activation=activation)

    if function == "ALL":
        for fn_name, fn in boolean_functions.items():
            outputs = fn(inputs)
            nn.train(inputs, outputs, epochs=5000, learning_rate=0.01)
            predictions = nn.predict(inputs)
            print(f"{fn_name}:")
            for inp, pred in zip(inputs, predictions):
                print(f"{tuple(inp)} -> {pred[0]:.4f}")
            print()
    else:
        outputs = boolean_functions[function](inputs)
        nn.train(inputs, outputs, epochs=5000, learning_rate=0.1)
        predictions = nn.predict(inputs)
        print(f"{function}:")
        for inp, pred in zip(inputs, predictions):
            print(f"{tuple(inp)} -> {pred[0]:.4f}")


# Input
function = "ALL" # AND, OR, XOR, ALL
activation = 0 # 0 - sig, 1 - tanh
hidden_layers = [2]
test_neural_network(function, activation, hidden_layers)
