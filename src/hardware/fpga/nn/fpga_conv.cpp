/*
* FPGA support for EDDL Library - European Distributed Deep Learning Library.
* Version: 0.6
* copyright (c) 2020, Universidad Politécnica de Valencia (UPV), GAP research group
* Date: June 2020
* Author: GAP Research Group (UPV), contact: carlherlu@gap.upv.es, jflich@disca.upv.es
* All rights reserved
*/

#ifdef cFPGA

#include <cstdio>      /* printf, scanf, NULL */
#include <cstdlib>     /* malloc, free, rand */
#include <iostream>

#include "eddl/hardware/fpga/nn/fpga_nn.h"
#include "eddl/hardware/fpga/fpga_hw.h"
#include "eddl/hardware/cpu/nn/cpu_tensor_nn.h"

// emulation switches of functions (via cpu)
// when set the function is run on the cpu
char fpga_set_cpuemu_conv2D      = 1;
char fpga_set_cpuemu_conv2D_grad = 1;
char fpga_set_cpuemu_conv2D_back = 1;

// -----------------------------------------------------------------
// conv2D
//
//

void fpga_cpuemu_conv2D(ConvolDescriptor *D) {
  fpga_copy_from_fpga(D->K, D->K->ptr);
  fpga_copy_from_fpga(D->bias, D->bias->ptr);
  fpga_copy_from_fpga(D->I, D->I->ptr);
  cpu_conv2D(D);
  fpga_copy_to_fpga(D->O->ptr, D->O);
  fpga_copy_memory_to_fpga(D->ptrI, D->fpga_ptrI, D->fpga_sizeI);
}

// Convolution: Kernel(3x3), Stride(1x1), Padding(1x1), BatchSize=1
void fpga_conv2D_K3x3_S1x1_P1x1_BS1(cl::Buffer I, int Irows, int Icols, int Ichannels, cl::Buffer K, cl::Buffer B, cl::Buffer O, int Ochannels) {

 cl_int err;
 cl::Event event;
 int arg=0;
 OCL_CHECK(err, err = kernel_conv2D_K3x3_S1x1_P1x1_BS1.setArg(arg++, I));
 OCL_CHECK(err, err = kernel_conv2D_K3x3_S1x1_P1x1_BS1.setArg(arg++, Irows));
 OCL_CHECK(err, err = kernel_conv2D_K3x3_S1x1_P1x1_BS1.setArg(arg++, Icols));
 OCL_CHECK(err, err = kernel_conv2D_K3x3_S1x1_P1x1_BS1.setArg(arg++, Ichannels));
 OCL_CHECK(err, err = kernel_conv2D_K3x3_S1x1_P1x1_BS1.setArg(arg++, K));
 OCL_CHECK(err, err = kernel_conv2D_K3x3_S1x1_P1x1_BS1.setArg(arg++, B));
 OCL_CHECK(err, err = kernel_conv2D_K3x3_S1x1_P1x1_BS1.setArg(arg++, O));
 OCL_CHECK(err, err = kernel_conv2D_K3x3_S1x1_P1x1_BS1.setArg(arg++, Ochannels));

 OCL_CHECK(err, err = q.enqueueTask(kernel_conv2D_K3x3_S1x1_P1x1_BS1, NULL, &event));
 q.finish();
}

