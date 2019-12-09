/*
* EDDL Library - European Distributed Deep Learning Library.
* Version: 0.2
* copyright (c) 2019, Universidad Politécnica de Valencia (UPV), PRHLT Research Centre
* Date: October 2019
* Author: PRHLT Research Centre, UPV, (rparedes@prhlt.upv.es), (jon@prhlt.upv.es)
* All rights reserved
*/


#include <stdio.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "eddl.h"


using namespace std;

////////////////////////////////////////////////////////
///// EDDL is a wrapper class to ease and define the API
////////////////////////////////////////////////////////


namespace eddl {

    // ---- CORE LAYERS ----
    layer Activation(layer parent, string activation, float param, string name) {
        return new LActivation(parent, activation, name, DEV_CPU,param);
    }
    layer Softmax(layer parent)
    {
      return new LActivation(parent,"softmax","",DEV_CPU);
    }
    layer Sigmoid(layer parent)
    {
      return new LActivation(parent,"sigmoid","",DEV_CPU);
    }
    layer ReLu(layer parent)
    {
      return new LActivation(parent,"relu","",DEV_CPU);
    }
    layer LReLu(layer parent,float param)
    {
      return new LActivation(parent,"lrelu","",DEV_CPU,param);
    }

    layer Tanh(layer parent)
    {
      return new LActivation(parent,"tanh","",DEV_CPU);
    }

    layer Conv(layer parent, int filters, const vector<int> &kernel_size,
               const vector<int> &strides, string padding, int groups, const vector<int> &dilation_rate,
               bool use_bias, string name) {
        LConv *l = new LConv(parent, filters, kernel_size, strides, padding, groups, dilation_rate, use_bias, name, DEV_CPU);

        return l;
    }

    layer ConvT(layer parent, int filters, const vector<int> &kernel_size,
                const vector<int> &output_padding, string padding, const vector<int> &dilation_rate,
                const vector<int> &strides, bool use_bias, string name) {
        return new LConvT(parent, filters, kernel_size, output_padding, padding, dilation_rate, strides, use_bias, name,
                          DEV_CPU);
    }

    layer Dense(layer parent, int ndim, bool use_bias, string name) {
        LDense *l = new LDense(parent, ndim, use_bias, name, DEV_CPU);

        return l;
    }

    layer Embedding(int input_dim, int output_dim, string name) {
        return new LEmbedding(input_dim, output_dim, name, DEV_CPU);
    }

    layer Input(const vector<int> &shape, string name) {
        tshape s = vector<int>(shape.begin(), shape.end());
        s.insert(s.begin(), 1);
        return new LInput(new Tensor(s), name, DEV_CPU);
    }

    layer UpSampling(layer parent, const vector<int> &size, string interpolation, string name) {
        return new LUpSampling(parent, size, interpolation, name, DEV_CPU);
    }

    layer Reshape(layer parent, const vector<int> &shape, string name) {
        tshape s = vector<int>(shape.begin(), shape.end());
        s.insert(s.begin(), 1);
        return new LReshape(parent, s, name, DEV_CPU);
    }

    layer Transpose(layer parent, const vector<int> &dims, string name) {
        return new LTranspose(parent, dims, name, DEV_CPU);
    }

    // ---- TRANSFORMATIONS ----
    layer Shift(layer parent, vector<int> shift, string da_mode, float constant, string name){
        return new LShift(parent, shift, da_mode, constant, name, DEV_CPU);
    }
    layer Rotate(layer parent, float angle, vector<int> offset_center, string da_mode, float constant, string name){
        return new LRotate(parent, angle, offset_center, da_mode, constant, name, DEV_CPU);
    }

    layer Scale(layer parent, vector<int> new_shape, bool reshape, string da_mode, float constant, string name){
        return new LScale(parent, new_shape, reshape, da_mode, constant, name, DEV_CPU);
    }

    layer Flip(layer parent, int axis, string name){
        return new LFlip(parent, axis, name, DEV_CPU);
    }

    layer Crop(layer parent, vector<int> from_coords, vector<int> to_coords, bool reshape, float constant, string name){
        return new LCrop(parent, from_coords, to_coords, reshape, constant, name, DEV_CPU);
    }

    layer Crop(layer parent, vector<int> size, bool reshape, float constant, string name){
        // Compute center
        int center_y = (int)parent->output->shape[2]/2;
        int center_x = (int)parent->output->shape[3]/2;

        // Get coordinates
        int y1 = center_y - size[0]/2;
        int x1 = center_x - size[1]/2;
        int y2 = y1 + size[0] - 1;
        int x2 = x1 + size[1] - 1;
        return new LCrop(parent, {y1, x1}, {y2, x2}, reshape, constant, name, DEV_CPU);
    }

