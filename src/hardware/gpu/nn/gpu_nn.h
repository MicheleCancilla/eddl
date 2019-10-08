#ifndef EDDL_GPU_NN_H
#define EDDL_GPU_NN_H

#include <cuda.h>
#include <curand.h>
#include <cuda_runtime_api.h>
#include <cublas_v2.h>

#include "../../../tensor/tensor.h"
#include "../../../descriptors/descriptors.h"

// Aux

// Activations
void gpu_relu(Tensor *A,Tensor *B);
void gpu_d_relu(Tensor *D,Tensor *I,Tensor *PD);

void gpu_softmax(Tensor *A,Tensor *B);
void gpu_d_softmax(Tensor *D,Tensor *I,Tensor *PD);

// Losses
void gpu_cent(Tensor *A,Tensor *B,Tensor *C);

// Metrics
void gpu_accuracy(Tensor *A,Tensor *B,int *acc);

// Conv
void gpu_conv2D(ConvolDescriptor *D);
void gpu_conv2D_grad(ConvolDescriptor *D);
void gpu_conv2D_back(ConvolDescriptor *D);

// Pool
void gpu_mpool2D(PoolDescriptor *D);
void gpu_mpool2D_back(PoolDescriptor *D);

// Tensor
void gpu_repeat_nn(Tensor *A, Tensor *B, vector<int> size);
void gpu_d_repeat_nn(Tensor *D, Tensor *A, vector<int> size);

#endif //EDDL_GPU_NN_H