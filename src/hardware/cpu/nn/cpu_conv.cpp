/*
* EDDL Library - European Distributed Deep Learning Library.
* Version: 0.9
* copyright (c) 2020, Universidad Politécnica de Valencia (UPV), PRHLT Research Centre
* Date: November 2020
* Author: PRHLT Research Centre, UPV, (rparedes@prhlt.upv.es), (jon@prhlt.upv.es)
* All rights reserved
*/

#include <cstdio>      /* printf, scanf, NULL */
#include <cstdlib>     /* malloc, free, rand */
#include <iostream>

#include "eddl/hardware/cpu/nn/cpu_tensor_nn.h"

#define VERBOSE 0

float get_pixel(int b,int px,int py,int pz,ConvolDescriptor *D,int isize,int irsize) {

  if (VERBOSE)
    cout<<"pixel: "<<pz<<" "<<py<<" "<<px<<endl;
  //getchar();
  
  // Check boundaries of the window
  if (px<0) return 0.0;
  if (py<0) return 0.0;
  if (px>=D->ic) return 0.0;
  if (py>=D->ir) return 0.0;

  // Compute address from indices (row-major)
  unsigned int address = (b*isize) + (pz*irsize) + (py*D->ic) + px;
  return D->I->ptr[address];
}

void add_pixel(int b,int px,int py,int pz,ConvolDescriptor *D,int isize,int irsize,float val) {
  // Check boundaries of the window
  if (px<0) return;
  if (py<0) return;
  if (px>=D->ic) return;
  if (py>=D->ir) return;

  // Compute address from indices (row-major)
  unsigned int address = (b*isize) + (pz*irsize) + (py*D->ic) + px;
  D->ID->ptr[address]+=val;
}


void im2col(int b,ConvolDescriptor *D,float *ptrI,int col2im)
{
  _profile(_CPU_IM2COL, 0);
  int i,j,k;
  int pz,py,px,y,x;
  int ksize=D->kr*D->kc;
  int kr2=D->kr/2;
  int kc2=D->kc/2;

  if (kc2==0) kc2=-1;

  int orsize=D->r*D->c;

  int isize=D->ir*D->ic*D->iz;
  int irsize=D->ir*D->ic;

  k=0;
  py=-D->padrt;
  px=-D->padcl;


  for(j=0;j<D->matI.rows();j++) {
    k=j;

    if (VERBOSE){
    cout<<"======================"<<endl;
    cout<<j<<" "<<j/D->c<<" "<<j%D->c<<endl;
    cout<<"======================"<<endl;
    }
    if ((j!=0)&&((j%D->c)==0)) {
       if (VERBOSE) cout<<"change row"<<endl; 
      px=-D->padcl;
      py+=D->sr;
    }

    
    for(i=0;i<D->matI.cols();i++,k+=orsize) {
      pz=i/ksize;
      y=py+(i%ksize)/D->kc;
      x=px+(i%D->kc);

      if(col2im)
      add_pixel(b,x,y,pz,D,isize,irsize,ptrI[k]);
      else
      ptrI[k]=get_pixel(b,x,y,pz,D,isize,irsize);

    }
    px+=D->sc;
  if (VERBOSE)    getchar();
  }
    _profile(_CPU_IM2COL, 1);

    if (VERBOSE){
    getchar();
    for(int i=0;i<100;i++) cout<<ptrI[i]<<" ";

    getchar();
    }

}


