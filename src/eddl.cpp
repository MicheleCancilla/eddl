// This file is part of EDDLL an European Distributed Deep Learning Library.
// Developed within the DeepHealth project.
// Boosting AI in Europe.
//
// The MIT License (MIT)
//
// Copyright (c) 2019
//           Roberto Paredes Palacios, <rparedes@dsic.upv.es>
//           Jon Ander Gómez, <jon@dsic.upv.es>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "eddl.h"


using namespace std;

extern ostream &operator<<(ostream &os, const vector<int> shape);

EDDL eddl;

////////////////////////////////////////////////////////
///// EDDL is a wrapper class to ease and define the API
////////////////////////////////////////////////////////

tensor EDDL::T(const initializer_list<int> &shape) {
    vector<int> s(shape.begin(), shape.end());
    return T(s);
}

tensor EDDL::T(const vector<int> shape) {
    return new LTensor(shape, DEV_CPU);
}

tensor EDDL::T(string fname) {
    return new LTensor(fname);
}


void EDDL::div(tensor t, float v) {
    t->input->div(v);
}
//////////////////////////////////////////////////////


layer EDDL::Activation(layer parent, string activation) {
    return EDDL::Activation(parent, activation, "activation" + to_string(1 + LActivation::activation_created));
}

layer EDDL::Activation(layer parent, string activation, string name) {
    return new LActivation(parent, activation, name, DEV_CPU);
}
//////////////////////////////////////////////////////

layer EDDL::BatchNormalization(layer parent, float momentum, float epsilon, bool affine, string name){
    //Todo: Implement
}

//////////////////////////////////////////////////////

layer EDDL::Conv(layer parent, const initializer_list<int> &ks) {
    return new LConv(parent, ks, {1, 1}, "same", "conv" + to_string(1 + LConv::conv_created), DEV_CPU);
}

layer EDDL::Conv(layer parent, const initializer_list<int> &ks, const initializer_list<int> &st) {
    return new LConv(parent, ks, st, "same", "conv" + to_string(1 + LConv::conv_created), DEV_CPU);
}

layer EDDL::Conv(layer parent, const initializer_list<int> &ks, const initializer_list<int> &st, string p) {
    return new LConv(parent, ks, st, p, "conv" + to_string(1 + LConv::conv_created), DEV_CPU);
}

layer EDDL::Conv(layer parent, const initializer_list<int> &ks, string p) {
    return new LConv(parent, ks, {1, 1}, p, "conv" + to_string(1 + LConv::conv_created), DEV_CPU);
}

layer EDDL::Conv(layer parent, int filters, const initializer_list<int> &kernel_size, string padding,
                 const initializer_list<int> &strides, int groups, const initializer_list<int> &dilation_rate,
                 bool use_bias, string name) {
    // TODO: IMPLEMENT
}

//////////////////////////////////////////////////////
layer EDDL::Dense(layer parent, int ndim) {
    return EDDL::Dense(parent, ndim, "dense" + to_string(1 + LDense::dense_created));
}

layer EDDL::Dense(layer parent, int ndim, string name) {
    return new LDense(parent, ndim, name, DEV_CPU);
}

//////////////////////////////////////////////////////

layer EDDL::DeConv(layer parent, int filters, const initializer_list<int> &kernel_size, string padding,
        const initializer_list<int> &output_padding, const initializer_list<int> &dilation_rate,
        const initializer_list<int> &strides, bool use_bias, string name){
    // TODO: IMPLEMENT
}

/////////////////////////////////////////////////////////
layer EDDL::Dropout(layer parent, float rate) {
    return EDDL::Dropout(parent, rate, "drop" + to_string(1 + LDrop::drop_created));
}

layer EDDL::Dropout(layer parent, float rate, string name) {
    return new LDrop(parent, rate, name, DEV_CPU);
}

//////////////////////////////////////////////////////
layer EDDL::Embedding(int input_dim, int output_dim, string name){
    // TODO: IMPLEMENT
}

//////////////////////////////////////////////////////
layer EDDL::GaussianNoise(layer parent, float stdev, string name){
    // TODO: IMPLEMENT
}

//////////////////////////////////////////////////////

layer EDDL::Input(tensor t) {
    return new LInput(t->input, "input" + to_string(1 + LInput::input_created), DEV_CPU);
}

layer EDDL::Input(const initializer_list<int> &shape) {
    return EDDL::Input(shape, "input" + to_string(1 + LInput::input_created));
}

layer EDDL::Input(const initializer_list<int> &shape, string name) {
    return new LInput(new Tensor(shape), name, DEV_CPU);
}

//////////////////////////////////////////////////////

layer EDDL::Interpolate(layer parent, const initializer_list<int> &size, string interpolation, string name){
    // TODO: IMPLEMENT
}

//////////////////////////////////////////////////////
layer EDDL::AveragePool(layer parent, const initializer_list<int> &pool_size) {
    return EDDL::AveragePool(parent, pool_size, pool_size);
}

