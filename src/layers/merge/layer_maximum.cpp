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

#include "layer_merge.h"


using namespace std;

int LMaximum::total_layers = 0;

LMaximum::LMaximum(vector<Layer *> parent, string name, int dev) : MLayer(name, dev) {
    if (parent.size() == 0) msg("Error: LMaximum layer with empty list");

    if (parent.size() > 1)
        for (int i = 0; i < parent.size() - 1; ++i)
            if (!Tensor::eqsize(parent[i]->output, parent[i + 1]->output)) {
                parent[i]->output->info();
                parent[i + 1]->output->info();
                msg("Error: LMaximum layers with different tensor shape");
            }

    total_layers++;

    input = parent[0]->output;

    output = new Tensor(parent[0]->output->getShape(), dev);
    delta = new Tensor(parent[0]->output->getShape(), dev);

    for (int i = 0; i < parent.size(); ++i) {
        parent[i]->addchild(this);
        addparent(parent[i]);
    }

}


// virtual

string LMaximum::plot(int c) {
    string s;

    s = name + " [label=" + "\"" + name + "\",style=filled,fontsize=12,fillcolor=lightblue3,shape=box]";

    return s;
}


void LMaximum::forward() {
    // TODO: Implement
    output->set(0.0);
    for (int i = 0; i < parent.size(); ++i)
        Tensor::inc(parent[i]->output, output);

}

void LMaximum::backward() {
    // TODO: Implement
    for (int i = 0; i < parent.size(); ++i)
        Tensor::inc(delta, parent[i]->delta);
}

Layer *LMaximum::share(int c, int bs, vector<Layer *> p) {
    LMaximum *n = new LMaximum(p, "share_" + to_string(c) + name, dev);
    n->orig = this;

    return n;
}


Layer *LMaximum::clone(int c, int bs, vector<Layer *> p, int todev) {
    LMaximum *n = new LMaximum(p, "share_" + to_string(c) + name, todev);
    n->orig = this;

    return n;
}





///////////////
