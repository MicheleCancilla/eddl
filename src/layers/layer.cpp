/*
* EDDL Library - European Distributed Deep Learning Library.
* Version: 0.1
* copyright (c) 2019, Universidad Politécnica de Valencia (UPV), PRHLT Research Centre
* Date: October 2019
* Author: PRHLT Research Centre, UPV, (rparedes@prhlt.upv.es), (jon@prhlt.upv.es)
* All rights reserved
*/


#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "layer.h"
#include "../tensor/tensor.h"
#include "operators/layer_operators.h"

using namespace std;


////////////////////////////////////
///// BASE LAYER CLASS
////////////////////////////////////

Layer::Layer(string name, int dev) {
    mode = TRMODE;
    target = delta = input = output = nullptr;
    this->name = name;
    this->dev = dev;
    lin = lout = 0;
    delta_bp = 0;
    isplot=true;
    inner=false;
}

Layer::~Layer()
{
  cout<<"OKL1\n";
  if (input!=nullptr) delete input;
  cout<<"OKL2\n";
  if (output!=nullptr) delete output;
  cout<<"OKL3\n";
  if (delta!=nullptr) delete delta;
  cout<<"OKL4\n";
  if (target!=nullptr) delete target;
  cout<<"OKL5\n";
}

void Layer::initialize() {

    for (int i = 0; i != params.size(); i++) {
        if (params[i]->ndim == 1)
            params[i]->rand_signed_uniform(0.1);
        else if (params[i]->ndim == 2)
            params[i]->rand_normal(0.0, sqrt(2.0 / params[i]->shape[0]));
        else
            params[i]->rand_normal(0.0, sqrt(2.0 / (params[i]->size / params[i]->shape[0])));
    }
}


void Layer::resize(int batch)
{
     if (output!=nullptr) output->resize(batch);
     if (delta!=nullptr) delta->resize(batch);
     if (target!=nullptr) target->resize(batch);
}

void Layer::detach(Layer *l)
{
  for(int i=0;i<child.size();i++)
    if(child[i]==l) {
      child.erase(child.begin() + i);
      lout--;
    }
}

void Layer::reset() {
    delta->set(0.0);
}

void Layer::setmode(int m) {

    mode = m;
}

vector<int> Layer::getShape()
{
  return output->getShape();
}

void Layer::save(FILE *fe)
{
  for (int i = 0; i != params.size(); i++)
    params[i]->save(fe);
}

void Layer::load(FILE *fe)
{
  for (int i = 0; i != params.size(); i++)
    params[i]->load(fe);
}

void Layer::info() {
    cout << "\n===============\n";
    cout << "Layer " << name << "\n";
    if (parent.size()) {
        cout << "Parent layers:\n";
        for (int i = 0; i < parent.size(); i++)
            cout << parent[i]->name << "\n";
    } else cout << "No parent layers\n";

    if (child.size()) {
        cout << "Child layers:\n";
        for (int i = 0; i != child.size(); i++)
            cout << child[i]->name << "\n";
    } else cout << "No child layers\n";

    cout << "Input tensor:\n";
    input->info();

    if (params.size()) {
        cout << "Params:\n";
        for (int i = 0; i < params.size(); i++)
            params[i]->info();
    } else cout << "No params\n";

    cout << "Output tensor:\n";
    output->info();
    cout << "===============\n\n";
}

Tensor* Layer::getWeights(){
    return nullptr;
}

Tensor* Layer::setWeights(Tensor bias){
    return nullptr;
}

Tensor* Layer::getBias(){
    return nullptr;
}

Tensor* Layer::setBias(Tensor bias){
    return nullptr;
}

////////////////////////////////////
///// LINEAR LAYERS
////////////////////////////////////
LinLayer::LinLayer(string name, int dev) : Layer(name, dev) {}

void LinLayer::addchild(Layer *l) {
    child.push_back(l);
    if (!l->inner) lout++;
}

void LinLayer::addparent(Layer *l) {
    if (parent.size() != 0) msg("This layers only can have one parent layer", l->name.c_str());
    parent.push_back(l);
    lin++;
}


////////////////////////////////////
///// Multiple LAYERS
////////////////////////////////////
MLayer::MLayer(string name, int dev) : Layer(name, dev) {}

void MLayer::addchild(Layer *l) {
    child.push_back(l);
    if (!l->inner) lout++;
}


void MLayer::addparent(Layer *l) {
    parent.push_back(l);
    lin++;
}


///////////////////////////////////////
/// OP OVERLOAD
Layer* operator+(Layer &l1,Layer &l2) {

    return  new LSum(&l1, &l2,"",l1.dev);

}
Layer* operator+(Layer &l,float f)
{

  return new LSum(&l, f,"",l.dev);

}

Layer* operator+(float f,Layer &l)
{

  return new LSum(&l, f,"",l.dev);

}

Layer* operator*(Layer &l1,Layer &l2) {

    return  new LMult(&l1, &l2,"",l1.dev);

}
Layer* operator*(Layer &l,float f)
{

  return new LMult(&l, f,"",l.dev);

}

Layer* operator*(float f,Layer &l)
{

  return new LMult(&l, f,"",l.dev);

}
