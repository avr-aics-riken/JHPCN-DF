#include "gtest/gtest.h"
#include "Utility.h"

class FloatZeroPaddingTest : public ::testing::TestWithParam<float>{};
class DoubleZeroPaddingTest : public ::testing::TestWithParam<double>{};
class FloatZeroPadding3OperandTest : public ::testing::TestWithParam<std::tr1::tuple<float, int> >
{
    protected:
        virtual void SetUp(void)
        {
            block_size=std::tr1::get<1>(GetParam());
            switch (block_size) {
                case 1:
                    func=&n_bit_zero_padding<1>;
                    break;
                case 2:
                    func=&n_bit_zero_padding<2>;
                    break;
                case 4:
                    func=&n_bit_zero_padding<4>;
                    break;
                case 8:
                    func=&n_bit_zero_padding<8>;
                    break;
                case 16:
                    func=&n_bit_zero_padding<16>;
                    break;
                case 32:
                    func=&n_bit_zero_padding<32>;
                    break;
                case 64:
                    func=&n_bit_zero_padding<64>;
                    break;
            }
            src1=new float[block_size];
            result=new float[block_size];
            for(int i=0;i<block_size; i++)
            {
                src1[i]=std::tr1::get<0>(GetParam());
            }
        }

        virtual void TearDown(void)
        {
            delete [] src1;
            delete [] result;
        }

        float *src1;
        float *result;
        void (*func)(const float*, float*, const unsigned int&);
        int block_size;
};

class DoubleZeroPadding3OperandTest : public ::testing::TestWithParam<std::tr1::tuple<double, int> >
{
    protected:
        virtual void SetUp(void)
        {
            block_size=std::tr1::get<1>(GetParam());
            switch (block_size) {
                case 1:
                    func=&n_bit_zero_padding<1>;
                    break;
                case 2:
                    func=&n_bit_zero_padding<2>;
                    break;
                case 4:
                    func=&n_bit_zero_padding<4>;
                    break;
                case 8:
                    func=&n_bit_zero_padding<8>;
                    break;
                case 16:
                    func=&n_bit_zero_padding<16>;
                    break;
                case 32:
                    func=&n_bit_zero_padding<32>;
                    break;
                case 64:
                    func=&n_bit_zero_padding<64>;
                    break;
            }
            src1=new double[block_size];
            result=new double[block_size];
            for(int i=0;i<block_size; i++)
            {
                src1[i]=std::tr1::get<0>(GetParam());
            }
        }

        virtual void TearDown(void)
        {
            delete [] src1;
            delete [] result;
        }

        double *src1;
        double *result;
        void (*func)(const double*, double*, const unsigned int&);
        int block_size;
};


TEST_P(FloatZeroPaddingTest, ZeroBit)
{
    float value=GetParam();
    EXPECT_EQ(n_bit_zero_padding(value, 0), value);
}
TEST_P(DoubleZeroPaddingTest, ZeroBit)
{
    double value=GetParam();
    EXPECT_EQ(n_bit_zero_padding(value,0 ), value);
}
TEST_P(FloatZeroPadding3OperandTest, ZeroBit3Operand)
{
    func(src1, result, 0);
    for(int i=0; i<block_size;i++)
    {
        EXPECT_EQ(result[i], src1[i]);
    }
}
TEST_P(DoubleZeroPadding3OperandTest, ZeroBit3Operand)
{
    func(src1, result, 0);
    for(int i=0; i<block_size;i++)
    {
        EXPECT_EQ(result[i], src1[i]);
    }
}

INSTANTIATE_TEST_CASE_P(FloatZeroPadding3OperandTest,   FloatZeroPadding3OperandTest, ::testing::Combine(
            ::testing::Values(FLT_MIN, FLT_MAX, 0.0F, -FLT_MIN, -FLT_MAX, 256.0F,  -256.0F),
            ::testing::Values(1,2,4,8,16,32,64)
            ));
INSTANTIATE_TEST_CASE_P(DoubleZeroPadding3OperandTest, DoubleZeroPadding3OperandTest, ::testing::Combine(
            ::testing::Values(DBL_MIN, DBL_MAX, 0.0L, -DBL_MIN, -DBL_MAX, 2014.0L, -2014.0L),
            ::testing::Values(1,2,4,8,16,32,64)
            ));
INSTANTIATE_TEST_CASE_P(FloatZeroPaddingTest, FloatZeroPaddingTest, ::testing::Values(FLT_MIN, FLT_MAX, 0.0F, -FLT_MIN, -FLT_MAX, 256.0F,  -256.0F));
INSTANTIATE_TEST_CASE_P(DoubleZeroPaddingTest, DoubleZeroPaddingTest, ::testing::Values(DBL_MIN, DBL_MAX, 0.0L, -DBL_MIN, -DBL_MAX, 2014.0L, -2014.0L));
