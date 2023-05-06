/**
 * @file emg_learning.h
 * @brief Header file for the EMGLearning class
 * @date Created: 2023-05-05
 * @date Modified: 2023-06-05
 * @version 1.3
 * This is the header file for the EMGLearning class. It contains the basic functionality for the learning algorithm.
*/

#include <Eigen/Eigen>
#include <iostream>
#include <vector>

#ifndef EMG_LEARNING_H
#define EMG_LEARNING_H

typedef float Scalar;
typedef Eigen::MatrixXf Matrix;
typedef Eigen::RowVectorXf RowVector;
typedef Eigen::VectorXf Vector;

class NeuralNetwork {
    public:
        NeuralNetwork(std::vector<uint> topology, Scalar learningRate = Scalar(0.005));
        void forwardPropagate(RowVector& input);
        void backwardPropagate(RowVector& output);

        void calcErrors(RowVector& output);
        void updateWeights();
        void train(std::vector<RowVector*> data, std::vector<RowVector*> outputData);

        std::vector<RowVector*> neuronLayers;
        std::vector<RowVector*> cacheLayers;
        std::vector<RowVector*> deltas;
        std::vector<Matrix*> weights;
        std::vector<uint> topology;
        Scalar learningRate;
    
    private:
        Scalar activationFunction(Scalar x);
        Scalar activationFunctionDerivative(Scalar x);
};

#endif