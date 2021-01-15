/*
* EDDL Library - European Distributed Deep Learning Library.
* Version: 0.8
* copyright (c) 2020, Universidad Politécnica de Valencia (UPV), PRHLT Research Centre
* Date: November 2020
* Author: PRHLT Research Centre, UPV, (rparedes@prhlt.upv.es), (jon@prhlt.upv.es)
* All rights reserved
*/


#ifndef EDDL_LAYER_CONV_H
#define EDDL_LAYER_CONV_H

#include <string>
#include <cstdio>

#include "eddl/layers/layer.h"
#include "eddl/regularizers/regularizer.h"

#ifdef cCUDNN
#include <cudnn.h>
extern cudnnHandle_t hdnn;

#endif


#define TRMODE 1
#define TSMODE 0

using namespace std;


/// Conv2D Layer
class LConv : public LinLayer {
public:
    static int total_layers;
	bool distributed_training;

    ConvolDescriptor *cd;
#ifdef cCUDNN
    // Following cuDNN nomenclature
    cudnnHandle_t cudnn_handle;
    cudnnConvolutionMode_t convolution_mode;
    cudnnDataType_t data_type;
    cudnnTensorFormat_t tensor_format;

    cudnnConvolutionFwdAlgo_t fwd_algorithm;
    cudnnConvolutionBwdFilterAlgo_t bwd_filter_algorithm;
    cudnnConvolutionBwdDataAlgo_t bwd_weights_algorithm;
    cudnnConvolutionDescriptor_t convolution_descriptor;
    cudnnTensorDescriptor_t xDesc; //input. also dxDesc
    cudnnFilterDescriptor_t wDesc; //kernels also dwDesc
    cudnnTensorDescriptor_t yDesc; //output also dyDesc
    cudnnTensorDescriptor_t bDesc; //bias, also dbias

    int cudnn_env_init = -1;
#endif

    // constructors and clones
    LConv(Layer *parent, const vector<int> &ks, const vector<int> &st, const vector<int> &p, string name, int dev, int mem);

    LConv(Layer *parent, int filters, const vector<int> &ks, const vector<int> &st,const vector<int> &p, string name, int dev, int mem);

    LConv(Layer *parent, int filters, const vector<int> &kernel_size, const vector<int> &strides, string padding,
          int groups, const vector<int> &dilation_rate, bool use_bias, string name, int dev, int mem);

    // Destructor
    ~LConv();

    LConv(Layer *parent, ConvolDescriptor *cd, string name, int dev, int mem);

    Layer *share(int c, int bs, vector<Layer *> p) override;

    Layer *clone(int c, int bs, vector<Layer *> p, int todev) override;

    void mem_delta() override;

    // implementation
    void forward() override;

    void backward() override;

    void resize(int batch) override;

	void update_weights(Tensor* w, Tensor* bias=nullptr) override;

	void accumulate_accumulated_gradients(Tensor* gw, Tensor* gbias=nullptr) override;

	void reset_accumulated_gradients() override;

	void apply_accumulated_gradients() override;

    string plot(int c) override;

	static void reset_name_counter();

	void enable_distributed() override;

};

/// Conv1D Layer
class LConv1D : public LinLayer {
public:
    static int total_layers;
	bool distributed_training;

    Tensor* input_reshaped;
    ConvolDescriptor *cd;

    // constructors and clones
    LConv1D(Layer *parent, const vector<int> &ks, const vector<int> &st, const vector<int> &p, string name, int dev, int mem);

    LConv1D(Layer *parent, int filters, const vector<int> &ks, const vector<int> &st,const vector<int> &p, string name, int dev, int mem);

    LConv1D(Layer *parent, int filters, const vector<int> &kernel_size, const vector<int> &strides, string padding,
          int groups, const vector<int> &dilation_rate, bool use_bias, string name, int dev, int mem);

    // Destructor
    ~LConv1D();

    LConv1D(Layer *parent, ConvolDescriptor *cd, string name, int dev, int mem);

    Layer *share(int c, int bs, vector<Layer *> p) override;

    Layer *clone(int c, int bs, vector<Layer *> p, int todev) override;

    void mem_delta() override;

    // implementation
    void forward() override;

    void backward() override;

    void resize(int batch) override;

	void update_weights(Tensor* w, Tensor* bias=nullptr) override;

	void accumulate_accumulated_gradients(Tensor* gw, Tensor* gbias=nullptr) override;

	void reset_accumulated_gradients() override;

	void apply_accumulated_gradients() override;

    string plot(int c) override;

	static void reset_name_counter();

	void enable_distributed() override;

};



/// ConvT2D Layer
class LConvT : public LinLayer {
public:
    static int total_layers;
    ConvolDescriptor *cd;

    // constructors and clones
    LConvT(Layer *parent, int filters, const vector<int> &kernel_size,
           const vector<int> &output_padding, string padding, const vector<int> &dilation_rate,
           const vector<int> &strides, bool use_bias, string name, int dev, int mem);

    LConvT(Layer *parent, ConvolDescriptor *cd, string name, int dev, int mem);

    // Destructor
    ~LConvT();

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
    LUpSampling(Layer *parent, const vector<int> &size, string interpolation, string name, int dev, int mem);

    Layer *share(int c, int bs, vector<Layer *> p) override;

    Layer *clone(int c, int bs, vector<Layer *> p, int todev) override;

    // Params are in ConvolDescriptor

    // implementation
    void forward() override;

    void backward() override;

    string plot(int c) override;

};

#endif //EDDL_LAYER_CONV_H