void fpga_conv2D(ConvolDescriptor *D)
{
  _profile_fpga(_FPGA_CONV2D, 0);
#if !defined(K_ENABLED_CONV2D) && !defined(K_ENABLED_CONV2D_K3X3_S1X1_P1X1_BS1)
  fpga_cpuemu_conv2D(D);
#else
  cl_int err;
  cl::Event event;

  // conv2D parameters
  int batch_size   = D->I->shape[0];     // batch size
  cl::Buffer I     = *D->I->fpga_ptr;    // input activations
  int Irows        = D->I->shape[2];     // rows of input image
  int Icols        = D->I->shape[3];     // cols of input image
  int Ichannels    = D->I->shape[1];     // input channels
  cl::Buffer K     = *D->K->fpga_ptr;    // kernel
  int Krows        = D->kr;              // kernel rows
  int Kcols        = D->kc;              // kernel cols
  cl::Buffer B     = *D->bias->fpga_ptr; // bias
  int use_bias     = D->use_bias;        // whether use bias or not
  cl::Buffer O     = *D->O->fpga_ptr;    // output activations
  int Orows        = D->O->shape[2];     // rows of output images
  int Ocols        = D->O->shape[3];     // cols of output images
  int Ochannels    = D->O->shape[1];     // output channels
  int padding_rows = D->padrt;           // padding rows (for top and for bottom)
  int padding_cols = D->padcl;           // padding cols (for left and right)
  int stride_rows  = D->sr;              // rows stride
  int stride_cols  = D->sc;              // cols stride

  // depending on the conv parameters we select the kernel to launch
  if ((stride_rows == 1) && (stride_cols == 1) && (Krows == 3) && (Kcols == 3) && (batch_size == 1) && (padding_rows == 1) && (padding_cols == 1)) {
    fpga_conv2D_K3x3_S1x1_P1x1_BS1(I, Irows, Icols, Ichannels, K, B, O, Ochannels);
  } else {
    OCL_CHECK(err, err = kernel_conv2d.setArg(0, batch_size));
    OCL_CHECK(err, err = kernel_conv2d.setArg(1, I));
    OCL_CHECK(err, err = kernel_conv2d.setArg(2, Irows));    // input
    OCL_CHECK(err, err = kernel_conv2d.setArg(3, Icols));    // output
    OCL_CHECK(err, err = kernel_conv2d.setArg(4, Ichannels));
    OCL_CHECK(err, err = kernel_conv2d.setArg(5, K));
    OCL_CHECK(err, err = kernel_conv2d.setArg(6, Krows));
    OCL_CHECK(err, err = kernel_conv2d.setArg(7, Kcols));
    OCL_CHECK(err, err = kernel_conv2d.setArg(8, B));
    OCL_CHECK(err, err = kernel_conv2d.setArg(9, use_bias));
    OCL_CHECK(err, err = kernel_conv2d.setArg(10, O));
    OCL_CHECK(err, err = kernel_conv2d.setArg(11, Orows));
    OCL_CHECK(err, err = kernel_conv2d.setArg(12, Ocols));
    OCL_CHECK(err, err = kernel_conv2d.setArg(13, Ochannels));
    OCL_CHECK(err, err = kernel_conv2d.setArg(14, padding_rows));
    OCL_CHECK(err, err = kernel_conv2d.setArg(15, padding_cols));
    OCL_CHECK(err, err = kernel_conv2d.setArg(16, stride_rows));
    OCL_CHECK(err, err = kernel_conv2d.setArg(17, stride_cols));

    OCL_CHECK(err, err = q.enqueueTask(kernel_conv2d, NULL, &event));
    q.finish();
  }
#endif
  _profile_fpga(_FPGA_CONV2D, 1);
}

// -----------------------------------------------------------------
// conv2D_grad
//
void fpga_cpuemu_conv2D_grad(ConvolDescriptor *D) {
  fpga_copy_from_fpga(D->D, D->D->ptr);
  fpga_copy_memory_from_fpga(D->fpga_ptrI, D->ptrI, D->fpga_sizeI);
  fpga_copy_from_fpga(D->gK, D->gK->ptr);
  fpga_copy_from_fpga(D->gbias, D->gbias->ptr);
  cpu_conv2D_grad(D);
  fpga_copy_to_fpga(D->gK->ptr, D->gK);
  fpga_copy_to_fpga(D->gbias->ptr, D->gbias);
}

void fpga_conv2D_grad(ConvolDescriptor *D)
{
  _profile_fpga(_FPGA_CONV2D_GRAD, 0);
  if (fpga_set_cpuemu_conv2D_grad == 1) {
    fpga_cpuemu_conv2D_grad(D);
  } else {
      printf("fpga_conv2D_grad not implemented yet\n"); exit(1);
  }
  _profile_fpga(_FPGA_CONV2D_GRAD, 1);
}

// -----------------------------------------------------------------
// conv2D_back
//
void fpga_cpuemu_conv2D_back(ConvolDescriptor *D) {
  fpga_copy_from_fpga(D->D, D->D->ptr);
  fpga_copy_memory_from_fpga(D->fpga_ptrI, D->ptrI, D->fpga_sizeI);
  fpga_copy_from_fpga(D->K, D->K->ptr);
  cpu_conv2D_back(D);
  fpga_copy_to_fpga(D->ID->ptr, D->ID);
}

void fpga_conv2D_back(ConvolDescriptor *D)
{
  _profile_fpga(_FPGA_CONV2D_BACK, 0);
  if (fpga_set_cpuemu_conv2D_back == 1) {
    fpga_cpuemu_conv2D_back(D);
  } else {
      printf("fpga_conv2D_back not implemented yet\n"); exit(1);
  }
  _profile_fpga(_FPGA_CONV2D_BACK, 1);
}

#endif
