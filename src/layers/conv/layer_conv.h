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

#ifndef EDDLL_LAYER_CONV_H
#define EDDLL_LAYER_CONV_H

#include <string>
#include <stdio.h>

#include "../layer.h"

#define TRMODE 1
#define TSMODE 0

using namespace std;


/// Conv2D Layer
class LConv : public LinLayer {
public:
    static int total_layers;

    ConvolDescriptor *cd;

    // constructors and clones
    LConv(Layer *parent, const initializer_list<int> &ks, const initializer_list<int> &st, string p, string name, int d);

    LConv(Layer *parent, const initializer_list<int> &ks, const initializer_list<int> &st,
          const initializer_list<int> &p, string name, int d);

    LConv(Layer *parent, int filters, const initializer_list<int> &kernel_size, const initializer_list<int> &strides, string padding,
          int groups, const initializer_list<int> &dilation_rate, bool use_bias, string name, int dev);

    LConv(Layer *parent, const vector<int> &ks, const vector<int> &st, string p, string name, int d);

    LConv(Layer *parent, ConvolDescriptor *cd, string name, int d);

    Layer *share(int c, int bs, vector<Layer *> p) override;

    Layer *clone(int c, int bs, vector<Layer *> p, int todev) override;

    // Params are in ConvolDescriptor

    // implementation
    void forward() override;

    void backward() override;

    string plot(int c) override;

};

/// ConvT2D Layer
class LConvT : public LinLayer {
public:
    static int total_layers;

    // constructors and clones
    LConvT(Layer *parent, int filters, const initializer_list<int> &kernel_size,
           const initializer_list<int> &output_padding, string padding, const initializer_list<int> &dilation_rate,
           const initializer_list<int> &strides, bool use_bias, string name, int dev);

    LConvT(Layer *parent, ConvolDescriptor *cd, string name, int dev);

//    Layer *share(int c, int bs, vector<Layer *> p) override;
//
//    Layer *clone(int c, int bs, vector<Layer *> p, int todev) override;
//
//    // Params are in ConvolDescriptor
//
//    // implementation
//    void forward() override;
//
//    void backward() override;
//
//    string plot(int c) override;

};

/// UpSampling2D Layer
class LUpSampling : public LinLayer {
public:
    vector<int> size;
    string interpolation;
    static int total_layers;

    // constructors and clones
    LUpSampling(Layer *parent, const initializer_list<int> &size, string interpolation, string name, int dev);

//    Layer *share(int c, int bs, vector<Layer *> p) override;
//
//    Layer *clone(int c, int bs, vector<Layer *> p, int todev) override;
//
//    // Params are in ConvolDescriptor
//
//    // implementation
//    void forward() override;
//
//    void backward() override;
//
//    string plot(int c) override;

};

#endif //EDDLL_LAYER_CONV_H