    layer CropAndScale(layer parent, vector<int> from_coords, vector<int> to_coords, string da_mode, float constant, string name){
        return new LCropAndScale(parent, from_coords, to_coords, da_mode, constant, name, DEV_CPU);
    }

    layer Cutout(layer parent, vector<int> from_coords, vector<int> to_coords, float constant, string name){
        return new LCutout(parent, from_coords, to_coords, constant, name, DEV_CPU);
    }

    // ---- DATA AUGMENTATION ----
    layer ShiftRandom(layer parent, vector<float> factor_x, vector<float> factor_y, string da_mode, float constant, string name){
        return new LShiftRandom(parent, factor_x, factor_y, da_mode, constant, name, DEV_CPU);
    }

    layer RotateRandom(layer parent, vector<float> factor, vector<int> offset_center, string da_mode, float constant, string name){
        return new LRotateRandom(parent, factor, offset_center, da_mode, constant, name, DEV_CPU);
    }

    layer ScaleRandom(layer parent, vector<float> factor, string da_mode, float constant, string name){
        return new LScaleRandom(parent, factor, da_mode, constant, name, DEV_CPU);
    }

    layer FlipRandom(layer parent, int axis, string name){
        return new LFlipRandom(parent, axis, name, DEV_CPU);
    }

    layer CropRandom(layer parent, vector<int> new_shape, string name){
        return new LCropRandom(parent, new_shape, name, DEV_CPU);
    }

    layer CropScaleRandom(layer parent, vector<float> factor, string da_mode, string name){
        return new LCropScaleRandom(parent, factor, da_mode, name, DEV_CPU);
    }

    layer CutoutRandom(layer parent, vector<float> factor_x, vector<float> factor_y, float constant, string name){
        return new LCutoutRandom(parent, factor_x, factor_y, constant, name, DEV_CPU);
    }

    // ---- MERGE LAYERS ----
    layer Add(const vector<layer> &layers, string name) {
        return new LAdd(layers, name, DEV_CPU);
    }

    layer Average(const vector<layer> &layers, string name) {
        //TODO: Implement
        return new LAverage(layers, name, DEV_CPU);
    }

    layer Concat(const vector<layer> &layers, string name) {
        return new LConcat(layers, name, DEV_CPU);
    }

    layer MatMul(const vector<layer> &layers, string name) {
        return new LMatMul(layers, name, DEV_CPU);
    }

    layer Maximum(const vector<layer> &layers, string name) {
        return new LMaximum(layers, name, DEV_CPU);
    }

    layer Minimum(const vector<layer> &layers, string name) {
        return new LMinimum(layers, name, DEV_CPU);
    }

    layer Subtract(const vector<layer> &layers, string name) {
        return new LSubtract(layers, name, DEV_CPU);
    }


    // ---- NOISE LAYERS ----
    layer GaussianNoise(layer parent, float stdev, string name) {
        return new LGaussianNoise(parent, stdev, name, DEV_CPU);
    }


    // ---- NORMALIZATION LAYERS ----
    layer BatchNormalization(layer parent, float momentum, float epsilon, bool affine, string name) {
        return new LBatchNorm(parent, momentum, epsilon, affine, name, DEV_CPU);
    }
    layer LayerNormalization(layer parent, float momentum, float epsilon, bool affine, string name) {
        return new LLayerNorm(parent, momentum, epsilon, affine, name, DEV_CPU);
    }
    layer GroupNormalization(layer parent, int groups, float momentum, float epsilon, bool affine, string name) {
        return new LGroupNorm(parent, groups, momentum, epsilon, affine, name, DEV_CPU);
    }


    layer Norm(layer parent, float epsilon, string name)
    {
      return new LNorm(parent, epsilon, name, DEV_CPU);
    }

    layer NormMax(layer parent, float epsilon, string name)
    {
      return new LNormMax(parent, epsilon, name, DEV_CPU);
    }

    layer NormMinMax(layer parent, float epsilon, string name)
    {
      return new LNormMinMax(parent, epsilon, name, DEV_CPU);
    }


    layer Dropout(layer parent, float rate, string name) {
        return new LDropout(parent, rate, name, DEV_CPU);
    }