void cpu_conv2D(ConvolDescriptor *D)
{
  _profile(_CPU_CONV2D, 0);
  int osize=D->z*D->r*D->c;
  int isize=D->r*D->c*D->kc*D->kr*D->kz;//r*c,kr*kc*kz

  float *ptrO=D->O->ptr;
  float *ptrI=D->ptrI;


  // Map memory to Eigen
  Eigen::Map<Eigen::MatrixXf> matK=Eigen::Map<Eigen::MatrixXf>(D->K->ptr, D->kr * D->kc * D->kz, D->nk);

  #pragma omp parallel for
  for(int b=0;b<D->I->shape[0];b++){

    float *ptrO=D->O->ptr+(b*osize);
    float *ptrI=D->ptrI+(b*isize);

    Eigen::Map<Eigen::MatrixXf> matI=Eigen::Map<Eigen::MatrixXf>(ptrI,D->r*D->c,D->kz*D->kr*D->kc);
    Eigen::Map<Eigen::MatrixXf> matO=Eigen::Map<Eigen::MatrixXf>(ptrO,D->r*D->c,D->z);

    im2col(b,D,ptrI,0);

    matO=matI*matK;
  }// batch

  //bias
  if (D->use_bias) {
    #pragma omp parallel for
    for(int b=0;b<D->O->shape[0];b++) {
      float *ptrO=D->O->ptr+(b*osize);
      for(int z=0;z<D->O->shape[1];z++)
      for(int r=0;r<D->O->shape[2];r++)
      for(int c=0;c<D->O->shape[3];c++,ptrO++)
      (*ptrO)+=D->bias->ptr[z];
    }
  }
    _profile(_CPU_CONV2D, 1);

}

void cpu_conv2D_grad(ConvolDescriptor *D)
{
  _profile(_CPU_CONV2D_GRAD, 0);
  //return;
  int osize=D->z*D->r*D->c;
  int isize=D->r*D->c*D->kc*D->kr*D->kz;//r*c,kr*kc*kz


  // Map memory to Eigen
  Eigen::Map<Eigen::MatrixXf> matgK=Eigen::Map<Eigen::MatrixXf>(D->gK->ptr, D->kr * D->kc * D->kz, D->nk);

  //#pragma omp parallel for
  for(int b=0;b<D->I->shape[0];b++){

    float *ptrD=D->D->ptr+(b*osize);
    float *ptrI=D->ptrI+(b*isize);

    Eigen::Map<Eigen::MatrixXf> matI=Eigen::Map<Eigen::MatrixXf>(ptrI,D->r*D->c,D->kz*D->kr*D->kc);
    Eigen::Map<Eigen::MatrixXf> matD=Eigen::Map<Eigen::MatrixXf>(ptrD,D->r*D->c,D->z);

    matgK+=matI.transpose()*matD;
  }// batch

  //bias

  //#pragma omp parallel for
  if (D->use_bias) {
    for(int b=0;b<D->D->shape[0];b++) {
      float *ptrD=D->D->ptr+(b*osize);
      for(int z=0;z<D->D->shape[1];z++)
      for(int r=0;r<D->D->shape[2];r++)
      for(int c=0;c<D->D->shape[3];c++,ptrD++)
      D->gbias->ptr[z]+=(*ptrD);

    }
  }
    _profile(_CPU_CONV2D_GRAD, 1);
}

void cpu_conv2D_back(ConvolDescriptor *D)
{
  _profile(_CPU_CONV2D_BACK, 0);
  int osize=D->z*D->r*D->c;
  int isize=D->r*D->c*D->kc*D->kr*D->kz;//r*c,kr*kc*kz

  float *ptrD=D->D->ptr;
  float *ptrI=D->ptrI;

  // Map memory to Eigen
  Eigen::Map<Eigen::MatrixXf> matK=Eigen::Map<Eigen::MatrixXf>(D->K->ptr, D->kr * D->kc * D->kz, D->nk);

  #pragma omp parallel for
  for(int b=0;b<D->I->shape[0];b++){

    float *ptrD=D->D->ptr+(b*osize);
    float *ptrI=D->ptrI+(b*isize);

    Eigen::Map<Eigen::MatrixXf> matI=Eigen::Map<Eigen::MatrixXf>(ptrI,D->r*D->c,D->kz*D->kr*D->kc);
    Eigen::Map<Eigen::MatrixXf> matD=Eigen::Map<Eigen::MatrixXf>(ptrD,D->r*D->c,D->z);

    matI=matD*matK.transpose();

    im2col(b,D,ptrI,1);

  }// batch
    _profile(_CPU_CONV2D_BACK, 1);
}

