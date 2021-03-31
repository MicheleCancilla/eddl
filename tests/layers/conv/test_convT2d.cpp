#include <gtest/gtest.h>

#include "eddl/tensor/tensor.h"
#include "eddl/tensor/nn/tensor_nn.h"
#include "eddl/descriptors/descriptors.h"



TEST(ConvT2DTestSuite, convt2d_custom)
{
    // Image (force padding manually, I don't want surprises)
    auto *ptr_img = new float[3*5*5]{
            -2.13, 0.60, 0.81, -1.50, -0.55,
            -0.02, 0.61, 0.95, -1.18, 0.75,
            0.55, -1.22, -0.62, -0.13, 0.21,
            0.06, -1.16, -1.14, -0.60, 1.42,
            3.27, -0.47, -0.86, 0.27, -0.26,

            -0.15, 1.23, 1.21, 2.31, 2.23,
            -0.04, -0.35, -0.53, -0.73, -0.17,
            -1.03, 1.03, -1.24, 0.36, -0.52,
            -0.81, 0.59, 1.90, 0.06, -0.85,
            0.11, -0.09, 0.84, 0.14, -1.68,

            -0.23, 0.33, 0.48, -1.43, -0.05,
            -0.10, -0.13, -1.32, 0.07, -0.35,
            0.61, 1.96, 1.11, 0.60, -0.83,
            0.58, 0.28, 0.32, -0.05, -1.93,
            0.03, -0.46, 0.08, 0.49, 0.39
    };
    auto* t_image = new Tensor({1, 3, 5, 5}, ptr_img, DEV_CPU);
//    t_image->toGPU();

    // Forward
    auto *ptr_fwrd = new float[1*7*7]{
        -2.51, -0.35, 2.15, 4.04, 3.51, 1.01, 1.63,
        -2.67, -0.38, 1.22, 1.43, 1.00, -0.60, 1.86,
        -2.54, 1.52, 2.37, 3.28, -0.06, -0.91, 0.72,
        -0.20, 1.41, 0.84, -0.56, -4.44, -3.87, -2.27,
        3.37, 3.83, 4.22, 1.99, -2.52, -2.91, -4.05,
        3.24, 1.93, 3.07, 0.14, -1.46, -2.60, -2.91,
        3.41, 2.39, 2.45, -0.06, -0.59, -0.65, -1.55,
    };;
    auto* t_fwrd = new Tensor({1, 1, 7, 7}, ptr_fwrd, DEV_CPU);
//    t_fwrd->toGPU();


    // Operation
    auto *cd = new ConvolDescriptorT(1, {3, 3}, {1, 1}, "none", {}, 1, {1, 1}, true);
    cd->build(t_image);
    cd->K = Tensor::ones({3, 1, 3, 3}, DEV_CPU);
    cd->bias = Tensor::zeros({1}, DEV_CPU); //Tensor::zeros(cd->bias->getShape());
    cd->ID = Tensor::zeros(cd->I->getShape());
    cd->D = Tensor::ones(cd->O->getShape());

//    cd->K->toGPU();
//    cd->bias->toGPU();
//    cd->ID->toGPU();
//    cd->D->toGPU();

    // Forward
    tensorNN::Conv2DT(cd);
    ASSERT_TRUE((bool) Tensor::equivalent(t_fwrd, cd->O, 1e-3f, 0.0f, true, true));

//    // Backward
//    tensorNN::Conv2DT_back(cd);
////    cd->ID->toCPU(); cd->ID->print(2, true);
//    ASSERT_TRUE((bool) Tensor::equivalent(t_bwrd, cd->ID, 1e-3f, 0.0f, true, true));
}

