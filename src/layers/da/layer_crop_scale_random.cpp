/*
* EDDL Library - European Distributed Deep Learning Library.
* Version: 0.3
* copyright (c) 2019, Universidad Politécnica de Valencia (UPV), PRHLT Research Centre
* Date: October 2019
* Author: PRHLT Research Centre, UPV, (rparedes@prhlt.upv.es), (jon@prhlt.upv.es)
* All rights reserved
*/


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <utility>

#include "layer_da.h"


using namespace std;

int LCropScaleRandom::total_layers = 0;

LCropScaleRandom::LCropScaleRandom(Layer *parent, vector<float> factor, string da_mode, string name, int dev, int mem) : LDataAugmentation(parent, name, dev, mem) {
    if(name.empty()) this->name = "crop_scale" + to_string(++total_layers);

    output = new Tensor(input->shape, dev);

    // Params
    this->factor=std::move(factor);
    this->da_mode=std::move(da_mode);

    parent->addchild(this);
    addparent(parent);
}


void LCropScaleRandom::forward() {
    Tensor::crop_scale_random(this->input, this->output, this->factor, this->da_mode);
}

void LCropScaleRandom::backward() {

}


Layer *LCropScaleRandom::share(int c, int bs, vector<Layer *> p) {
    auto *n = new LCropScaleRandom(p[0], this->factor, this->da_mode, "share_" + to_string(c) + this->name, this->dev, this->mem_level);
    n->orig = this;

    return n;
}

Layer *LCropScaleRandom::clone(int c, int bs, vector<Layer *> p, int todev) {
    auto *n = new LCropScaleRandom(p[0], this->factor, this->da_mode, "clone_" + to_string(todev) + name, todev, this->mem_level);
    n->orig = this;

    return n;
}


string LCropScaleRandom::plot(int c) {
    string s;

    if (c) s = name + " [label=" + "\"" + name + "\",style=filled,fontsize=12,fillcolor=bisque4,shape=box]";
    else s = name + " [label=" + "\"" + name + "\",style=filled,fontsize=12,fillcolor=White,shape=box]";

    return s;
}
