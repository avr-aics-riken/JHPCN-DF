/*
 * JHPCN-DF - Data compression library based on
 *            Jointed Hierarchical Precision Compression Number Data Format
 *
 * Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 */

// @file TestOR.cpp

#include "gtest/gtest.h"
#include "Utility.h"


class FloatorTest : public ::testing::TestWithParam<float> {};
class DoubleorTest : public ::testing::TestWithParam<double > {};

TEST_P(FloatorTest, WithZero)
{
    float value=GetParam();
    EXPECT_EQ(real_or(0, value), value);
}
TEST_P(DoubleorTest, WithZero)
{
    double value=GetParam();
    EXPECT_EQ(real_or(0, value), value);
}

TEST_P(FloatorTest, SameData)
{
    float value=GetParam();
    EXPECT_EQ(real_or(value, value), 0);
}
TEST_P(DoubleorTest, SameData)
{
    double value=GetParam();
    EXPECT_EQ(real_or(value, value), 0);
}

TEST_P(FloatorTest, WithMinusZero)
{
    float value=GetParam();
    EXPECT_EQ(real_or(value, -0.0F), -value);
}
TEST_P(DoubleorTest, WithMinusZero)
{
    double value=GetParam();
    EXPECT_EQ(real_or(value, -0.0L), -value);
}

class Floator3OperandTest : public ::testing::TestWithParam<std::tr1::tuple<float, int> >
{
    protected:
        virtual void SetUp(void)
        {
            block_size=std::tr1::get<1>(GetParam());
            switch (block_size) {
                case 1:
                    func=&real_or<1>;
                    break;
                case 2:
                    func=&real_or<2>;
                    break;
                case 4:
                    func=&real_or<4>;
                    break;
                case 8:
                    func=&real_or<8>;
                    break;
                case 16:
                    func=&real_or<16>;
                    break;
                case 32:
                    func=&real_or<32>;
                    break;
                case 64:
                    func=&real_or<64>;
                    break;
            }
            src1=new float[block_size];
            src2=new float[block_size];
            result=new float[block_size];
            for(int i=0;i<block_size; i++)
            {
                src1[i]=std::tr1::get<0>(GetParam());
                src2[i]=0.0F;
            }
        }

        virtual void TearDown(void)
        {
            delete [] src1;
            delete [] src2;
            delete [] result;
        }

        float *src1;
        float *src2;
        float *result;
        void (*func)(const float*, const float*, float*);
        int block_size;
};

class Doubleor3OperandTest : public ::testing::TestWithParam<std::tr1::tuple<double, int> >
{
    protected:
        virtual void SetUp(void)
        {
            block_size=std::tr1::get<1>(GetParam());
            switch (block_size) {
                case 1:
                    func=&real_or<1>;
                    break;
                case 2:
                    func=&real_or<2>;
                    break;
                case 4:
                    func=&real_or<4>;
                    break;
                case 8:
                    func=&real_or<8>;
                    break;
                case 16:
                    func=&real_or<16>;
                    break;
                case 32:
                    func=&real_or<32>;
                    break;
                case 64:
                    func=&real_or<64>;
                    break;
            }
            src1=new double[block_size];
            src2=new double[block_size];
            result=new double[block_size];
            for(int i=0;i<block_size; i++)
            {
                src1[i]=std::tr1::get<0>(GetParam());
                src2[i]=0.0L;
            }
        }

        virtual void TearDown(void)
        {
            delete [] src1;
            delete [] src2;
            delete [] result;
        }

        double *src1;
        double *src2;
        double *result;
        void (*func)(const double*, const double*, double*);
        int block_size;
};

TEST_P(Floator3OperandTest, WithZero3Operand)
{
    func(src1, src2, result);
    for(int i=0; i<block_size;i++)
    {
        EXPECT_EQ(result[i], src1[i]);
    }
}
TEST_P(Doubleor3OperandTest, WithZero3Operand)
{
    func(src1, src2, result);
    for(int i=0; i<block_size;i++)
    {
        EXPECT_EQ(result[i], src1[i]);
    }
}

TEST_P(Floator3OperandTest, SameData3Operand)
{
    func(src1, src1, result);
    for(int i=0; i<block_size;i++)
    {
        EXPECT_EQ(result[i], 0.0F);
    }
}
TEST_P(Doubleor3OperandTest, SameData3Operand)
{
    func(src1, src1, result);
    for(int i=0; i<block_size;i++)
    {
        EXPECT_EQ(result[i], 0.0L);
    }
}

TEST_P(Floator3OperandTest, WithMinusZero3Operand)
{
    for(int i=0; i<block_size;i++)
    {
        src2[i]*=-1;
    }
    func(src1, src2, result);
    for(int i=0; i<block_size;i++)
    {
        EXPECT_EQ(result[i], -src1[i]);
    }
}
TEST_P(Doubleor3OperandTest, WithMinusZero3Operand)
{
    for(int i=0; i<block_size;i++)
    {
        src2[i]*=-1;
    }
    func(src1, src2, result);
    for(int i=0; i<block_size;i++)
    {
        EXPECT_EQ(result[i], -src1[i]);
    }
}
INSTANTIATE_TEST_CASE_P(Floator3OperandTest,   Floator3OperandTest, ::testing::Combine(
            ::testing::Values(FLT_MIN, FLT_MAX, 0.0F, -FLT_MIN, -FLT_MAX, 256.0F,  -256.0F),
            ::testing::Values(1,2,4,8,16,32,64)
            ));
INSTANTIATE_TEST_CASE_P(Doubleor3OperandTest, Doubleor3OperandTest, ::testing::Combine(
            ::testing::Values(DBL_MIN, DBL_MAX, 0.0L, -DBL_MIN, -DBL_MAX, 2014.0L, -2014.0L),
            ::testing::Values(1,2,4,8,16,32,64)
            ));
INSTANTIATE_TEST_CASE_P(FloatorTest,   FloatorTest, ::testing::Values(FLT_MIN, FLT_MAX, 0.0F, -FLT_MIN, -FLT_MAX, 256.0F,  -256.0F));
INSTANTIATE_TEST_CASE_P(DoubleorTest, DoubleorTest, ::testing::Values(DBL_MIN, DBL_MAX, 0.0L, -DBL_MIN, -DBL_MAX, 2014.0L, -2014.0L));