TEST(ConvT2DTestSuite, conv2d_k2x2_s2x2_pad_valid)
{
    // Image
    auto *ptr_img = new float[5*5]{0, 1, 0, 4, 5,
                                   2, 3, 2, 1, 3,
                                   4, 4, 0, 4, 3,
                                   2, 5, 2, 6, 4,
                                   1, 0, 0, 5, 7};
    auto* t_image = new Tensor({1, 1, 5, 5}, ptr_img, DEV_CPU);


    // Forward
    auto *ptr_fwrd = new float[2*2]{6.00, 7.00,
                                    15.00, 12.00};
    auto* t_fwrd = new Tensor({1, 1, 2, 2}, ptr_fwrd, DEV_CPU);


    // backward
    auto *ptr_bwrd = new float[5*5]{1.00, 1.00, 1.00, 1.00, 0.00,
                                    1.00, 1.00, 1.00, 1.00, 0.00,
                                    1.00, 1.00, 1.00, 1.00, 0.00,
                                    1.00, 1.00, 1.00, 1.00, 0.00,
                                    0.00, 0.00, 0.00, 0.00, 0.00};
    auto* t_bwrd = new Tensor({1, 1, 5, 5}, ptr_bwrd, DEV_CPU);

    // Operation
    auto *cd = new ConvolDescriptor(1, {2, 2}, {2, 2}, "valid", {}, 1, {1, 1}, true);
    cd->build(t_image);
    cd->K = Tensor::ones(cd->K->getShape());
    cd->bias = Tensor::zeros(cd->bias->getShape());
    cd->ID = Tensor::zeros(cd->I->getShape());
    cd->D = Tensor::ones(cd->O->getShape());

    // Forward
    tensorNN::Conv2D(cd);
    ASSERT_TRUE((bool) Tensor::equivalent(t_fwrd, cd->O, 1e-3f, 0.0f, true, true));

    // Backward
    tensorNN::Conv2D_back(cd);
    ASSERT_TRUE((bool) Tensor::equivalent(t_bwrd, cd->ID, 1e-3f, 0.0f, true, true));

}

TEST(ConvT2DTestSuite, conv2d_k2x2_s2x2_pad_same)
{
    // Image
    auto *ptr_img = new float[5*5]{0, 1, 0, 4, 5,
                                   2, 3, 2, 1, 3,
                                   4, 4, 0, 4, 3,
                                   2, 5, 2, 6, 4,
                                   1, 0, 0, 5, 7};
    auto* t_image = new Tensor({1, 1, 5, 5}, ptr_img, DEV_CPU);


    // Forward
    auto *ptr_fwrd = new float[3*3]{6.00, 7.00, 8.00,
                                    15.00, 12.00, 7.00,
                                    1.00, 5.00, 7.00};
    auto* t_fwrd = new Tensor({1, 1, 3, 3}, ptr_fwrd, DEV_CPU);


    // backward
    auto *ptr_bwrd = new float[5*5]{1.00, 1.00, 1.00, 1.00, 1.00,
                                    1.00, 1.00, 1.00, 1.00, 1.00,
                                    1.00, 1.00, 1.00, 1.00, 1.00,
                                    1.00, 1.00, 1.00, 1.00, 1.00,
                                    1.00, 1.00, 1.00, 1.00, 1.00};
    auto* t_bwrd = new Tensor({1, 1, 5, 5}, ptr_bwrd, DEV_CPU);

    // Operation
    auto *cd = new ConvolDescriptor(1, {2, 2}, {2, 2}, "same", {}, 1, {1, 1}, true);
    cd->build(t_image);
    cd->K = Tensor::ones(cd->K->getShape());
    cd->bias = Tensor::zeros(cd->bias->getShape());
    cd->ID = Tensor::zeros(cd->I->getShape());
    cd->D = Tensor::ones(cd->O->getShape());

    // Forward
    tensorNN::Conv2D(cd);
    ASSERT_TRUE((bool) Tensor::equivalent(t_fwrd, cd->O, 1e-3f, 0.0f, true, true));

    // Backward
    tensorNN::Conv2D_back(cd);
    ASSERT_TRUE((bool) Tensor::equivalent(t_bwrd, cd->ID, 1e-3f, 0.0f, true, true));
}


