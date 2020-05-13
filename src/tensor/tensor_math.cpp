/*
* EDDL Library - European Distributed Deep Learning Library.
* Version: 0.5
* copyright (c) 2020, Universidad Politécnica de Valencia (UPV), PRHLT Research Centre
* Date: April 2020
* Author: PRHLT Research Centre, UPV, (rparedes@prhlt.upv.es), (jon@prhlt.upv.es)
* All rights reserved
*/

#include <cmath>
#include <limits>
#include <iostream>

#include "eddl/tensor/tensor.h"
#include "eddl/hardware/cpu/cpu_hw.h"

#ifdef cGPU
#include "eddl/hardware/gpu/gpu_tensor.h"
#include "eddl/hardware/gpu/gpu_hw.h"
#include "eddl/hardware/gpu/nn/gpu_nn.h"
#endif


using namespace std;

// Math operations (zero) ************************
static float max(Tensor* A){
    if (A->isCPU()) {
        return cpu_max(A);
    }
#ifdef cGPU
    else if (A->isGPU())
    {
        return gpu_max(A);
    }
#endif
#ifdef cFPGA
    else {

    }
#endif

    msg("Invalid device", "Tensor::max");
    return 0.0f; // Never used, this is for the compiler warning
}

static float min(Tensor* A){
    if (A->isCPU()) {
        return cpu_min(A);
    }
#ifdef cGPU
    else if (A->isGPU())
    {
        return gpu_min(A);
    }
#endif
#ifdef cFPGA
    else {

    }
#endif

    msg("Invalid device", "Tensor::max");
    return 0.0f; // Never used, this is for the compiler warning
}

static float sum(Tensor* A){
    if (A->isCPU()) {
        return cpu_sum(A);
    }
#ifdef cGPU
    else if (A->isGPU())
    {
        return gpu_sum(A);
    }
#endif
#ifdef cFPGA
    else {

    }
#endif

    msg("Invalid device", "Tensor::max");
    return 0.0f; // Never used, this is for the compiler warning
}


static float sum_abs(Tensor* A){
    if (A->isCPU()) {
        return cpu_sum_abs(A);
    }
#ifdef cGPU
    else if (A->isGPU())
    {
        return gpu_sum_abs(A);
    }
#endif
#ifdef cFPGA
    else {

    }
#endif

    msg("Invalid device", "Tensor::max");
    return 0.0f; // Never used, this is for the compiler warning
}