layer EDDL::AveragePool(layer parent, const initializer_list<int> &pool_size, const initializer_list<int> &strides, string padding){
    //TODO: Fix LMPool
    return EDDL::AveragePool(parent, pool_size, strides, padding, "avgpool" + to_string(1 + LMPool::pool_created));
}

layer EDDL::AveragePool(layer parent, const initializer_list<int> &pool_size, const initializer_list<int> &strides, string padding, string name){
    //TODO: Implement
}

//////////////////////////////////////////////////////
static layer GlobalMaxPool(layer parent){
    //TODO: Fix LMPool
    //return EDDL::GlobalMaxPool(parent, "globalmaxpool" + to_string(1 + LMPool::pool_created));
}

static layer GlobalMaxPool(layer parent, string name){
    //TODO: Implement
}

//////////////////////////////////////////////////////
static layer GlobalAveragePool(layer parent){
    //TODO: Fix LMPool
    //return EDDL::GlobalAveragePool(parent, "globalavgpool" + to_string(1 + LMPool::pool_created));
}

static layer GlobalAveragePool(layer parent, string name){
    //TODO: Implement
}


//////////////////////////////////////////////////////
layer EDDL::MaxPool(layer parent, const initializer_list<int> &pool_size) {
    return EDDL::MaxPool(parent, pool_size, pool_size);
}

layer EDDL::MaxPool(layer parent, const initializer_list<int> &pool_size, const initializer_list<int> &strides, string padding){
    return EDDL::MaxPool(parent, pool_size, strides, padding, "mpool" + to_string(1 + LMPool::pool_created));
}

layer EDDL::MaxPool(layer parent, const initializer_list<int> &pool_size, const initializer_list<int> &strides, string padding, string name){
    return new LMPool(parent, pool_size, strides, padding, name, DEV_CPU);

}

//////////////////////////////////////////////////////
layer EDDL::RNN(layer parent, int units, int num_layers, bool use_bias, float dropout, bool bidirectional){
    //TODO: Fix name
    return EDDL::RNN(parent, units, num_layers, use_bias, dropout, bidirectional, "rnn" + to_string(1 + LMPool::pool_created));
}

layer EDDL::RNN(layer parent, int units, int num_layers, bool use_bias, float dropout, bool bidirectional, string name){
    //TODO: Implement
}

//////////////////////////////////////////////////////
layer EDDL::LSTM(layer parent, int units, int num_layers, bool use_bias, float dropout, bool bidirectional){
    //TODO: Fix name
    return EDDL::LSTM(parent, units, num_layers, use_bias, dropout, bidirectional, "lstm" + to_string(1 + LMPool::pool_created));
}

layer EDDL::LSTM(layer parent, int units, int num_layers, bool use_bias, float dropout, bool bidirectional, string name){
    //TODO: Implement
}
//////////////////////////////////////////////////////
layer EDDL::Reshape(layer parent, const initializer_list<int> &shape) {
    return EDDL::Reshape(parent, shape, "reshape" + to_string(1 + LReshape::reshape_created));
}

layer EDDL::Reshape(layer parent, const initializer_list<int> &shape, string name) {
    return new LReshape(parent, shape, name, DEV_CPU);
}

/////////////////////////////////////////////////////////

layer EDDL::Transpose(layer parent, const initializer_list<int> &dims, string name){
    //TODO: Implement
}


/////////////////////////////////////////////////////////

layer EDDL::Add(const initializer_list<layer> &layers) {
    return EDDL::Add(layers, "add" + to_string(1 + LAdd::add_created));
}

layer EDDL::Add(const initializer_list<layer> &layers, string name) {
    return new LAdd(vlayer(layers.begin(), layers.end()), name, DEV_CPU);
}

////////////////////////////////////////////////////////

static layer Average(const initializer_list<layer> &layers){
    // TODO: Fix LCAT
    return EDDL::Average(layers, "average" + to_string(1 + LCat::cat_created));
}

static layer Average(const initializer_list<layer> &layers, string name){
    //TODO: Implement
}

/////////////////////////////////////////////////////////

layer EDDL::Subtract(const initializer_list<layer> &layers) {
    // TODO: Fix LAadd
    return EDDL::Subtract(layers, "substract" + to_string(1 + LAdd::add_created));
}

layer EDDL::Subtract(const initializer_list<layer> &layers, string name) {
    //TODO: Implement
}

////////////////////////////////////////////////////////

layer EDDL::Concat(const initializer_list<layer> &layers) {
    return EDDL::Concat(layers, "concat" + to_string(1 + LCat::cat_created));
}

layer EDDL::Concat(const initializer_list<layer> &layers, string name) {
    return new LCat(vlayer(layers.begin(), layers.end()), name, DEV_CPU);
}

////////////////////////////////////////////////////////

static layer MatMul(const initializer_list<layer> &layers){
    // TODO: Fix LCAT
    return EDDL::MatMul(layers, "matmul" + to_string(1 + LCat::cat_created));
}