TEST(ConvT2DTestSuite, conv2d_k3x3_s1x1_pad_valid)
{
    // Image
    auto *ptr_img = new float[5*5]{0, 1, 0, 4, 5,
                                   2, 3, 2, 1, 3,
                                   4, 4, 0, 4, 3,
                                   2, 5, 2, 6, 4,
                                   1, 0, 0, 5, 7};
    auto* t_image = new Tensor({1, 1, 5, 5}, ptr_img, DEV_CPU);


    // Forward
    auto *ptr_fwrd = new float[3*3]{16.00, 19.00, 22.00,
                                    24.00, 27.00, 25.00,
                                    18.00, 26.00, 31.00};
    auto* t_fwrd = new Tensor({1, 1, 3, 3}, ptr_fwrd, DEV_CPU);


    // backward
    auto *ptr_bwrd = new float[5*5]{1.00, 2.00, 3.00, 2.00, 1.00,
                                    2.00, 4.00, 6.00, 4.00, 2.00,
                                    3.00, 6.00, 9.00, 6.00, 3.00,
                                    2.00, 4.00, 6.00, 4.00, 2.00,
                                    1.00, 2.00, 3.00, 2.00, 1.00};
    auto* t_bwrd = new Tensor({1, 1, 5, 5}, ptr_bwrd, DEV_CPU);

    // Operation
    auto *cd = new ConvolDescriptor(1, {3, 3}, {1,1}, "valid", {}, 1, {1, 1}, true);
    cd->build(t_image);
    cd->K = Tensor::ones(cd->K->getShape());
    cd->bias = Tensor::zeros(cd->bias->getShape());
    cd->ID = Tensor::zeros(cd->I->getShape());
    cd->D = Tensor::ones(cd->O->getShape());

    // Forward
    tensorNN::Conv2D(cd);
    ASSERT_TRUE((bool) Tensor::equivalent(t_fwrd, cd->O, 1e-3f, 0.0f, true, true));

    // Backward
    tensorNN::Conv2D_back(cd);
    ASSERT_TRUE((bool) Tensor::equivalent(t_bwrd, cd->ID, 1e-3f, 0.0f, true, true));
}


TEST(ConvT2DTestSuite, conv2d_k3x3_s1x1_pad_same)
{
    // Image
    auto *ptr_img = new float[5*5]{0, 1, 0, 4, 5,
                                   2, 3, 2, 1, 3,
                                   4, 4, 0, 4, 3,
                                   2, 5, 2, 6, 4,
                                   1, 0, 0, 5, 7};
    auto* t_image = new Tensor({1, 1, 5, 5}, ptr_img, DEV_CPU);


    // Forward
    auto *ptr_fwrd = new float[5*5]{6.00, 8.00, 11.00, 15.00, 13.00,
                                    14.00, 16.00, 19.00, 22.00, 20.00,
                                    20.00, 24.00, 27.00, 25.00, 21.00,
                                    16.00, 18.00, 26.00, 31.00, 29.00,
                                    8.00, 10.00, 18.00, 24.00, 22.00};
    auto* t_fwrd = new Tensor({1, 1, 5, 5}, ptr_fwrd, DEV_CPU);


    // backward
    auto *ptr_bwrd = new float[5*5]{4.00, 6.00, 6.00, 6.00, 4.00,
                                    6.00, 9.00, 9.00, 9.00, 6.00,
                                    6.00, 9.00, 9.00, 9.00, 6.00,
                                    6.00, 9.00, 9.00, 9.00, 6.00,
                                    4.00, 6.00, 6.00, 6.00, 4.00};
    auto* t_bwrd = new Tensor({1, 1, 5, 5}, ptr_bwrd, DEV_CPU);

    // Operation
    auto *cd = new ConvolDescriptor(1, {3, 3}, {1, 1}, "same", {}, 1, {1, 1}, true);
    cd->build(t_image);
    cd->K = Tensor::ones(cd->K->getShape());
    cd->bias = Tensor::zeros(cd->bias->getShape());
    cd->ID = Tensor::zeros(cd->I->getShape());
    cd->D = Tensor::ones(cd->O->getShape());

    // Forward
    tensorNN::Conv2D(cd);
    ASSERT_TRUE((bool) Tensor::equivalent(t_fwrd, cd->O, 1e-3f, 0.0f, true, true));

    // Backward
    tensorNN::Conv2D_back(cd);
    ASSERT_TRUE((bool) Tensor::equivalent(t_bwrd, cd->ID, 1e-3f, 0.0f, true, true));
}




