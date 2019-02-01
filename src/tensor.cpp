// This file is part of EDDLL an European Distributed Deep Learning Library.
// Developed within the DeepHealth project.
// Boosting AI in Europe.
//
// The MIT License (MIT)
//
// Copyright (c) 2019 Roberto Paredes Palacios, <rparedes@dsic.upv.es>

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
#include <math.h>
#include <initializer_list>
#include <vector>
#include <string>
#include <iostream>
#include "tensor.h"

#ifdef cGPU
#include "gpu/tensor_cuda.h"
#include "gpu/tensor_cuda_op.h"
#endif


using namespace std;
int initcuda[MAX_GPUS]={0,0,0,0,0,0,0,0};

void msg(string s,string s2)
{
  cout<<"\n"<<s<<s2<<"\n";
  exit(0);
}
void msg(string s){msg(s,"");}


// Tensor class
Tensor::Tensor():device(DEV_CPU),dim(0),tam(0){}

Tensor::Tensor(const initializer_list<int>& init):Tensor(init,DEV_CPU){}
Tensor::Tensor(const initializer_list<int>& init, int dev):Tensor(shape(init.begin(), init.end()),dev){}

Tensor::Tensor(const shape s):Tensor(s,DEV_CPU){}
Tensor::Tensor(shape s,int dev)
{
  #ifndef cGPU
  if ((dev>DEV_CPU)&&(dev<DEV_FPGA)){
    fprintf(stderr,"Not compiled for GPU\n");
    exit(0);
  }
  #endif
  #ifndef cFPGA
  if (dev>=DEV_FPGA){
    fprintf(stderr,"Not compiled for FPGA\n");
    exit(0);
  }
  #endif

  device=dev;
  dim=s.size();
  sizes=s;

  tam=1;
  for(int i=0;i<dim;++i) tam*=s[i];

  if (device==DEV_CPU) {
    if (dim==1) ptr1.resize(sizes[0]);
    else if (dim==2) ptr2.resize(sizes[0],sizes[1]);
    else {
      ptr=(Tensor **)malloc(sizes[0]*sizeof(Tensor *));
      s.erase(s.begin());
      for(int i=0;i<sizes[0];++i)
        ptr[i]=new Tensor(s,device);
    }
  }
  #ifdef cGPU
  else if ((dev>DEV_CPU)&&(dev<DEV_FPGA)) {
    gpu_device=device-DEV_GPU;
    if (!initcuda[gpu_device]) {
      gpu_init(gpu_device);
      initcuda[gpu_device]=1;
    }
    gpu_set_device(gpu_device);
    =gpu_create_tensor(tam);
  }
  #endif
}

///////////////////////////////////////////
Tensor *Tensor::clone()
{
  Tensor *C=new Tensor(getshape(),device);

  return C;

}

///////////////////////////////////////////
Tensor::~Tensor()
{
  if (device==DEV_CPU) {
    if (dim==1) ptr1.resize(0);
    else if (dim==2) ptr2.resize(0,0);
    else {
      for(int i=0;i<sizes[0];++i)
        delete ptr[i];
      delete ptr;
    }
  }
  #ifdef cGPU
  else if ((device>DEV_CPU)&&(device<DEV_FPGA)) {
    gpu_set_device(gpu_device);
    gpu_delete_tensor(gptr);
  }
  #endif
}


///////////////////////////////////////////
// unary operators
///////////////////////////////////////////

shape Tensor::getshape()
{
  shape s=sizes;
  return s;
}



///////////////////////////////////////////
void Tensor::info()
{
  int i;

  fprintf(stderr,"DIM=%d\n",dim);
  fprintf(stderr,"(");
  for (i = 0; i < dim-1; i++)
		fprintf(stderr,"%d,",sizes[i]);
  fprintf(stderr,"%d)\n",sizes[i]);

  fprintf(stderr,"Total bytes=%ld\n",tam*sizeof(float));
  if (device==DEV_CPU) fprintf(stderr,"Device=CPU\n");
  else if (device<DEV_FPGA) fprintf(stderr,"Device=GPU (%d)\n",gpu_device);
  else fprintf(stderr,"Device=FPGA\n");
}

///////////////////////////////////////////

void Tensor::print(){

  if (device==DEV_CPU) {
    if (dim==1) cout<<ptr1;
    else if (dim==2) cout<<ptr2;
    else
      for(int i=0;i<sizes[0];++i) {
        ptr[i]->print();
        cout<<"\n";
      }
    cout<<"\n";
  }
  else {
    if (dim<3) {
      gpu_set_device(gpu_device);
      float *v= (float*)malloc(tam*sizeof(float));
      cudaMemcpy(v,gptr,tam*sizeof(float),cudaMemcpyDeviceToHost);
      if (dim==2) {
        int i,j,p=0;
        for(i=0;i<sizes[0];++i) {
          for(j=0;j<sizes[1];++j,++p)
            printf("%f ",v[p]);
          printf("\n");
         }
       }
       else {
         int i;
         for(i=0;i<sizes[0];++i)
           printf("%f ",v[i]);
         printf("\n");
       }
       free(v);

    }

  }
}

///////////////////////////////////////////
void Tensor::set(float v){
  if (device==DEV_CPU) {
    if (dim==1)
      for(int i=0;i<sizes[0];++i) ptr1(i)=v;
    else if (dim==2)
      for(int i=0;i<sizes[0];++i) for(int j=0;j<sizes[1];++j) ptr2(i,j)=v;
    else
      for(int i=0;i<sizes[0];++i)
        Tensor::set(ptr[i],v);
  }
  #ifdef cGPU
  else{
    gpu_set_device(gpu_device);
    gpu_set(this,v);
  }
  #endif
}


///////////////////////////////////////////
void Tensor::rand(Tensor *A){
  if (device==DEV_CPU) {
    if (dim==1)
      for(int i=0;i<sizes[0];++i) ptr1(i)=(std::rand()%1000)/1000.0;
    else if (dim==2)
      for(int i=0;i<sizes[0];++i) for(int j=0;j<sizes[1];++j) ptr2(i,j)=(std::rand()%1000)/1000.0;
    else
      for(int i=0;i<sizes[0];++i)
        Tensor::rand(ptr[i]);

  }
}




///////////////////////////////////////////





























//////
