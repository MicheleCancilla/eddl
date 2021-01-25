/*
* EDDL Library - European Distributed Deep Learning Library.
* Version: 0.8
* copyright (c) 2020, Universidad Politécnica de Valencia (UPV), PRHLT Research Centre
* Date: November 2020
* Author: PRHLT Research Centre, UPV, (rparedes@prhlt.upv.es), (jon@prhlt.upv.es)
* All rights reserved
*/

#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "eddl/apis/eddl.h"


using namespace eddl;


//////////////////////////////////
// Text generation
// Only Decoder
//////////////////////////////////


int main(int argc, char **argv) {

    layer in  = Input({3, 256, 256});
    layer l=in;

    l=MaxPool(ReLu(Conv(l,2,{3,3},{1,1})),{2,2});
    l=MaxPool(ReLu(Conv(l,4,{3,3},{1,1})),{2,2});
    l=MaxPool(ReLu(Conv(l,8,{3,3},{1,1})),{2,2});
    l=MaxPool(ReLu(Conv(l,16,{3,3},{1,1})),{2,2});
    l=GlobalAveragePool(l);
    l = Flatten(l);
    l = LSTM(l, 128);
    l = Dense(l, 100);
    l = ReLu(l);
    l = Dense(l, 2);
    layer out = ReLu(l);
    model deepVO = Model({in},{out});

    build(deepVO,
          adam(),
          {"mse"},
          {"mse"},
          CS_GPU({1})
//          CS_CPU()
          );
    plot(deepVO,"model.pdf","TB");
    summary(deepVO);

    // 32 samples that are sequences of 10 RGB images of 256x256. Target 2 values per image, a sequence as well
    Tensor* seqImages = Tensor::randu({32, 10, 3, 256, 256});
    Tensor* seqLabels = Tensor::randu({32, 10, 2});
    fit(deepVO, {seqImages}, {seqLabels}, 4, 10);

    return 0;


}