    // ---- OPERATOR LAYERS ----
    layer Abs(layer l) {
        return new LAbs(l, "", DEV_CPU);
    }

    layer Diff(layer l1, layer l2) {
        return new LDiff(l1, l2, "", DEV_CPU);
    }

    layer Diff(layer l1, float k) {
        return new LDiff(l1, k, "", DEV_CPU);
    }

    layer Diff(float k,layer l1) {
        return new LDiff(k, l1, "", DEV_CPU);
    }
    layer Div(layer l1, layer l2) {
        return new LDiv(l1, l2, "", DEV_CPU);
    }

    layer Div(layer l1, float k) {
        return new LDiv(l1, k, "", DEV_CPU);
    }

    layer Div(float k,layer l1) {
        return new LDiv(k, l1, "", DEV_CPU);
    }

    layer Exp(layer l) {
        return new LExp(l, "", DEV_CPU);
    }

    layer Log(layer l) {
        return new LLog(l, "", DEV_CPU);
    }

    layer Log2(layer l) {
        return new LLog2(l, "", DEV_CPU);
    }

    layer Log10(layer l) {
        return new LLog10(l, "", DEV_CPU);
    }

    layer Mult(layer l1, layer l2) {
        return new LMult(l1, l2, "", DEV_CPU);
    }

    layer Mult(layer l1, float k) {
        return new LMult(l1, k, "", DEV_CPU);
    }

    layer Mult(float k,layer l1) {
        return new LMult(l1, k, "", DEV_CPU);
    }
    layer Pow(layer l1, layer l2) {
        return new LPow(l1, l2, "", DEV_CPU);
    }

    layer Pow(layer l1, float k) {
        return new LPow(l1, k, "", DEV_CPU);
    }

    layer Sqrt(layer l) {
        return new LSqrt(l, "", DEV_CPU);
    }

    layer Sum(layer l1, layer l2) {
        return new LSum(l1, l2, "", DEV_CPU);
    }

    layer Sum(layer l1, float k) {
        return new LSum(l1, k, "", DEV_CPU);
    }

    layer Sum(float k,layer l1) {
        return new LSum(l1, k, "", DEV_CPU);
    }

    layer Select(layer l, vector<string> indices){
        return new LSelect(l, indices, "", DEV_CPU);
    }

    layer Permute(layer l, vector<int> dims){
        return new LPermute(l, dims, "", DEV_CPU);
    }

    // ---- REDUCTION LAYERS ----
    layer ReduceMean(layer l, const vector<int> axis, bool keepdims) {
        return new LRMean(l, axis, keepdims, "", DEV_CPU);
    }

    layer ReduceVar(layer l, const vector<int> axis, bool keepdims) {
        return new LRVar(l, axis, keepdims, "", DEV_CPU);
    }

    layer ReduceSum(layer l, const vector<int> axis, bool keepdims) {
        return new LRSum(l, axis, keepdims, "", DEV_CPU);
    }

    layer ReduceMax(layer l, const vector<int> axis, bool keepdims) {
        return new LRMax(l, axis, keepdims, "", DEV_CPU);
    }

    layer ReduceMin(layer l, const vector<int> axis, bool keepdims) {
        return new LRMin(l, axis, keepdims, "", DEV_CPU);
    }


    // ---- GENERATOR LAYERS ----
    layer GaussGenerator(float mean, float stdev, vector<int> size) {
        return new LGauss(mean, stdev, size, "", DEV_CPU);
    }

    layer UniformGenerator(float low, float high, vector<int> size) {
        return new LUniform(low, high, size, "", DEV_CPU);
    }

    // ---- LOSSES ----
    Loss* getLoss(string type) {
        if (type == "mse" || type == "mean_squared_error") {
            return new LMeanSquaredError();
        } else if (type == "cross_entropy") {
            return new LCrossEntropy();
        } else if (type == "soft_cross_entropy") {
            return new LSoftCrossEntropy();
        }
        return nullptr;
    }

    loss newloss(const std::function<Layer*(vector<Layer*>)>& f, vector<Layer*> in, string name)
    {
      return new NetLoss(f,in,name);
    }
    loss newloss(const std::function<Layer*(Layer*)>& f, Layer *in, string name)
    {
      return new NetLoss(f,in,name);
    }
    // ---- METRICS ----
    Metric* getMetric(string type) {
        if (type == "mse" || type == "mean_squared_error") {
            return new MMeanSquaredError();
        } else if (type == "categorical_accuracy" || type == "accuracy") {
            return new MCategoricalAccuracy();
        }
        else if (type=="mean_absolute_error") {
          return new MMeanAbsoluteError();
        }
        else if (type=="mean_relative_error") {
          return new MMeanRelativeError();
        }
        else {
          cout<<"Not supported metric: "<<type<<"\n";
          exit(1);
        }
        return nullptr;
    }