TEST(ConvT2DTestSuite, conv2d_k3x3_s2x2_pad_valid)
{
    // Image
    auto *ptr_img = new float[5*5]{0, 1, 0, 4, 5,
                                   2, 3, 2, 1, 3,
                                   4, 4, 0, 4, 3,
                                   2, 5, 2, 6, 4,
                                   1, 0, 0, 5, 7};
    auto* t_image = new Tensor({1, 1, 5, 5}, ptr_img, DEV_CPU);


    // Forward
    auto *ptr_fwrd = new float[2*2]{16.00, 22.00,
                                    18.00, 31.00};
    auto* t_fwrd = new Tensor({1, 1, 2, 2}, ptr_fwrd, DEV_CPU);


    // backward
    auto *ptr_bwrd = new float[5*5]{1.00, 1.00, 2.00, 1.00, 1.00,
                                    1.00, 1.00, 2.00, 1.00, 1.00,
                                    2.00, 2.00, 4.00, 2.00, 2.00,
                                    1.00, 1.00, 2.00, 1.00, 1.00,
                                    1.00, 1.00, 2.00, 1.00, 1.00};
    auto* t_bwrd = new Tensor({1, 1, 5, 5}, ptr_bwrd, DEV_CPU);

    // Operation
    auto *cd = new ConvolDescriptor(1, {3, 3}, {2, 2}, "valid", {}, 1, {1, 1}, true);
    cd->build(t_image);
    cd->K = Tensor::ones(cd->K->getShape());
    cd->bias = Tensor::zeros(cd->bias->getShape());
    cd->ID = Tensor::zeros(cd->I->getShape());
    cd->D = Tensor::ones(cd->O->getShape());

    // Forward
    tensorNN::Conv2D(cd);
    ASSERT_TRUE((bool) Tensor::equivalent(t_fwrd, cd->O, 1e-3f, 0.0f, true, true));

    // Backward
    tensorNN::Conv2D_back(cd);
    ASSERT_TRUE((bool) Tensor::equivalent(t_bwrd, cd->ID, 1e-3f, 0.0f, true, true));
}


TEST(ConvT2DTestSuite, conv2d_k3x3_s2x2_pad_same){
    // Image
    auto *ptr_img = new float[5*5]{0, 1, 0, 4, 5,
                                   2, 3, 2, 1, 3,
                                   4, 4, 0, 4, 3,
                                   2, 5, 2, 6, 4,
                                   1, 0, 0, 5, 7};
    auto* t_image = new Tensor({1, 1, 5, 5}, ptr_img, DEV_CPU);


    // Forward
    auto *ptr_fwrd = new float[3*3]{6.00, 11.00, 13.00,
                                    20.00, 27.00, 21.00,
                                    8.00, 18.00, 22.00};
    auto* t_fwrd = new Tensor({1, 1, 3, 3}, ptr_fwrd, DEV_CPU);


    // backward
    auto *ptr_bwrd = new float[5*5]{1.00, 2.00, 1.00, 2.00, 1.00,
                                    2.00, 4.00, 2.00, 4.00, 2.00,
                                    1.00, 2.00, 1.00, 2.00, 1.00,
                                    2.00, 4.00, 2.00, 4.00, 2.00,
                                    1.00, 2.00, 1.00, 2.00, 1.00};
    auto* t_bwrd = new Tensor({1, 1, 5, 5}, ptr_bwrd, DEV_CPU);

    // Operation
    auto *cd = new ConvolDescriptor(1, {3, 3}, {2, 2}, "same", {}, 1, {1, 1}, true);
    cd->build(t_image);
    cd->K = Tensor::ones(cd->K->getShape());
    cd->bias = Tensor::zeros(cd->bias->getShape());
    cd->ID = Tensor::zeros(cd->I->getShape());
    cd->D = Tensor::ones(cd->O->getShape());

    // Forward
    tensorNN::Conv2D(cd);
    ASSERT_TRUE((bool) Tensor::equivalent(t_fwrd, cd->O, 1e-3f, 0.0f, true, true));

    // Backward
    tensorNN::Conv2D_back(cd);
    ASSERT_TRUE((bool) Tensor::equivalent(t_bwrd, cd->ID, 1e-3f, 0.0f, true, true));
}