inline void naive_image_conv2D(int image_rows, int image_cols, float *image,
        int kernel_rows, int kernel_cols, float *kernel,
        int out_rows, int out_cols, float *output,
        int pad_row, int pad_col, int stride_rows, int stride_cols)
{
    // int dx = kernel_rows / 2;
    // int dy = kernel_cols / 2;

    // int out_rows = image_rows - kernel_cols + 1 + pad_left + pad_right;
    // int out_cols = image_cols - kernel_rows + 1 + pad_top + pad_bottom;
    // printf("out_rows=%d out_cols=%d\n", out_rows, out_cols);

    for (int i = 0; i < out_rows; i++) {
        for (int j = 0; j < out_cols; j++) {
            float s = 0;
            // printf("[%d,%d]\n", i, j);
            for (int x = 0; x < kernel_rows; x++) {
                int px = i * stride_rows + x - pad_row;
                if (px >= 0 && px < image_rows) {
                    for (int y = 0; y < kernel_cols; y++) {
                        int py = j * stride_cols + y - pad_col;
                        if (py >= 0 && py < image_cols) {
                        // printf("    k[%d,%d]=%e i[%d,%d]=%e\n",
                        //         x, y, kernel[dx-x + (dy-y) * kernel_rows],
                        //         px, py, image[px + py * image_rows]);
                            s = s + kernel[x * kernel_cols + y] * image[px * image_cols + py];
                        }
                    }
                }
            }
            output[i * out_cols + j] += s;
        }
    }
}

void cpu_naive_conv2D(ConvolDescriptor *D, float *output)
{
    // printf("input: shape[0]=%d, in=%d\n", D->I->shape[0], D->in);
    /* printf("input: in=%d, iz=%d, ir=%d, ic=%d\n", D->I->shape[0], D->iz, D->ir, D->ic);
    printf("kernel: nk=%d, kz (iz)=%d, kr=%d, kc=%d\n", D->nk, D->kz, D->kr, D->kc);
    printf("output: in=%d, z (nk)=%d, r=%d, c=%d\n", D->in, D->z, D->r, D->c);
    printf("padcl=%d, padcr=%d, padrt=%d, padrb=%d\n", D->padcl, D->padcr, D->padrt, D->padrb);
    printf("stride: %d,%d bias:%d\n", D->sr, D->sc, D->use_bias); */
    float *ptrO = output;
    // #pragma omp parallel for // collapse(2) doesn't work
    for (int b = 0; b < D->I->shape[0]; b++) { // batch
        for (int k = 0; k < D->nk; k++) { // kernel
            memset(ptrO, 0, D->r * D->c * sizeof(float));
            for (int z = 0; z < D->iz; z++) { // canal
                // printf("b=%d k=%d z=%d\n", b, k, z);
                // printf("ptrO=%p ptrI=%p ptrK=%p\n", ptrO, ptrI, ptrK);
                naive_image_conv2D(D->ir, D->ic,
                    D->I->ptr + (b * D->iz + z) * D->ir * D->ic,
                    D->kr, D->kc,
                    D->K->ptr + (k * D->kz + z) * D->kr * D->kc,
                    D->r, D->c, ptrO,
                    D->padrt, D->padcl, D->sr, D->sc);
            }
            ptrO += D->r * D->c;
        }
    }

    //bias
    if (D->use_bias) {
        // for(int z = 0; z < D->O->shape[1]; z++) printf("%e ", D->bias->ptr[z]);
        // printf("\n");
        int osize = D->z * D->r * D->c;
        // #pragma omp parallel for
        for(int b = 0; b< D->O->shape[0]; b++) {
            float *ptrO = output + (b * osize);
            for(int z = 0; z < D->O->shape[1]; z++)
                for(int r = 0; r < D->O->shape[2]; r++)
                    for(int c = 0; c < D->O->shape[3]; c++, ptrO++)
                        (*ptrO) += D->bias->ptr[z];
        }
    }
}

