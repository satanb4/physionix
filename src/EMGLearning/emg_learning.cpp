/**
 * @file emg_learning.cpp
 * @brief EMG learning class
 * @date Created: 2023-05-05
 * @date Modified: 2023-06-05
 * @version 1.3
 * @author Bandyopadhyay, Sayan
*/

#include <iostream>
#include <algorithm>
#include <functional>
#include <fstream>
#include <sstream>
#include <string>
#include "emg_learning.h"

// #define DEBUG

/**
 * @brief Constructor for NeuralNetwork class
 * @param topology Vector of unsigned integers
 * @param learningRate Scalar
 * @return None
 * @details This is the constructor for the NeuralNetwork class. It initializes the neural network with the given topology and learning rate.
*/
NeuralNetwork::NeuralNetwork(std::vector<uint> topology, Scalar learningRate) {
    this->topology = topology;
    this->learningRate = learningRate;

    // Initializing the neuron layers
    for (uint i = 0; i < topology.size(); i++) {
        if (i == topology.size() - 1)
            neuronLayers.push_back(new RowVector(topology[i]));
        else
            neuronLayers.push_back(new RowVector(topology[i] + 1));
        
        // Initializing the cache and delta layers
        cacheLayers.push_back(new RowVector(neuronLayers.size()));
        deltas.push_back(new RowVector(neuronLayers.size()));

        // vector.back() returns a reference to the last element in the vector
        // coeffRef returns a reference to the coefficient at the given index
        // Since we are using pointers we set a value to the coeff address of the last element
        if (i != topology.size() - 1) {
            neuronLayers.back()->coeffRef(topology[i]) = 1.0; 
            cacheLayers.back()->coeffRef(topology[i]) = 1.0;
        }

        // Initializing weight matrix
        if (i > 0) {
            // Matrix(rows, cols)
            if (i != topology.size() - 1 ) {
                weights.push_back(new Matrix(topology[i-1] + 1, topology[i] + 1)); // Adding 1 to the number of rows for the bias
                weights.back()->setRandom(); // Random initialization for the weights
                weights.back()->col(topology[i]).setZero(); // Initializing the bias
                weights.back()->coeffRef(topology[i-1], topology[i]) = 1.0; // Setting the bias to 1
            }
            else {
                // The last layer does not have a column for the bias
                weights.push_back(new Matrix(topology[i-1] + 1, topology[i])); 
                weights.back()->setRandom(); // Random initialization for the bias
            }
        }
    }
}

/**
 * @brief Forward propagates the input through the network
 * @param input RowVector
 * @return None
 * @details The input is set to the first layer of the network. The input is then forward propagated through the network.
*/
void NeuralNetwork::forwardPropagate(RowVector& input) {
    // Setting the input layer
    // block(row, col, rows, cols)
    neuronLayers.front()->block(0,0,1, neuronLayers.front()->size() - 1) = input;

    // Forward propagating the input
    // unaryExpr applies the activation function to each element of the matrix
    for (uint i = 1; i<topology.size(); i++) {
        (*neuronLayers[i]) = (*neuronLayers[i-1]) * (*weights[i-1]);
        neuronLayers[i]->block(0, 0, 1, topology[i]).unaryExpr([this](double x) { return activationFunction(x); }).eval();
    }
}

/**
 * @brief Back propagates the error through the network
 * @param input RowVector
 * @return None
 * @details The error is calculated for each layer and the weights are updated accordingly
*/
void NeuralNetwork::calcErrors(RowVector& output) {
    // Calculating the error for the output layer
    (*deltas.back()) = output - (*neuronLayers.back());

    // Error calculation for the hidden layers is done in reverse order
    // The delta for the hidden layer is the dot product of the delta of the next layer and the transpose of the weight matrix
    for (uint i = topology.size() -2; i>0 ; i--) {
        (*deltas[i]) = (*deltas[i+1]) * (weights[i]->transpose());
    }
}

