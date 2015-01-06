/*
 * JHPCN-DF - Data compression library based on
 *            Jointed Hierarchical Precision Compression Number Data Format
 *
 * Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 */

// @file TestZeroPadding.cpp

#include "gtest/gtest.h"
#include <cmath>
#include "Utility.h"

class FloatZeroPaddingTest : public ::testing::Test
{
    protected:
        FloatZeroPaddingTest():max_size(128){}
        virtual void SetUp(void)
        {
            src=new float[max_size];
            dst=new float[max_size];
            for(int i=0; i<max_size; i++)
            {
                src[i]=FLT_MAX;
                dst[i]=0;
            }
        }
        virtual void TearDown(void)
        {
            delete [] src;
            delete [] dst;
        }
        const int max_size;
        float* src;
        float* dst;
};

TEST_F(FloatZeroPaddingTest, NormalImplimentation)
{
    EXPECT_EQ(FLT_MAX, n_bit_zero_padding(src[0], 0));
    EXPECT_FLOAT_EQ(FLT_MAX-FLT_MIN, n_bit_zero_padding(src[0], 1));
    EXPECT_EQ(std::pow(2,127), n_bit_zero_padding(src[0], 23));
    EXPECT_EQ(0, n_bit_zero_padding(src[0], 31));
    EXPECT_EQ(0, n_bit_zero_padding(src[0], 32));
}

TEST_F(FloatZeroPaddingTest, SIMDOrientedImplimentationVL1)
{
#define VL 1
    n_bit_zero_padding<VL>(src, dst, 0);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(FLT_MAX, dst[i]);

    n_bit_zero_padding<VL>(src, dst, 1);
    for(int i=0; i<VL;i++)
    EXPECT_FLOAT_EQ(FLT_MAX-FLT_MIN, dst[i]);

    n_bit_zero_padding<VL>(src, dst, 23);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(std::pow(2,127), dst[i]);

    n_bit_zero_padding<VL>(src, dst, 31);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);

    n_bit_zero_padding<VL>(src, dst, 32);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);
#undef VL
}

TEST_F(FloatZeroPaddingTest, SIMDOrientedImplimentationVL2)
{
#define VL 2
    n_bit_zero_padding<VL>(src, dst, 0);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(FLT_MAX, dst[i]);

    n_bit_zero_padding<VL>(src, dst, 1);
    for(int i=0; i<VL;i++)
    EXPECT_FLOAT_EQ(FLT_MAX-FLT_MIN, dst[i]);

    n_bit_zero_padding<VL>(src, dst, 23);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(std::pow(2,127), dst[i]);

    n_bit_zero_padding<VL>(src, dst, 31);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);

    n_bit_zero_padding<VL>(src, dst, 32);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);
#undef VL
}

TEST_F(FloatZeroPaddingTest, SIMDOrientedImplimentationVL4)
{
#define VL 4
    n_bit_zero_padding<VL>(src, dst, 0);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(FLT_MAX, dst[i]);

    n_bit_zero_padding<VL>(src, dst, 1);
    for(int i=0; i<VL;i++)
    EXPECT_FLOAT_EQ(FLT_MAX-FLT_MIN, dst[i]);

    n_bit_zero_padding<VL>(src, dst, 23);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(std::pow(2,127), dst[i]);

    n_bit_zero_padding<VL>(src, dst, 31);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);

    n_bit_zero_padding<VL>(src, dst, 32);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);
#undef VL
}
TEST_F(FloatZeroPaddingTest, SIMDOrientedImplimentationVL8)
{
#define VL 8
    n_bit_zero_padding<VL>(src, dst, 0);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(FLT_MAX, dst[i]);

    n_bit_zero_padding<VL>(src, dst, 1);
    for(int i=0; i<VL;i++)
    EXPECT_FLOAT_EQ(FLT_MAX-FLT_MIN, dst[i]);

    n_bit_zero_padding<VL>(src, dst, 23);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(std::pow(2,127), dst[i]);

    n_bit_zero_padding<VL>(src, dst, 31);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);

    n_bit_zero_padding<VL>(src, dst, 32);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);
#undef VL
}
TEST_F(FloatZeroPaddingTest, SIMDOrientedImplimentationVL16)
{
#define VL 16
    n_bit_zero_padding<VL>(src, dst, 0);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(FLT_MAX, dst[i]);

    n_bit_zero_padding<VL>(src, dst, 1);
    for(int i=0; i<VL;i++)
    EXPECT_FLOAT_EQ(FLT_MAX-FLT_MIN, dst[i]);

    n_bit_zero_padding<VL>(src, dst, 23);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(std::pow(2,127), dst[i]);

    n_bit_zero_padding<VL>(src, dst, 31);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);

    n_bit_zero_padding<VL>(src, dst, 32);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);
#undef VL
}
TEST_F(FloatZeroPaddingTest, SIMDOrientedImplimentationVL32)
{
#define VL 32
    n_bit_zero_padding<VL>(src, dst, 0);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(FLT_MAX, dst[i]);

    n_bit_zero_padding<VL>(src, dst, 1);
    for(int i=0; i<VL;i++)
    EXPECT_FLOAT_EQ(FLT_MAX-FLT_MIN, dst[i]);

    n_bit_zero_padding<VL>(src, dst, 23);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(std::pow(2,127), dst[i]);

    n_bit_zero_padding<VL>(src, dst, 31);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);

    n_bit_zero_padding<VL>(src, dst, 32);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);