#ifdef cGPU
TEST(ConvT2DTestSuite, conv2d_cpu_gpu){
    // Image
    Tensor* t_cpu = Tensor::randu({1, 3, 1000, 1000});
    Tensor* t_gpu = t_cpu->clone(); t_gpu->toGPU();

    vector<string> padding = {"valid", "same"};
    vector<int> strides = {1, 2, 3, 5};
    vector<int> kernels = {1, 2, 3, 5, 7};

    for(auto& p : padding){
        for(auto& s : strides){
            for(auto& k : kernels){
                try{
                    // CPU Operation
                    auto *cd_cpu = new ConvolDescriptor(1, {k, k}, {s, s}, p, {}, 1, {1, 1}, true);
                    cd_cpu->build(t_cpu);
                    cd_cpu->K = Tensor::ones(cd_cpu->K->getShape());
                    cd_cpu->bias = Tensor::zeros(cd_cpu->bias->getShape());
                    cd_cpu->ID = Tensor::zeros(cd_cpu->I->getShape());
                    cd_cpu->D = Tensor::ones(cd_cpu->O->getShape());

                    // GPU Operation
                    auto *cd_gpu = new ConvolDescriptor(1, {k, k}, {s, s}, p, {}, 1, {1, 1}, true);
                    cd_gpu->build(t_gpu);
                    cd_gpu->K = Tensor::ones(cd_gpu->K->getShape(), t_gpu->device);
                    cd_gpu->bias = Tensor::zeros(cd_gpu->bias->getShape(), t_gpu->device);
                    cd_gpu->ID = Tensor::zeros(cd_gpu->I->getShape(), t_gpu->device);
                    cd_gpu->D = Tensor::ones(cd_gpu->O->getShape(), t_gpu->device);

                    // Forward
                    tensorNN::Conv2D(cd_cpu);
                    tensorNN::Conv2D(cd_gpu);
                    Tensor *cd_gpu_O = cd_gpu->O->clone(); cd_gpu_O->toCPU();  // Tensor::equivalent is only for CPU (at the moment)
                    bool test_fwrd = (bool) Tensor::equivalent(cd_cpu->O, cd_gpu_O, 1e-3f, 0.0f, true, true);

                    // Backward
                    tensorNN::Conv2D_back(cd_cpu);
                    tensorNN::Conv2D_back(cd_gpu);
                    Tensor *cd_gpu_ID = cd_gpu->ID->clone(); cd_gpu_ID->toCPU(); // Tensor::equivalent is only for CPU (at the moment)
                    bool test_bwrd = (bool) Tensor::equivalent(cd_cpu->ID, cd_gpu_ID, 1e-3f, 0.0f, true, true);

                    // Print results to ease debugging
                    cout << "Testing conv2d_cpu_gpu (" << "padding=" << p << "; kernel=" << k << "; stride=" << s << ")" <<
                         " [Forward="<< test_fwrd << "; Backward=" << test_bwrd << "]" << endl;

                    // Test correctness
                    ASSERT_TRUE(test_fwrd);
                    ASSERT_TRUE(test_bwrd);

                    delete cd_cpu->K;
                    delete cd_cpu->bias;
                    delete cd_cpu->ID;
                    delete cd_cpu->D;
                    delete cd_cpu;

                    delete cd_gpu->K;
                    delete cd_gpu->bias;
                    delete cd_gpu->ID;
                    delete cd_gpu->D;
                    delete cd_gpu;

                    delete cd_gpu_O;
                    delete cd_gpu_ID;
                }
                catch (...) {
                    cout << "[FAILED] Testing conv2d_cpu_gpu (" << "padding=" << p << "; kernel=" << k << "; stride=" << s << ")" <<endl;
                }
            }
        }
    }
}
#endif


