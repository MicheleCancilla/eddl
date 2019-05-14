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
#include <stdio.h>

#ifndef _LAYER_
#define _LAYER_

#include <string>

#include "../tensor.h"

#define TRMODE 1
#define TSMODE 0

using namespace std;


class Layer {
public:
    string name;
    Tensor *input;
    Tensor *output;
    Tensor *target;
    Tensor *delta;
    Layer *orig;

    vector<Tensor *> params;
    vector<Tensor *> gradients;

    vector<Layer *> parent;
    vector<Layer *> child;

    int mode;
    int dev;
    int lin, lout;
    int delta_bp;

    Layer(string name, int dev);

    void initialize();

    void reset();

    void info();

    void setmode(int m);

    Tensor getWeights();
    Tensor setWeights(Tensor bias);

    Tensor getBias();
    Tensor setBias(Tensor bias);

    //virtual
    virtual string plot(int c) { return ""; }

    virtual void addchild(Layer *l) {}

    virtual void addparent(Layer *l) {}

    virtual void forward() {}

    virtual void backward() {}

    virtual Layer *share(int c, int bs, vector<Layer *>) { return NULL; }

    virtual Layer *clone(int c, int bs, vector<Layer *>, int todev) { return NULL; }

};


/////////////////////////////////////////
/////////////////////////////////////////
// Layers with only one input
class LinLayer : public Layer {
public:

    LinLayer(string name, int dev);

    void addchild(Layer *l);

    void addparent(Layer *l);
};

/// Tensor Layer
class LTensor : public LinLayer {
public:
    static int tensor_created;

    LTensor(string fname);

    LTensor(const initializer_list<int> &init, int dev);

    LTensor(const vector<int> shape, int dev);

    LTensor(Layer *l);

    Layer *share(int c, int bs, vector<Layer *> p) { return NULL; }

    Layer *clone(int c, int bs, vector<Layer *>, int todev) { return NULL; }

    void info() {}

    void forward() {}

    void backward() {}

    string plot(int c) { return ""; }

    LTensor operator+(LTensor L);


};

/// INPUT Layer
class LInput : public LinLayer {
public:
    static int input_created;

    LInput(Tensor *in, string name, int dev);

    Layer *share(int c, int bs, vector<Layer *> p);

    Layer *clone(int c, int bs, vector<Layer *>, int todev);

    void forward();

    void backward();

    string plot(int c);

};

/// DENSE Layer
class LDense : public LinLayer {
public:
    int ndim;
    static int dense_created;

    LDense(Layer *parent, int ndim, string name, int d);

    Layer *share(int c, int bs, vector<Layer *> p);

    Layer *clone(int c, int bs, vector<Layer *>, int todev);

    // Params
    Tensor *W;
    Tensor *gW;
    Tensor *bias;
    Tensor *gbias;

    void forward();

    void backward();

    string plot(int c);

};

/// Activation Layer
class LActivation : public LinLayer {
public:
    string act;
    static int activation_created;

    LActivation(Layer *parent, string act, string name, int d);

    Layer *share(int c, int bs, vector<Layer *> p);

    Layer *clone(int c, int bs, vector<Layer *>, int todev);

    void forward();

    void backward();

    string plot(int c);

};

/// Resahpe Layer
class LReshape : public LinLayer {
public:
    static int reshape_created;
    vector<int> ls;

    // constructors and clones
    LReshape(Layer *parent, const initializer_list<int> &init, string name, int d);

    LReshape(Layer *parent, vector<int> shape, string name, int d);

    Layer *share(int c, int bs, vector<Layer *> p);

    Layer *clone(int c, int bs, vector<Layer *> p, int todev);


    // implementation
    void forward();

    void backward();

    string plot(int c);

};

/// Conv2D Layer
class LConv : public LinLayer {
public:
    static int conv_created;

    ConvolDescriptor *cd;

    // constructors and clones
    LConv(Layer *parent, const initializer_list<int> &ks, const initializer_list<int> &st, string p, string name,
          int d);

    LConv(Layer *parent, const initializer_list<int> &ks, const initializer_list<int> &st,
          const initializer_list<int> &p, string name, int d);

    LConv(Layer *parent, const vector<int> &ks, const vector<int> &st, string p, string name, int d);

    LConv(Layer *parent, ConvolDescriptor *cd, string name, int d);

    Layer *share(int c, int bs, vector<Layer *> p);

    Layer *clone(int c, int bs, vector<Layer *> p, int todev);

    // Params are in ConvolDescriptor

    // implementation
    void forward();

    void backward();

    string plot(int c);

};

/// Pool2D Layer
class LPool : public LinLayer {
public:
    static int pool_created;
    PoolDescriptor *pd;

    // constructors
    LPool(Layer *parent, PoolDescriptor *cd, string name, int d);
};

/// MaxPool2D Layer
class LMPool : public LPool {
public:

    // constructors and clones
    LMPool(Layer *parent, const initializer_list<int> &ks, const initializer_list<int> &st, string p, string name,
           int d);

    LMPool(Layer *parent, const initializer_list<int> &ks, const initializer_list<int> &st,
           const initializer_list<int> &p, string name, int d);

    LMPool(Layer *parent, const vector<int> &ks, const vector<int> &st, string p, string name, int d);

    LMPool(Layer *parent, PoolDescriptor *cd, string name, int d);

    // Params
    Tensor *indX, *indY;

    // implementation
    void forward();

    void backward();

    Layer *share(int c, int bs, vector<Layer *> p);

    Layer *clone(int c, int bs, vector<Layer *> p, int todev);

    string plot(int c);

};

/// Drop-out Layer
class LDrop : public LinLayer {
public:
    int ndim;
    static int drop_created;

    // constructors and clones
    LDrop(Layer *parent, float df, string name, int d);

    Layer *share(int c, int bs, vector<Layer *> p);

    Layer *clone(int c, int bs, vector<Layer *> p, int todev);

    float df;
    Tensor *mask;

    // implementation
    void forward();

    void backward();

    string plot(int c);

};


/////////////////////////////////////////
/////////////////////////////////////////
// Layers with several inputs (ADD, CAT,...)
class MLayer : public Layer {
public:

    MLayer(string name, int dev);

    void addchild(Layer *l);

    void addparent(Layer *l);

    //virtual

    virtual string plot(int c) { return ""; }

    virtual void forward() {}

    virtual void backward() {}

    virtual Layer *share(int c, int bs, vector<Layer *> p) { return NULL; }

    virtual Layer *clone(int c, int bs, vector<Layer *>, int todev) { return NULL; }

};

/// Add Layer
class LAdd : public MLayer {
public:
    static int add_created;


    LAdd(vector<Layer *> in, string name, int dev);

    Layer *share(int c, int bs, vector<Layer *> p);

    Layer *clone(int c, int bs, vector<Layer *>, int todev);

    void forward();

    void backward();

    string plot(int c);

};


/// Cat Layer
class LCat : public MLayer {
public:
    int ndim;
    vector<int> index;
    static int cat_created;

    // constructors and clones
    LCat(vector<Layer *> in, string name, int d);

    Layer *share(int c, int bs, vector<Layer *> p);

    Layer *clone(int c, int bs, vector<Layer *> p, int todev);

    // Params


    // implementation
    void forward();

    void backward();

    string plot(int c);

};

#endif