static layer MatMul(const initializer_list<layer> &layers, string name){
    //TODO: Implement
}

////////////////////////////////////////////////////////

static layer Maximum(const initializer_list<layer> &layers){
    // TODO: Fix LCAT
    return EDDL::Maximum(layers, "maximum" + to_string(1 + LCat::cat_created));
}

static layer Maximum(const initializer_list<layer> &layers, string name){
    //TODO: Implement
}

////////////////////////////////////////////////////////

static layer Minimum(const initializer_list<layer> &layers){
    // TODO: Fix LCAT
    return EDDL::Minimum(layers, "minimum" + to_string(1 + LCat::cat_created));
}

static layer Minimum(const initializer_list<layer> &layers, string name){
    //TODO: Implement
}


////////////////////////////////////////////////////////

optimizer EDDL::SGD(const initializer_list<float> &p) {
    return new sgd(p);
}

void EDDL::change(optimizer o, const initializer_list<float> &p) {
    o->change(p);
}

////////////////////////////////////////////////////////

initializer EDDL::Constant(float value){
    //Todo: Implement
}

initializer EDDL::Identity(float gain){
    //Todo: Implement
}
initializer EDDL::GlorotNormal(float seed) {
    //Todo: Implement
}
initializer EDDL::GlorotUniform(float seed){
    //Todo: Implement
}
initializer EDDL::RandomNormal(float mean, float stdev, int seed){
    //Todo: Implement
}
initializer EDDL::RandomUniform(float minval, float maxval, int seed){
    //Todo: Implement
}
initializer EDDL::Orthogonal(float gain, int seed){
    //Todo: Implement
}


/////////////////////////////////////////////////////////
model EDDL::Model(vlayer in, vlayer out) {
    return new Net(in, out);
}

///////////
compserv EDDL::CS_CPU(int th) {
    return new CompServ(th, {}, {});
}

compserv EDDL::CS_GPU(const initializer_list<int> &g) {
    return new CompServ(0, g, {});
}

compserv EDDL::CS_FGPA(const initializer_list<int> &f) {
    return new CompServ(0, {}, f);
}


////////////

void EDDL::info(model m) {
    m->info();
}

void EDDL::plot(model m, string fname) {
    m->plot(fname);
}

void EDDL::build(model net, optimizer o, const initializer_list<string> &c, const initializer_list<string> &m) {
    net->build(o, c, m);
}

void EDDL::build(model net, optimizer o, const initializer_list<string> &c, const initializer_list<string> &m,
                 CompServ *cs) {
    net->build(o, c, m, cs);
}

void EDDL::fit(model net, const initializer_list<LTensor *> &in, const initializer_list<LTensor *> &out, int batch,
               int epochs) {
    vltensor ltin = vltensor(in.begin(), in.end());
    vltensor ltout = vltensor(out.begin(), out.end());

    vtensor tin;
    for (int i = 0; i < ltin.size(); i++)
        tin.push_back(ltin[i]->input);

    vtensor tout;
    for (int i = 0; i < ltout.size(); i++)
        tout.push_back(ltout[i]->input);


    net->fit(tin, tout, batch, epochs);
}

void EDDL::evaluate(model net, const initializer_list<LTensor *> &in, const initializer_list<LTensor *> &out) {
    vltensor ltin = vltensor(in.begin(), in.end());
    vltensor ltout = vltensor(out.begin(), out.end());

    vtensor tin;
    for (int i = 0; i < ltin.size(); i++)
        tin.push_back(ltin[i]->input);

    vtensor tout;
    for (int i = 0; i < ltout.size(); i++)
        tout.push_back(ltout[i]->input);


    net->evaluate(tin, tout);
}


////

bool exist(string name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    }
    return false;
}

void EDDL::download_mnist() {
    string cmd;
    string trX = "trX.bin";
    string trY = "trY.bin";
    string tsX = "tsX.bin";
    string tsY = "tsY.bin";

    if ((!exist(trX)) || (!exist(trY)) || (!exist(tsX)) || (!exist(tsY))) {
        cmd = "wget https://www.dropbox.com/s/khrb3th2z6owd9t/trX.bin";
        int status = system(cmd.c_str());
        if (status < 0) {
            msg("wget must be installed", "eddl.download_mnist");
            exit(1);
        }

        cmd = "wget https://www.dropbox.com/s/m82hmmrg46kcugp/trY.bin";
        status = system(cmd.c_str());
        if (status < 0) {
            msg("wget must be installed", "eddl.download_mnist");
            exit(1);
        }
        cmd = "wget https://www.dropbox.com/s/7psutd4m4wna2d5/tsX.bin";
        status = system(cmd.c_str());
        if (status < 0) {
            msg("wget must be installed", "eddl.download_mnist");
            exit(1);
        }
        cmd = "wget https://www.dropbox.com/s/q0tnbjvaenb4tjs/tsY.bin";
        status = system(cmd.c_str());
        if (status < 0) {
            msg("wget must be installed", "eddl.download_mnist");
            exit(1);
        }

    }
}












//////