    // ---- OPTIMIZERS ----
    optimizer adadelta(float lr, float rho, float epsilon, float weight_decay) {
        //Todo: Implement
        return new AdaDelta(lr, rho, epsilon, weight_decay);
    }

    optimizer adam(float lr, float beta_1, float beta_2, float epsilon, float weight_decay, bool amsgrad) {
        //Todo: Implement
        return new Adam(lr, beta_1, beta_2, epsilon, weight_decay, amsgrad);
    }

    optimizer adagrad(float lr, float epsilon, float weight_decay) {
        //Todo: Implement
        return new Adagrad(lr, epsilon, weight_decay);
    }

    optimizer adamax(float lr, float beta_1, float beta_2, float epsilon, float weight_decay) {
        //Todo: Implement
        return new Adamax(lr, beta_1, beta_2, epsilon, weight_decay);
    }

    optimizer nadam(float lr, float beta_1, float beta_2, float epsilon, float schedule_decay) {
        //Todo: Implement
        return new Nadam(lr, beta_1, beta_2, epsilon, schedule_decay);
    }

    optimizer rmsprop(float lr, float rho, float epsilon, float weight_decay) {
        //Todo: Implement
        return new RMSProp(lr, rho, epsilon, weight_decay);
    }

    optimizer sgd(float lr, float momentum, float weight_decay, bool nesterov) {
        return new SGD(lr, momentum, weight_decay, nesterov);
    }


    // ---- POOLING LAYERS ----
    layer AveragePool(layer parent, const vector<int> &pool_size, const vector<int> &strides, string padding,
                      string name) {
        //TODO: Implement
        return new LAveragePool(parent, pool_size, strides, padding, name, DEV_CPU);
    }

    layer GlobalMaxPool(layer parent, string name) {
        //TODO: Implement
        //return new LGlobalMaxPool(parent, PoolDescriptor({1,1}, {1,1}), name, DEV_CPU);
        return nullptr;
    }

    layer GlobalAveragePool(layer parent, string name) {
        //TODO: Implement
        //return new LGlobalAveragePool(parent,  PoolDescriptor({1,1}, {1,1}, "none"), name, DEV_CPU);
        return nullptr;
    }

    layer MaxPool(layer parent, const vector<int> &pool_size, const vector<int> &strides, string padding, string name) {
        return new LMaxPool(parent, pool_size, strides, padding, name, DEV_CPU);
    }


    // ---- RECURRENT LAYERS ----
    layer RNN(layer parent, int units, int num_layers, bool use_bias, float dropout, bool bidirectional, string name) {
        return new LRNN(parent, units, num_layers, use_bias, dropout, bidirectional, name, DEV_CPU);
    }

    layer LSTM(layer parent, int units, int num_layers, bool use_bias, float dropout, bool bidirectional, string name) {
        return new LLSTM(parent, units, num_layers, use_bias, dropout, bidirectional, name, DEV_CPU);
    }


    // ---- INITIALIZERS ----
    layer GlorotNormal(layer l,int seed)
    {
      l->init=new IGlorotNormal(seed);
      return l;
    }

    layer GlorotUniform(layer l,int seed)
    {
      l->init=new IGlorotUniform(seed);
      return l;
    }

    layer RandomNormal(layer l, float m,float s, float seed)
    {
      l->init=new IRandomNormal(m,s,seed);
      return l;
    }

    layer RandomUniform(layer l, float min,float max, float seed)
    {
      l->init=new IRandomUniform(min,max,seed);
      return l;
    }

    layer Constant(layer l, float v)
    {
      l->init=new IConstant(v);
      return l;
    }


    // ---- REGULARIZERS ----
    layer L2(layer l,float l2){
      l->reg=new RL2(l2);
      return l;
    }
    layer L1(layer l,float l1){
      l->reg=new RL1(l1);
      return l;
    }
    layer L1L2(layer l,float l1,float l2){
      l->reg=new RL1L2(l1,l2);
      return l;
    }


    // ---- COMPUTING SERVICES ----
    compserv CS_CPU(int th) {
        return new CompServ(th, {}, {},0);
    }