inline void naive_image_conv2D_back(int image_rows, int image_cols, float *image,
        int kernel_rows, int kernel_cols, float *kernel,
        int out_rows, int out_cols, float *output,
        int pad_row, int pad_col, int stride_rows, int stride_cols)
{
    int dx = kernel_rows / 2;
    int dy = kernel_cols / 2;

    // int out_rows = image_rows - kernel_cols + 1 + pad_left + pad_right;
    // int out_cols = image_cols - kernel_rows + 1 + pad_top + pad_bottom;
    // printf("out_rows=%d out_cols=%d\n", out_rows, out_cols);

    float *t = new float[out_rows * out_cols];
    memset(t, 0, out_rows * out_cols * sizeof(float));
    for (int i = 0; i < out_rows; i++) {
        for (int j = 0; j < out_cols; j++) {
            int px = i - pad_row;
            int py = j - pad_col;
            if (px % stride_rows) continue;
            if (py % stride_cols) continue;
            px /= stride_cols;
            py /= stride_cols;
            if (px >= 0 && px < image_rows &&
                py >= 0 && py < image_cols) {
                    t[i * out_cols + j] = image[px * image_cols + py];
            }
        }
    }
    for (int i = 0; i < out_rows; i++) {
        for (int j = 0; j < out_cols; j++) {
            printf("% 6.1g ", t[i * out_cols + j]);
        }
        printf("\n");
    }

    for (int i = 0; i < out_rows; i++) {
        for (int j = 0; j < out_cols; j++) {
            float s = 0;
            // printf("[%d,%d]\n", i, j);
            for (int x = 0; x < kernel_rows; x++) {
                for (int y = 0; y < kernel_cols; y++) {
                    int px = i - pad_row + x - dx;
                    int py = j - pad_col + y - dy;
                    if (px % stride_rows) continue;
                    if (py % stride_cols) continue;
                    px /= stride_cols;
                    py /= stride_cols;
                    if (px >= 0 && px < image_rows &&
                        py >= 0 && py < image_cols) {
                    // printf("    k[%d,%d]=%e i[%d,%d]=%e\n",
                             // x - dx, y - dx,
                             // kernel[(kernel_rows - 1 - x) * kernel_cols + kernel_cols - 1 - y],
                             // px, py, image[px + py * image_rows]);
                        s += kernel[(kernel_rows - 1 - x) * kernel_cols +
                                     kernel_cols - 1 - y]
                           * image[px * image_cols + py];
                    }
                }
            }
            output[i * out_cols + j] += s;
        }
    }
    delete(t);
}

