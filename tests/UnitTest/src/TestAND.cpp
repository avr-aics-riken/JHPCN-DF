#include "gtest/gtest.h"
#include "Utility.h"

class FloatAnd3OperandTest : public ::testing::TestWithParam<std::tr1::tuple<float, int> >
{
    protected:
        virtual void SetUp(void)
        {
            block_size=std::tr1::get<1>(GetParam());
            switch (block_size) {
                case 1:
                    func=&real_and<1>;
                    break;
                case 2:
                    func=&real_and<2>;
                    break;
                case 4:
                    func=&real_and<4>;
                    break;
                case 8:
                    func=&real_and<8>;
                    break;
                case 16:
                    func=&real_and<16>;
                    break;
                case 32:
                    func=&real_and<32>;
                    break;
                case 64:
                    func=&real_and<64>;
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

class DoubleAnd3OperandTest : public ::testing::TestWithParam<std::tr1::tuple<double, int> >
{
    protected:
        virtual void SetUp(void)
        {
            block_size=std::tr1::get<1>(GetParam());
            switch (block_size) {
                case 1:
                    func=&real_and<1>;
                    break;
                case 2:
                    func=&real_and<2>;
                    break;
                case 4:
                    func=&real_and<4>;
                    break;
                case 8:
                    func=&real_and<8>;
                    break;
                case 16:
                    func=&real_and<16>;
                    break;
                case 32:
                    func=&real_and<32>;
                    break;
                case 64:
                    func=&real_and<64>;
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

TEST_P(FloatAnd3OperandTest, WithZero3Operand)
{
    func(src1, src2, result);
    for(int i=0; i<block_size;i++)
    {
        EXPECT_EQ(result[i], 0.0F);
    }
}
TEST_P(DoubleAnd3OperandTest, WithZero3Operand)
{
    func(src1, src2, result);
    for(int i=0; i<block_size;i++)
    {
        EXPECT_EQ(result[i], 0.0L);
    }
}

TEST_P(FloatAnd3OperandTest, SameData3Operand)
{
    func(src1, src1, result);
    for(int i=0; i<block_size;i++)
    {
        EXPECT_EQ(result[i], src1[i]);
    }
}
TEST_P(DoubleAnd3OperandTest, SameData3Operand)
{
    func(src1, src1, result);
    for(int i=0; i<block_size;i++)
    {
        EXPECT_EQ(result[i], src1[i]);
    }
}

TEST_P(FloatAnd3OperandTest, WithMinusZero3Operand)
{
    for(int i=0; i<block_size;i++)
    {
        src2[i]*=-1;
    }
    func(src1, src2, result);
    for(int i=0; i<block_size;i++)
    {
        EXPECT_EQ(result[i], -0.0F);
    }
}
TEST_P(DoubleAnd3OperandTest, WithMinusZero3Operand)
{
    for(int i=0; i<block_size;i++)
    {
        src2[i]*=-1;
    }
    func(src1, src2, result);
    for(int i=0; i<block_size;i++)
    {
        EXPECT_EQ(result[i], -0.0L);
    }
}
INSTANTIATE_TEST_CASE_P(FloatAnd3OperandTest,   FloatAnd3OperandTest, ::testing::Combine(
            ::testing::Values(FLT_MIN, FLT_MAX, 0.0F, -FLT_MIN, -FLT_MAX, 256.0F,  -256.0F),
            ::testing::Values(1,2,4,8,16,32,64)
            ));
INSTANTIATE_TEST_CASE_P(DoubleAnd3OperandTest, DoubleAnd3OperandTest, ::testing::Combine(
            ::testing::Values(DBL_MIN, DBL_MAX, 0.0L, -DBL_MIN, -DBL_MAX, 2014.0L, -2014.0L),
            ::testing::Values(1,2,4,8,16,32,64)
            ));