#undef VL
}
TEST_F(FloatZeroPaddingTest, SIMDOrientedImplimentationVL64)
{
#define VL 64
    n_bit_zero_padding<VL>(src, dst, 0);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(FLT_MAX, dst[i]);

    n_bit_zero_padding<VL>(src, dst, 1);
    for(int i=0; i<VL;i++)
    EXPECT_FLOAT_EQ(FLT_MAX-FLT_MIN, dst[i]);

    n_bit_zero_padding<VL>(src, dst, 23);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(std::pow(2,127), dst[i]);

    n_bit_zero_padding<VL>(src, dst, 31);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);

    n_bit_zero_padding<VL>(src, dst, 32);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);
#undef VL
}
TEST_F(FloatZeroPaddingTest, SIMDOrientedImplimentationVL128)
{
#define VL 128
    n_bit_zero_padding<VL>(src, dst, 0);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(FLT_MAX, dst[i]);

    n_bit_zero_padding<VL>(src, dst, 1);
    for(int i=0; i<VL;i++)
    EXPECT_FLOAT_EQ(FLT_MAX-FLT_MIN, dst[i]);

    n_bit_zero_padding<VL>(src, dst, 23);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(std::pow(2,127), dst[i]);

    n_bit_zero_padding<VL>(src, dst, 31);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);

    n_bit_zero_padding<VL>(src, dst, 32);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);
#undef VL
}

TEST_F(FloatZeroPaddingTest, AndMaskImplimentationVL1)
{
#define VL 1
    n_bit_mask<VL,0>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(FLT_MAX, dst[i]);

    n_bit_mask<VL,1>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_FLOAT_EQ(FLT_MAX-FLT_MIN, dst[i]);

    n_bit_mask<VL,23>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(std::pow(2,127), dst[i]);

    n_bit_mask<VL,31>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);

    n_bit_mask<VL,32>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);
#undef VL
}

TEST_F(FloatZeroPaddingTest, AndMaskImplimentationVL2)
{
#define VL 2
    n_bit_mask<VL,0>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(FLT_MAX, dst[i]);

    n_bit_mask<VL,1>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_FLOAT_EQ(FLT_MAX-FLT_MIN, dst[i]);

    n_bit_mask<VL,23>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(std::pow(2,127), dst[i]);

    n_bit_mask<VL,31>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);

    n_bit_mask<VL,32>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);
#undef VL
}

TEST_F(FloatZeroPaddingTest, AndMaskImplimentationVL4)
{
#define VL 4
    n_bit_mask<VL,0>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(FLT_MAX, dst[i]);

    n_bit_mask<VL,1>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_FLOAT_EQ(FLT_MAX-FLT_MIN, dst[i]);

    n_bit_mask<VL,23>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(std::pow(2,127), dst[i]);

    n_bit_mask<VL,31>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);

    n_bit_mask<VL,32>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);
#undef VL
}

TEST_F(FloatZeroPaddingTest, AndMaskImplimentationVL8)
{
#define VL 8
    n_bit_mask<VL,0>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(FLT_MAX, dst[i]);

    n_bit_mask<VL,1>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_FLOAT_EQ(FLT_MAX-FLT_MIN, dst[i]);

    n_bit_mask<VL,23>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(std::pow(2,127), dst[i]);

    n_bit_mask<VL,31>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);

    n_bit_mask<VL,32>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);
#undef VL
}

TEST_F(FloatZeroPaddingTest, AndMaskImplimentationVL16)
{
#define VL 16
    n_bit_mask<VL,0>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(FLT_MAX, dst[i]);

    n_bit_mask<VL,1>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_FLOAT_EQ(FLT_MAX-FLT_MIN, dst[i]);

    n_bit_mask<VL,23>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(std::pow(2,127), dst[i]);

    n_bit_mask<VL,31>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);

    n_bit_mask<VL,32>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);
#undef VL
}

TEST_F(FloatZeroPaddingTest, AndMaskImplimentationVL32)
{
#define VL 32
    n_bit_mask<VL,0>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(FLT_MAX, dst[i]);

    n_bit_mask<VL,1>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_FLOAT_EQ(FLT_MAX-FLT_MIN, dst[i]);

    n_bit_mask<VL,23>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(std::pow(2,127), dst[i]);

    n_bit_mask<VL,31>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);

    n_bit_mask<VL,32>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);
#undef VL
}

TEST_F(FloatZeroPaddingTest, AndMaskImplimentationVL64)
{
#define VL 64
    n_bit_mask<VL,0>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(FLT_MAX, dst[i]);

    n_bit_mask<VL,1>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_FLOAT_EQ(FLT_MAX-FLT_MIN, dst[i]);

    n_bit_mask<VL,23>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(std::pow(2,127), dst[i]);

    n_bit_mask<VL,31>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);

    n_bit_mask<VL,32>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);
#undef VL
}

TEST_F(FloatZeroPaddingTest, AndMaskImplimentationVL128)
{
#define VL 128
    n_bit_mask<VL,0>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(FLT_MAX, dst[i]);

    n_bit_mask<VL,1>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_FLOAT_EQ(FLT_MAX-FLT_MIN, dst[i]);

    n_bit_mask<VL,23>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(std::pow(2,127), dst[i]);

    n_bit_mask<VL,31>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);

    n_bit_mask<VL,32>(src, dst);
    for(int i=0; i<VL;i++)
    EXPECT_EQ(0, dst[i]);
#undef VL
}

