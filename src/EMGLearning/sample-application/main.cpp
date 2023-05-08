/**
 * @file main.cpp
 * @brief Main file for the EMGLearning application
 * @date Created: 2023-05-05
 * @date Modified: 2023-06-05
 * @version 1.3
 * This is the main file for the EMGLearning application. It contains the main function.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "../emg_learning.h"

/**
 * @brief A helper ReadCSV function
 * @param filename The name of the file to read
 * @param data The data to read into
 * @return void
*/
void ReadCSV(const std::string& filename, std::vector<RowVector*>& data)
{
    data.clear();
    std::ifstream file(filename);
    std::string line, word;
    // determine number of columns in file
    getline(file, line, '\n');
    std::stringstream ss(line);
    std::vector<Scalar> parsed_vec;
    while (getline(ss, word, ',')) {
        parsed_vec.push_back(Scalar(std::stof(word)));
    }
    uint cols = static_cast<uint>(parsed_vec.size());
    data.push_back(new RowVector(cols));
    for (uint i = 0; i < cols; i++) {
        data.back()->coeffRef(0, i) = parsed_vec[i];
    }

    // read the file
    if (file.is_open()) {
        while (getline(file, line, '\n')) {
            std::stringstream ss(line);
            data.push_back(new RowVector(1, cols));
            uint i = 0;
            while (getline(ss, word, ',')) {
                data.back()->coeffRef(0, i) = Scalar(std::stof(word));
                i++;
            }
        }
    }
}

/**
 * @brief A testing helper function to generate data
 * @param filename The name of the file to write to
 * @return void
 * @details This function generates data for the EMGLearning application by taking a between 0 and 1 and writing it to an input file
 * and writing the output of the function 2xy + 10 + y to an output file.
*/
void genData(const std::string& filename)
{
    std::ofstream file1(filename + "-in");
    std::ofstream file2(filename + "-out");
    for (uint r = 0; r < 100000; r++) {
        Scalar x = static_cast<Scalar>(rand()) / static_cast<Scalar>(RAND_MAX);
        Scalar y = static_cast<Scalar>(rand()) / static_cast<Scalar>(RAND_MAX);
        file1 << x << "," << y << std::endl;
        file2 << 2 * x * y + 10 + y << std::endl;
    }
    file1.close();
    file2.close();
}


/**
 * @brief A testing helper function to train a model
 * @param filename The name of the file to write to
 * @param iterations The number of iterations to train for
 * @param learningRate The learning rate to use
 * @return void
 * @details This function trains a model for the EMGLearning application by taking a between 0 and 1 and writing it to an input file
*/
void trainModel(const std::string& filename, int iterations, Scalar learningRate=0.005)
{
    std::vector<RowVector*> in_dat, out_dat;
    ReadCSV(filename + "-in", in_dat);
    ReadCSV(filename + "-out", out_dat);
    NeuralNetwork n({ 2, 4, 5, 1 }, learningRate);
    for (uint i = 0; i < iterations; i++){
        if (i > 0 )
            n.loadWeights(filename + "Weights");
        std::cout << "Epoch " << i << std::endl;
        n.train(in_dat, out_dat);
        n.saveWeights(filename + "Weights");
    }
    n.saveWeights(filename + "Weights");
}

/**
 * @brief A testing helper function to test a trained model
 * @param filename The name of the file to write to
 * @return void
 * @details This function tests a trained model for the EMGLearning application by taking a between 0 and 1 and writing it to an input file
*/
void testTrainedModel(const std::string& filename)
{
    std::vector<RowVector*> in_dat, out_dat;
    ReadCSV(filename + "-in", in_dat);
    ReadCSV(filename + "-out", out_dat);
    NeuralNetwork n({ 2, 4, 5, 1 }, 0.005);
    n.loadWeights(filename + "Weights");
    uint32_t correct = 0;
    for (uint32_t i = 0; i < in_dat.size(); i++) {
        float out = n.predict(*in_dat[i]);
        if (abs(out - out_dat[i]->coeffRef(0, 0)) < 0.1)
            correct++;
    }
    std::cout << "Accuracy: " << (static_cast<Scalar>(correct) / static_cast<Scalar>(in_dat.size())) * 100 << "%" << std::endl;
}

/**
 * @brief The main function
 * @return int
 * @details This function is the main function for the EMGLearning application. It calls the helper functions to generate data, train a model, and test a trained model.
*/
int main()
{
    genData("myTest");
    trainModel("myTest", 25);
    testTrainedModel("myTest");
    return 0;
}