    compserv CS_GPU(const vector<int> &g,int lsb) {
        return new CompServ(0, g, {},lsb);
    }

    compserv CS_FGPA(const vector<int> &f,int lsb) {
        return new CompServ(0, {}, f,lsb);
    }

    compserv CS_COMPSS(string filename) {
        return new CompServ(filename);
    }



    // ---- FINE-GRAINED METHODS ----
    vector<int> random_indices(int batch_size, int num_samples){
        vector<int> sind;
        for (int k = 0; k < batch_size; k++) sind.push_back(rand() % num_samples);
        return sind;
    }

    void resize_model(model net, int batch_size){
        net->resize(batch_size);
    }

    void set_mode(model net, int mode){
        net->setmode(mode);
    }

    void train_batch(model net, vector<Tensor *> in, vector<Tensor *> out, vector<int> indices){
        net->tr_batches++;
        net->train_batch(in, out, indices);
    }

    void eval_batch(model net, vector<Tensor *> in, vector<Tensor *> out, vector<int> indices){
        net->train_batch(in, out, indices,1);
    }

    void next_batch(vector<Tensor *> in,vector<Tensor *> out)
     {
      int i,n;
      int batch_size;

      batch_size=out[0]->shape[0];
      n=in[0]->shape[0];
      vector<int> sind(batch_size);
      for (i = 0; i < batch_size; i++) sind[i] = rand() % n;
      for (i = 0; i<in.size();i++)
        Tensor::select(in[i], out[i], sind, 0, batch_size);
    }

    vlayer getOut(model net)
    {
      if (net->lout.size()) return net->lout;

      vlayer out;
      for(int i=0;i<net->layers.size();i++)
       if(net->layers[i]->child.size()==0)
         out.push_back(net->layers[i]);

      if (out.size()==0) {
        cout<<"Forwar over net "<<net->name<<"without outputs\n";
        exit(1);
      }


      return out;

    }
    vlayer forward(model net,vector<Layer*> in)
    {
      net->reset();
      net->forward(in);

      return getOut(net);
    }
    vlayer forward(model net,vector<Tensor*> in)
    {
      net->reset();
      net->forward(in);

      return getOut(net);
    }

    vlayer forward(model net,int b)
    {
      net->resize(b);
      net->reset();
      net->forward();

      return getOut(net);

    }
    vlayer forward(model net)
    {
      net->reset();
      net->forward();

      return getOut(net);
    }

    void clamp(model m,float min,float max)
    {
      m->clamp(min,max);
    }

    layer detach(layer l)
    {
      l->setdetach();
      return l;
    }

    vlayer detach(vlayer l)
    {
      for(int i=0;i<l.size();i++)
        l[i]->setdetach();
      return l;
    }

    void print_loss(model m, int batch){
      m->print_loss(batch);
    }

    void reset_loss(model m)
    {
      m->reset_loss();
    }

    void zeroGrads(model net)
    {
      net->reset_grads();
    }

    void backward(model net,vector<Tensor *> target)
    {
      net->backward(target);
    }


    void backward(model net)
    {
      net->backward({});
    }

    void backward(loss l)
    {
      l->graph->backward();
    }

    float compute_loss(loss L)
    {
      return L->compute();
    }

    void update(model net)
    {
      net->update();
    }

    void copyTensor(Layer *l1,Layer *l2)
    {
      collectTensor(l1);
      Tensor::copy(l1->output,l2->output);
      distributeTensor(l2);
    }

    void copyGrad(Layer *l1,Layer *l2)
    {
      collectTensor(l1,"grad");
      Tensor::copy(l1->delta,l2->delta);
      distributeTensor(l2,"grad");
    }


    Tensor* getTensor(layer l1) {
      collectTensor(l1);
      return l1->output;
    }

    Tensor* getGrad(layer l1) {
      collectTensor(l1,"grad");
      return l1->delta;
    }



    // ---- MODEL METHODS ----
    model Model(vlayer in, vlayer out) {
        return new Net(in, out);
    }

    void build(model net, optimizer o, CompServ *cs) {
        // Assign default computing service
        if (cs== nullptr){
            cs = new CompServ(std::thread::hardware_concurrency(), {}, {});
        }
        if (o== nullptr){
            o = new SGD(0.001,0.9);
        }

        net->build(o, {}, {}, cs);
    }