void Tensor::abs(Tensor *A, Tensor *B){
    if (A->isCPU() && B->isCPU()) {
        cpu_abs(A, B);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        gpu_abs(A, B);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::acos(Tensor *A, Tensor *B){
    if (A->isCPU() && B->isCPU()) {
        cpu_acos(A, B);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        gpu_acos(A, B);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::add(Tensor *A, Tensor *B, float v){
    if (A->isCPU() && B->isCPU()) {
        cpu_add(A, B, v);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        cpu_add(A, B, v);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::asin(Tensor *A, Tensor *B){
    if (A->isCPU() && B->isCPU()) {
        cpu_asin(A, B);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        gpu_asin(A, B);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::atan(Tensor *A, Tensor *B){
    if (A->isCPU() && B->isCPU()) {
        cpu_atan(A, B);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        gpu_atan(A, B);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::ceil(Tensor *A, Tensor *B){
    if (A->isCPU() && B->isCPU()) {
        cpu_ceil(A, B);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        gpu_ceil(A, B);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::clamp(Tensor *A, Tensor *B, float min, float max){
    if (A->isCPU() && B->isCPU()) {
        cpu_clamp(A, B, min, max);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        gpu_clamp(A, B, min, max);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::clampmax(Tensor *A, Tensor *B, float max){
    Tensor::clamp(A, B, MIN_FLOAT, max);
}


void Tensor::clampmin(Tensor *A, Tensor *B, float min){
    Tensor::clamp(A, B, min, MAX_FLOAT);
}


void Tensor::cos(Tensor *A, Tensor *B){
    if (A->isCPU() && B->isCPU()) {
        cpu_cos(A, B);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        gpu_cos(A, B);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::cosh(Tensor *A, Tensor *B){
    if (A->isCPU() && B->isCPU()) {
        cpu_cosh(A, B);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        gpu_cosh(A, B);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::div(Tensor *A, Tensor *B, float v){
    Tensor::mult(A, B, 1.0f/v);
}


void Tensor::exp(Tensor *A, Tensor *B){
    if (A->isCPU() && B->isCPU()) {
        cpu_exp(A, B);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        gpu_exp(A, B);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::floor(Tensor *A, Tensor *B){
    if (A->isCPU() && B->isCPU()) {
        cpu_floor(A, B);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        gpu_floor(A, B);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::inv(Tensor *A, Tensor *B, float v){
    if (A->isCPU() && B->isCPU()) {
        cpu_inv(A, B, v);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        gpu_inv(A, B, v);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::log(Tensor *A, Tensor *B){
    if (A->isCPU() && B->isCPU()) {
        cpu_log(A, B);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        gpu_log(A, B);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::log2(Tensor *A, Tensor *B){
    if (A->isCPU() && B->isCPU()) {
        cpu_log2(A, B);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        gpu_log2(A, B);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::log10(Tensor *A, Tensor *B){
    if (A->isCPU() && B->isCPU()) {
        cpu_log10(A, B);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        gpu_log10(A, B);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::logn(Tensor *A, Tensor *B, float n){
    if (A->isCPU() && B->isCPU()) {
        cpu_logn(A, B, n);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        gpu_logn(A, B, n);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::mod(Tensor *A, Tensor *B, float v){
    if (A->isCPU() && B->isCPU()) {
        cpu_mod(A, B, v);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        gpu_mod(A, B, v);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::mult(Tensor *A, Tensor *B, float v){
    if (A->isCPU() && B->isCPU()) {
        cpu_mult(A, B, v);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        gpu_mult(A, B, v);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::neg(Tensor *A, Tensor *B){
    Tensor::mult(A, B, -1.0f);
}


void Tensor::normalize(Tensor *A, Tensor *B, float min, float max){
    if (A->isCPU() && B->isCPU()) {
        cpu_normalize(A, B, min, max);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        cpu_normalize(A, B, min, max);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::pow(Tensor *A, Tensor *B, float exp){
    if (A->isCPU() && B->isCPU()) {
        cpu_pow(A, B, exp);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        cpu_pow(A, B, exp);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}



void Tensor::powb(Tensor *A, Tensor *B, float base){
    if (A->isCPU() && B->isCPU()) {
        cpu_powb(A, B, base);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        cpu_powb(A, B, base);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::reciprocal(Tensor *A, Tensor *B){
    if (A->isCPU() && B->isCPU()) {
        cpu_reciprocal(A, B);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        gpu_reciprocal(A, B);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::remainder(Tensor *A, Tensor *B, float v){
    if (A->isCPU() && B->isCPU()) {
        cpu_remainder(A, B, v);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        gpu_remainder(A, B, v);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::round(Tensor *A, Tensor *B){
    if (A->isCPU() && B->isCPU()) {
        cpu_round(A, B);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        gpu_round(A, B);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::rsqrt(Tensor *A, Tensor *B){
    if (A->isCPU() && B->isCPU()) {
        cpu_rsqrt(A, B);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        gpu_rsqrt(A, B);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::sigmoid(Tensor *A, Tensor *B){
    if (A->isCPU() && B->isCPU()) {
        cpu_sigmoid(A, B);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        gpu_sigmoid(A, B);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::sign(Tensor *A, Tensor *B, float zero_sign) {
    if (A->isCPU() && B->isCPU()) {
        cpu_sign(A, B, zero_sign);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        gpu_sign(A, B, zero_sign);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::sin(Tensor *A, Tensor *B){
    if (A->isCPU() && B->isCPU()) {
        cpu_sin(A, B);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        gpu_sin(A, B);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::sinh(Tensor *A, Tensor *B){
    if (A->isCPU() && B->isCPU()) {
        cpu_sinh(A, B);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        gpu_sinh(A, B);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::sqr(Tensor *A, Tensor *B){
    if (A->isCPU() && B->isCPU()) {
        cpu_sqr(A, B);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        gpu_sqr(A, B);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::sqrt(Tensor *A, Tensor *B){
    if (A->isCPU() && B->isCPU()) {
        cpu_log10(A, B);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        gpu_sqrt(A, B);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::sub(Tensor *A, Tensor *B, float v){
    Tensor::add(A, B, -1.0f * v);
}


void Tensor::tan(Tensor *A, Tensor *B){
    if (A->isCPU() && B->isCPU()) {
        cpu_tan(A, B);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        gpu_tan(A, B);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::tanh(Tensor *A, Tensor *B){
    if (A->isCPU() && B->isCPU()) {
        cpu_log10(A, B);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        gpu_tanh(A, B);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


void Tensor::trunc(Tensor *A, Tensor *B){
    if (A->isCPU() && B->isCPU()) {
        cpu_trunc(A, B);
    }
#ifdef cGPU
    else if (A->isGPU() && B->isGPU())
      {
        gpu_trunc(A, B);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
}


// Math operations (binary) ************************
void Tensor::add(Tensor *A, Tensor *B, Tensor *C) {
    Tensor::add(1.0, A, 1.0, B, C, 0);
}

void Tensor::div(Tensor *A, Tensor *B, Tensor *C) {
    Tensor::el_div(A, B, C, 0);
}

void Tensor::mult(Tensor *A, Tensor *B, Tensor *C) {
    Tensor::el_mult(A, B, C, 0);
}

void Tensor::interpolate(float factor1, Tensor *A, float factor2, Tensor *B, Tensor *C){
    Tensor::add(factor1, A, factor2, B, C, 1);
}

void Tensor::sub(Tensor *A, Tensor *B, Tensor *C) {
    Tensor::add(1.0, A, -1.0, B, C, 0);
}


// ***** Overload operators *****************************
// Tensor and Tensor (Element wise)  ********************
Tensor& operator+ (Tensor &A, Tensor &B) {
    Tensor* t = Tensor::add(&A, &B);
    return (*t);
}

//Tensor& operator- (Tensor &A, Tensor &B) {
//    Tensor* t = Tensor::sub(&A, &B);
//    return (*t);
//}

Tensor& operator* (Tensor &A, Tensor &B) {
    Tensor* t = Tensor::mult(&A, &B);
    return (*t);
}


Tensor& operator/ (Tensor &A, Tensor &B) {
    Tensor* t = Tensor::div(&A, &B);
    return (*t);
}


// Tensor op= Tensor (Element wise)  ********************
void operator+= (Tensor &A, Tensor &B) {
    Tensor::add(1.0f, &A, 1.0f, &B, &A, 0);
}

void operator-= (Tensor &A, Tensor &B) {
    Tensor::add(1.0f, &A, -1.0f, &B, &A, 0);
}

void operator*= (Tensor &A, Tensor &B) {
    Tensor::el_mult(&A, &B, &A, 0);
}

void operator/= (Tensor &A, Tensor &B) {
    Tensor::el_div(&A, &B, &A, 0);
}

// Tensor op= Scalar  ********************
void operator+= (Tensor &A, float v) {
    A.add_(v);
}

void operator-= (Tensor &A, float v) {
    A.sub_(v);
}

void operator*= (Tensor &A, float v) {
    A.mult_(v);
}

void operator/= (Tensor &A, float v) {
    A.div_(v);
}

// Tensor and scalar *******************
Tensor& operator+ (Tensor &A, float v) {
    Tensor* t = A.clone();
    t->add_(v);
    return (*t);
}

Tensor& operator- (Tensor &A, float v) {
    Tensor* t = A.clone();
    t->add_(-v);
    return (*t);
}

Tensor& operator* (Tensor &A, float v) {
    Tensor* t = A.clone();
    t->mult_(v);
    return (*t);
}

Tensor& operator/ (Tensor &A, float v) {
    Tensor* t = A.clone();
    t->div_(v);
    return (*t);
}


// Scalar and Tensor *******************
Tensor& operator+ (float v, Tensor &A) {
    return A + v;
}

Tensor& operator- (float v, Tensor &A) {
    Tensor* t = A.clone();
    t->neg_();
    t->add_(v);
    return (*t);
}

Tensor& operator* (float v, Tensor &A) {
    return A * v;
}

Tensor& operator/ (float v, Tensor &A) {
    Tensor* t = A.clone();
    t->inv_(v);
    return (*t);
}





void Tensor::add(float scA, Tensor *A, float scB, Tensor *B, Tensor *C, int incC) {
    ///////////////////////////////////////
    //// sum C=(sca*A)+(scb*B)
    //// or C+=(sca*A)+(scb*B) if incC is 1
    //// Dimensions and types must be compatible
    ///////////////////////////////////////
    int aux = 0;


    if ((A->device != B->device) || (A->device != C->device)) msg("Tensors in different devices", "Tensor::add_");
    if ((!eqsize(A, B)) || (!eqsize(A, C))) {
        A->info();
        B->info();
        C->info();
        msg("Incompatible dims", "Tensor::add");
    }

    C->tsem->lock();
    if (A->isCPU()) {
        cpu_add(scA, A, scB, B, C, incC);
    }
#ifdef cGPU
    else if (A->isGPU())
      {
        gpu_addc(scA,A,scB,B,C,incC);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif

    C->tsem->unlock();
}


void Tensor::inc(Tensor *A, Tensor *B) {
    // TODO: Review against add

    if (!Tensor::eqsize(A, B))
        msg("Tensors with different shape", "Tensor::inc");


    if ((A->isCPU()) && (B->isCPU())) {
        cpu_inc(A, B);
    }
#ifdef cGPU
        else if ((A->isGPU())&&(B->isGPU())) {
        Tensor::add(1,A,1,B,B,0);
    }
    else if (((A->isCPU())&&(B->isGPU()))||((A->isGPU())&&(B->isCPU())))
    {
        Tensor *n=new Tensor(B->getShape(),B->device);
        Tensor::copy(A,n);
        Tensor::add(1,n,1,B,B,0);
        delete n;
    }
#endif
    else {
        fprintf(stderr, "(%d %d)\n", A->device, B->device);
        msg("unsupported inc between devices", "Tensor::inc");
    }
}




void Tensor::el_div(Tensor *A, Tensor *B, Tensor *C, int incC) {
    ///////////////////////////////////////
    //// Element Div C=A./B
    //// incC 1 means C+=A./B (increment over C)
    //// Dimensions must be compatible
    ///////////////////////////////////////

    if ((A->device != B->device) || (A->device != C->device)) msg("Tensors in different devices", "Tensor::el_div");
    if ((!eqsize(A, B)) || (!eqsize(A, C))) msg("Incompatible dims", "Tensor::el_div");

    C->tsem->lock();
    if (A->isCPU()) {
        cpu_el_div(A, B, C, incC);
    }
#ifdef cGPU
    else if (A->isGPU())
      {
        gpu_el_div(A,B,C,incC);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
    C->tsem->unlock();
}





void Tensor::mult2D(Tensor *A, int tA, Tensor *B, int tB, Tensor *C, int incC) {
    ///////////////////////////////////////
    //// MULT2D C=A*B
    //// tA means transpose A {0,1}
    //// tB means transpose B {0,1}
    //// tC 1 means C+=A*B (increment over C)
    //// Dimensions and types must be compatible
    //// Only for 2D Tensors
    ///////////////////////////////////////

    if ((A->device != B->device) || (A->device != C->device)) {A->info();B->info();C->info();msg("Tensors in different devices", "Tensor::mult2D");}
    if ((A->ndim != 2) || (B->ndim != 2) || (C->ndim != 2)) msg("Only 2D tensors", "Tensor::mult2D");
    if (!tA) {
        if (!tB) {
            if ((A->shape[1] != B->shape[0]) || (A->shape[0] != C->shape[0]) || (B->shape[1] != C->shape[1]))
                msg("Incompatible dims", "Tensor::mult2D");
        } else if ((A->shape[1] != B->shape[1]) || (A->shape[0] != C->shape[0]) || (B->shape[0] != C->shape[1]))
            msg("Incompatible dims", "Tensor::mult2D");
    } else {
        if (!tB) {
            if ((A->shape[0] != B->shape[0]) || (A->shape[1] != C->shape[0]) || (B->shape[1] != C->shape[1]))
                msg("Incompatible dims", "Tensor::mult2D");
        } else if ((A->shape[0] != B->shape[1]) || (A->shape[1] != C->shape[0]) || (B->shape[0] != C->shape[1]))
            msg("Incompatible dims", "Tensor::mult2D");
    }


    C->tsem->lock();

    if (A->isCPU()) {
        cpu_mult2D(A, tA, B, tB, C, incC);
    }

#ifdef cGPU
    else if (A->isGPU())
      {
        gpu_mult2D(A,tA,B,tB,C,incC);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
    C->tsem->unlock();
}


void Tensor::el_mult(Tensor *A, Tensor *B, Tensor *C, int incC) {
    ///////////////////////////////////////
    //// Element Mult C=A.*B
    //// incC 1 means C+=A.*B (increment over C)
    //// Dimensions must be compatible
    ///////////////////////////////////////
    C->tsem->lock();
    if ((A->device != B->device) || (A->device != C->device)) msg("Tensors in different devices", "Tensor::el_mult");
    if ((!eqsize(A, B)) || (!eqsize(A, C))) {
        A->info();
        B->info();
        C->info();
        msg("Incompatible dims", "Tensor::el_mult");
    }

    if (A->isCPU()) {
        cpu_el_mult(A, B, C, incC);
    }
#ifdef cGPU
    else if (A->isGPU())
      {
         gpu_el_mult(A,B,C,incC);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
    C->tsem->unlock();
}


void Tensor::sum2D_rowwise(Tensor *A, Tensor *B, Tensor *C) {
    ///////////////////////////////////////
    //// sum2D_rowise C=A.rowise+B
    //// Dimensions and types must be compatible
    //// A is 2D Tensor
    //// B is 1D Tensor
    ///////////////////////////////////////
    if ((A->device != B->device) || (A->device != C->device))
        msg("Tensors in different devices", "Tensor::sum2D_rowwise");
    if ((A->ndim != 2) || (B->ndim != 1) || (C->ndim != 2)) msg("sum2D_rowwise dims");
    if ((!eqsize(A, C)) || (A->shape[1] != B->shape[0])) msg("Incompatible dims", "Tensor::sum2D_rowwise");

    C->tsem->lock();
    if (A->isCPU()) {
        cpu_sum2D_rowwise(A, B, C);
    }
#ifdef cGPU
    else if (A->isGPU())
      {
        gpu_sum2D_rowwise(A,B,C);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
    C->tsem->unlock();
}


void Tensor::reduce_sum2D(Tensor *A, Tensor *B, int axis, int incB) {
    ///////////////////////////////////////
    //// reduce_sum2D B=reduce_sum2D(A)
    //// Dimensions and types must be compatible
    //// A is 2D Tensor
    //// B is 1D Tensor
    //// axis is the dimension to be sumed
    ///////////////////////////////////////
    if (A->device != B->device) msg("Tensors in different devices", "Tensor::reduce_sum2D");
    if ((A->ndim - 1) != B->ndim) msg("Incorrect dims", "Tensor::reduce_sum2D");
    if ((A->shape[1 - axis] != B->shape[0])) msg("Incompatible dims", "Tensor::reduce_sum2D");

    B->tsem->lock();
    if (A->isCPU()) {
        cpu_reduce_sum2D(A, B, axis, incB);
    }
#ifdef cGPU
    else if (A->isGPU())
      {
        gpu_reduce_sum2D(A,B,axis,incB);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
    B->tsem->unlock();
}

void Tensor::sum2D_colwise(Tensor *A, Tensor *B, Tensor *C) {
    ///////////////////////////////////////
    //// sum2D_colwise C=A.colwise+B
    //// Dimensions and types must be compatible
    //// A is 2D Tensor
    //// B is 1D Tensor
    ///////////////////////////////////////
    if ((A->device != B->device) || (A->device != C->device))
        msg("Tensors in different devices", "Tensor::sum2D_colwise");
    if ((A->ndim != 2) || (B->ndim != 1) || (C->ndim != 2)) msg("sum2D_colwise dims");
    if ((!eqsize(A, C)) || (A->shape[0] != B->shape[0])) msg("Incompatible dims", "Tensor::sum2D_colwise");

    C->tsem->lock();
    if (A->isCPU()) {
        cpu_sum2D_colwise(A, B, C);
    }
#ifdef cGPU
    else if (A->isGPU())
      {
        gpu_sum2D_colwise(A,B,C);
      }
#endif
#ifdef cFPGA
    else {

    }
#endif
    C->tsem->unlock();
}
