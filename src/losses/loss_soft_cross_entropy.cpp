/*
* EDDL Library - European Distributed Deep Learning Library.
* Version: 0.7
* copyright (c) 2020, Universidad Politécnica de Valencia (UPV), PRHLT Research Centre
* Date: April 2020
* Author: PRHLT Research Centre, UPV, (rparedes@prhlt.upv.es), (jon@prhlt.upv.es)
* All rights reserved
*/


#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "eddl/losses/loss.h"

using namespace std;


LSoftCrossEntropy::LSoftCrossEntropy() : Loss("soft_cross_entropy"){}


void LSoftCrossEntropy::delta(Tensor *T, Tensor *Y, Tensor *D) {
    Tensor::add(-1.0, T, 1.0, Y, D, 0);
    D->div_(D->shape[0]);
}

float LSoftCrossEntropy::value(Tensor *T, Tensor *Y) {
    float f;
    Tensor *aux1;
    int size=T->size/T->shape[0];  // batch is divided in print_loss

    aux1 = new Tensor(T->getShape(), T->device);
    tensorNN::cent(T, Y, aux1);
    //f = aux1->sum()/size;
    // corss-entropy use to be used with one-hot vectors
    // dividing by size of vector gives too small values when
    // for instance nuber of classes is too large.
    f = aux1->sum();

    delete aux1;

    return f;
}
Loss* LSoftCrossEntropy::clone()
{
  return new LSoftCrossEntropy();
}