/**
 * @brief Updates the weights of the network
 * @param None
 * @return None
 * @details The weights are updated using the delta and the learning rate
*/
void NeuralNetwork::updateWeights() {
    /// Updating the weights for the hidden layers
    /// If the layer is the output layer, the bias is not updated, number of neurons = number of columns
    /// If the layer is a hidden layer, the bias is updated, number of neurons = number of columns - 1
    for (uint i = 0; i < topology.size() - 1; i++) {
        if (i != topology.size() - 2) {
            for (uint c=0; c<weights[i]->cols() -1; c++) {
                for (uint r=0; r<weights[i]->rows(); r++) {
                    /// The weight is updated by the product of the learning rate, the delta, the derivative of the activation function and the neuron value
                    weights[i]->coeffRef(r,c) += learningRate * deltas[i + 1]->coeffRef(c) * activationFunctionDerivative(cacheLayers[i+1]->coeffRef(c)) * neuronLayers[i]->coeffRef(r); 
                }
            }
        }
        else {
            for (uint c=0; c< weights[i]->cols(); c++) {
                for (uint r=0; r< weights[i]->rows(); r++){
                    weights[i]->coeffRef(r,c) += learningRate * deltas[i + 1]->coeffRef(c) * activationFunctionDerivative(cacheLayers[i+1]->coeffRef(c)) * neuronLayers[i]->coeffRef(r);
                }
            }
        }
    }
}

/**
 * @brief Back propagates the error through the network
 * @param input RowVector
 * @return None
 * @details The error is calculated for each layer and the weights are updated accordingly
*/
void NeuralNetwork::backwardPropagate(RowVector& output) {
    calcErrors(output);
    updateWeights();
}

Scalar NeuralNetwork::activationFunction(Scalar x) {
    return tanhf(x);
}

Scalar NeuralNetwork::activationFunctionDerivative(Scalar x) {
    return 1 - tanhf(x) * tanhf(x);
}

/**
 * @brief Trains the network
 * @param input_data vector<RowVector*>
 * @param output_data vector<RowVector*>
 * @return None
 * @details The input and output data are passed to the network and the network is trained using the forward and backward propagation
*/
void NeuralNetwork::train(std::vector<RowVector*> input_data, std::vector<RowVector*> output_data) {
    std::cout << "Training started" << std::endl;
    for (uint i = 0; i < input_data.size(); i++) {
        
        forwardPropagate(*input_data[i]);
        backwardPropagate(*output_data[i]);

        #ifdef DEBUG
        std::cout << "Input to neural network is : " << *input_data[i] << std::endl;
        std::cout << "Expected output is : " << *output_data[i] << std::endl;
        std::cout << "Output produced is : " << *neuronLayers.back() << std::endl;
        std::cout << "MSE : " << std::sqrt((*deltas.back()).dot((*deltas.back())) / deltas.back()->size()) << std::endl;
        #endif
    }
    std::cout << "Training complete" << std::endl;
}

/**
 * @brief Predicts the output for the given input
 * @param input RowVector
 * @return None
 * @details The input is passed to the network and the output is predicted
*/
void NeuralNetwork::predict(RowVector& input) {
    forwardPropagate(input);
    std::cout << "Output produced is : " << *neuronLayers.back() << std::endl;
}

/**
 * @brief Saves the weights of the network to a file
 * @param filename string
 * @return None
 * @details The weights are saved to a file
*/
void NeuralNetwork::saveWeights(std::string filename) {
    std::ofstream file;
    file.open(filename);
    for (uint i = 0; i < weights.size(); i++) {
        file << *weights[i] << std::endl;
    }
    file.close();
}

/**
 * @brief Loads the weights of the network from a file
 * @param filename string
 * @return None
 * @details The weights are loaded from a file
*/
void NeuralNetwork::loadWeights(std::string filename) {
    std::ifstream file;
    file.open(filename);
    std::string line;
    for (uint i = 0; i < weights.size(); i++) {
        for (uint r=0; r<weights[i]->rows(); r++) {
            getline(file, line);
            std::stringstream ss(line);
            for (uint c=0; c<weights[i]->cols(); c++) {
                ss >> weights[i]->coeffRef(r,c);
            }
        }
    }
    file.close();
}

/**
 * @brief Prints the weights of the network
 * @param None
 * @return None
 * @details The weights are printed to the console
*/
void NeuralNetwork::printWeights() {
    for (uint i = 0; i < weights.size(); i++) {
        std::cout << "Weight " << i << std::endl;
        std::cout << *weights[i] << std::endl;
    }
}

/**
 * @brief Destructor for the network
 * @param None
 * @return None
 * @details The memory allocated for the network is freed
*/
NeuralNetwork::~NeuralNetwork() {
    for (uint i = 0; i<neuronLayers.size(); i++) {
        delete neuronLayers[i];
        delete cacheLayers[i];
        delete deltas[i];
        if (i != neuronLayers.size() - 1) {
            delete weights[i];
        }
    }
}