void cpu_naive_conv2D_back(ConvolDescriptor *D, float *output)
{
#if 0
    /* printf("input: in=%d, iz=%d, ir=%d, ic=%d\n", D->I->shape[0], D->iz, D->ir, D->ic);
    printf("kernel: nk=%d, kz (iz)=%d, kr=%d, kc=%d\n", D->nk, D->kz, D->kr, D->kc);
    printf("output: in=%d, z (nk)=%d, r=%d, c=%d\n", D->in, D->z, D->r, D->c);
    printf("padcl=%d, padcr=%d, padrt=%d, padrb=%d\n", D->padcl, D->padcr, D->padrt, D->padrb);
    printf("padding: %d %d %d %d\n", D->pads[0], D->pads[1], D->pads[2], D->pads[3]);
    printf("stride: %d,%d bias:%d\n", D->sr, D->sc, D->use_bias); */
    float *ptrID = output;
    int pad_row = D->padrt == 0 ? D->kr / 2 : 0;
    int pad_col = D->padcl == 0 ? D->kc / 2 : 0;
    // #pragma omp parallel for // collapse(2) doesn't work
    for (int b = 0; b < D->I->shape[0]; b++) { // batch
        for (int k = 0; k < D->nk; k++) { // kernel
            memset(ptrID, 0, D->ir * D->ic * sizeof(float));
            for (int z = 0; z < D->iz; z++) { // canal
                // printf("b=%d k=%d z=%d\n", b, k, z);
                // printf("ptrO=%p ptrI=%p ptrK=%p\n", ptrO, ptrI, ptrK);
                naive_image_conv2D_back(D->r, D->c,
                    D->D->ptr + (b * D->iz + z) * D->r * D->c,
                    D->kr, D->kc,
                    D->K->ptr + (k * D->kz + z) * D->kr * D->kc,
                    D->ir, D->ic, ptrID,
                    pad_row, pad_col,
                    D->sr, D->sc);
            }
            ptrID += D->ir * D->ic;
        }
    }
#else
    int n = D->I->shape[0] * D->iz * D->ir * D->ic;
    memset(output, 0, n * sizeof(float));
    int osize=D->z*D->r*D->c;
    int isize=D->r*D->c*D->kc*D->kr*D->kz;//r*c,kr*kc*kz

    float *ptrD=D->D->ptr;
    float *ptrI=D->ptrI;

    // Map memory to Eigen
    Eigen::Map<Eigen::MatrixXf> matK=Eigen::Map<Eigen::MatrixXf>(D->K->ptr, D->kr * D->kc * D->kz, D->nk);

    #pragma omp parallel for
    for(int b=0;b<D->I->shape[0];b++){

        float *ptrD=D->D->ptr+(b*osize);
        float *ptrI=D->ptrI+(b*isize);

        Eigen::Map<Eigen::MatrixXf> matI=Eigen::Map<Eigen::MatrixXf>(ptrI,D->r*D->c,D->kz*D->kr*D->kc);
        Eigen::Map<Eigen::MatrixXf> matD=Eigen::Map<Eigen::MatrixXf>(ptrD,D->r*D->c,D->z);

        matI=matD*matK.transpose();

    // im2col(b,D,ptrI,1);
    // void im2col(int b,ConvolDescriptor *D,float *ptrI,int col2im)
    {

        int i,j,k;
        int pz,py,px,y,x;
        int ksize=D->kr*D->kc;
        int kr2=D->kr/2;
        int kc2=D->kc/2;

        if (kc2==0) kc2=-1;

        int orsize=D->r*D->c;

        int isize=D->ir*D->ic*D->iz;
        int irsize=D->ir*D->ic;

        k=0;
        py=-D->padrt;
        px=-D->padcl;

        for(j=0;j<D->matI.rows();j++) {
            k=j;
            if ((j!=0)&&((j%D->c)==0)) {
                px=-D->padcl;
                py+=D->sr;
            }
            for(i=0;i<D->matI.cols();i++,k+=orsize) {
                pz=i/ksize;
                y=py+(i%ksize)/D->kc;
                x=px+(i%D->kc);
                // add_pixel(b,x,y,pz,D,isize,irsize,ptrI[k]);
                // void add_pixel(int b,int px,int py,int pz,ConvolDescriptor *D,int isize,int irsize,float val)
                // Check boundaries of the window
                if (x<0) continue;
                if (y<0) continue;
                if (x>=D->ic) continue;
                if (y>=D->ir) continue;

                // Compute address from indices (row-major)
                unsigned int address = (b*isize) + (pz*irsize) + (y*D->ic) + x;
                // D->ID->ptr[address]+=val;
                output[address]+=ptrI[k];
            }
            px+=D->sc;
        }
    }
  }// batch
#endif
}


void cpu_conv3D(ConvolDescriptor3D *D){

}

void cpu_conv3D_grad(ConvolDescriptor3D *D){

}

void cpu_conv3D_back(ConvolDescriptor3D *D){

}
