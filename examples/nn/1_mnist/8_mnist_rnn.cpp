/*
* EDDL Library - European Distributed Deep Learning Library.
* Version: 0.3
* copyright (c) 2019, Universidad Politécnica de Valencia (UPV), PRHLT Research Centre
* Date: October 2019
* Author: PRHLT Research Centre, UPV, (rparedes@prhlt.upv.es), (jon@prhlt.upv.es)
* All rights reserved
*/

#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "eddl/apis/eddl.h"
#include "eddl/apis/eddlT.h"

using namespace eddl;

//////////////////////////////////
// mnist_rnn.cpp:
// A recurrent NN for mnist
// Using fit for training
//////////////////////////////////

int main(int argc, char **argv) {
    // Download mnist
    download_mnist();

    // Settings
    int epochs = 100;
    int batch_size = 100;
    int num_classes = 10;

    // Define network
    layer in = Input({28});
    layer l = in;  // Aux var

    l = LeakyReLu(Dense(l, 32));
    l = RNN(l, 32, "tanh");
    l = RNN(l, 32, "tanh");

    l = LeakyReLu(Dense(l, 32));

    layer out = Softmax(Dense(l, num_classes));
    model net = Model({in}, {out});
    net->verbosity_level = 0;

    // dot from graphviz should be installed:
    plot(net, "model.pdf");

    // Build model
    build(net,
          rmsprop(0.001), // Optimizer
          {"soft_cross_entropy"}, // Losses
          {"categorical_accuracy"}, // Metrics
          //CS_GPU({1,1},100) // one GPU
          CS_CPU(-1,"low_mem") // CPU with maximum threads availables
    );

    // View model
    summary(net);


    // Load dataset
    tensor x_train = eddlT::load("mnist_trX.bin");
    tensor y_train = eddlT::load("mnist_trY.bin");
    tensor x_test = eddlT::load("mnist_tsX.bin");
    tensor y_test = eddlT::load("mnist_tsY.bin");

    // Reshape to fit recurrent batch x timestep x dim
    x_train->reshape_({60000,28,28});
    x_test->reshape_({10000,28,28});

    // Preprocessing
    eddlT::div_(x_train, 255.0);
    eddlT::div_(x_test, 255.0);

    setlogfile(net,"recurrent_mnist");

    // Train model
    fit(net,{x_train}, {y_train}, batch_size, epochs);
    evaluate(net, {x_test}, {y_test});



}