    void build(model net, optimizer o, const vector<string> &lo, const vector<string> &me, CompServ *cs) {
        vector<Loss *> l;
        vector<Metric *> m;

        // TODO: Fix this sh*t
        // Replace string by functions
        for (const auto &li : lo) {
            l.push_back(getLoss(li));
        }
        for (const auto &mi : me) {
            m.push_back(getMetric(mi));
        }

        // Assign default computing service
        if (cs== nullptr){
            cs = new CompServ(std::thread::hardware_concurrency(), {}, {});
        }

        net->build(o, l, m, cs);
    }
    void toGPU(model net, vector<int> g,int lsb)
    {
      net->toGPU(g,lsb);
    }
    void toCPU(model net, int t)
    {
      net->toCPU(t);
    }

    void setlogfile(model net,string fname)
    {
      net->setlogfile(fname);
    }

    void setlr(model net,vector<float>p)
    {
      net->setlr(p);
    }

    void summary(model m) {
        cout<<m->summary()<<"\n";
    }

    void load(model m, const string&  fname, string format) {
        m->load(fname,format);
    }

    void save(model m, const string&  fname, string format) {
        m->save(fname,format);
    }

    void plot(model m, string fname,string mode) {
        m->plot(fname,mode);
    }

    void fit(model net, const vector<Tensor *> &in, const vector<Tensor *> &out, int batch, int epochs) {
        net->fit(in, out, batch, epochs);
    }

    void evaluate(model net, const vector<Tensor *> &in, const vector<Tensor *> &out) {
        net->evaluate(in, out);
    }


    // ---- DATASETS ----

    bool exist(string name) {
        if (FILE *file = fopen(name.c_str(), "r")) {
            fclose(file);
            return true;
        }
        return false;
    }




    void download_mnist() {
        // TODO: Too big, we should use the one in the PyEDDL
        // TODO: Need for "to_categorical" method
        string cmd;
        string trX = "trX.bin";
        string trY = "trY.bin";
        string tsX = "tsX.bin";
        string tsY = "tsY.bin";

        if ((!exist(trX)) || (!exist(trY)) || (!exist(tsX)) || (!exist(tsY))) {
            cmd = "wget https://www.dropbox.com/s/khrb3th2z6owd9t/trX.bin";
            int status = system(cmd.c_str());
            if (status < 0) {
                msg("wget must be installed", "eddl.download_mnist");
                exit(1);
            }

            cmd = "wget https://www.dropbox.com/s/m82hmmrg46kcugp/trY.bin";
            status = system(cmd.c_str());
            if (status < 0) {
                msg("wget must be installed", "eddl.download_mnist");
                exit(1);
            }
            cmd = "wget https://www.dropbox.com/s/7psutd4m4wna2d5/tsX.bin";
            status = system(cmd.c_str());
            if (status < 0) {
                msg("wget must be installed", "eddl.download_mnist");
                exit(1);
            }
            cmd = "wget https://www.dropbox.com/s/q0tnbjvaenb4tjs/tsY.bin";
            status = system(cmd.c_str());
            if (status < 0) {
                msg("wget must be installed", "eddl.download_mnist");
                exit(1);
            }

        }
    }

    void download_cifar10() {
        // TODO: Too big, we should use the one in the PyEDDL
        // TODO: Need for "to_categorical" method
        string cmd;
        string trX = "cifar_trX.bin";
        string trY = "cifar_trY.bin";
        string tsX = "cifar_tsX.bin";
        string tsY = "cifar_tsY.bin";

        if ((!exist(trX)) || (!exist(trY)) || (!exist(tsX)) || (!exist(tsY))) {
            cmd = "wget https://www.dropbox.com/s/wap282xox5ew02d/cifar_trX.bin";
            int status = system(cmd.c_str());
            if (status < 0) {
                msg("wget must be installed", "eddl.download_mnist");
                exit(1);
            }

            cmd = "wget https://www.dropbox.com/s/yxhw99cu1ktiwxq/cifar_trY.bin";
            status = system(cmd.c_str());
            if (status < 0) {
                msg("wget must be installed", "eddl.download_mnist");
                exit(1);
            }
            cmd = "wget https://www.dropbox.com/s/dh9vqxe9vt7scrp/cifar_tsX.bin";
            status = system(cmd.c_str());
            if (status < 0) {
                msg("wget must be installed", "eddl.download_mnist");
                exit(1);
            }
            cmd = "wget https://www.dropbox.com/s/gdmsve6mbu82ndp/cifar_tsY.bin";
            status = system(cmd.c_str());
            if (status < 0) {
                msg("wget must be installed", "eddl.download_mnist");
                exit(1);
            }

        }
    }
}
