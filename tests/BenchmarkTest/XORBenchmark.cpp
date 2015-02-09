/*
 * JHPCN-DF - Data compression library based on
 *            Jointed Hierarchical Precision Compression Number Data Format
 *
 * Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 */

// @file XORBenchmark.cpp

#include <typeinfo>
#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <cstdlib>
#include "Utility.h"
#include <omp.h>
#include "benchmark_common.h"

template <typename T>
class XORBenchmark
{
    public:
    XORBenchmark(int argc, char *argv[]):name("XOR"), num_test(100)
    {
        double t0=omp_get_wtime();
        if(argc !=2)
        {
            std::cerr<<"usage: "<<argv[0]<<" size_of_data_in_MiB"<<std::endl;
            exit(1);
        }
        num_data=std::atol(argv[1])*1024*1024/sizeof(T);

        random_data= new T [num_data];
        RandomNumber<REAL_TYPE> generator;
        generator(num_data, random_data);
        result = new T [num_data];
        data1  = new T [num_data];
        data2  = new T [num_data];
        copy_data(num_data, random_data, data1);
        zero_clear(num_data, data2);
        double t1=omp_get_wtime()-t0;
        std::cout << "Test data type = "<< typeid(T).name()<<std::endl;
        std::cout << "Test data size = "<< argv[1] <<" MiByte"<<std::endl;
        std::cout << "Elapsed time for initialize: "<<t1<<" sec"<<std::endl;
        std::cout << std::endl;
        std::cout << "====" << this->name << " Benchmark start ====" << std::endl;
    }
    ~XORBenchmark()
    {
        delete [] result;
        delete [] data1;
        delete [] data2;
    }
    double benchmark0()
    {
        double t0=omp_get_wtime();
        for(int test=0; test<num_test; test++)
        {
#pragma omp parallel for
            for(int i=0;i<num_data; i++)
            {
	        real4byte tmp1;
	        real4byte tmp2;
	        tmp1.real=data1[i];
	        tmp2.real=data2[i];
	        tmp1.integer ^= tmp2.integer;
                result[i] = tmp1.real;
            }
        }
        double t1= omp_get_wtime()-t0;
        TearDown();
        return t1/num_test;
    }
    double benchmark1()
    {
        double t0=omp_get_wtime();
        for(int test=0; test<num_test; test++)
        {
#pragma omp parallel for
            for(int i=0;i<num_data; i++)
            {
                result[i] = real_xor(data1[i], data2[i]);
            }
        }
        double t1= omp_get_wtime()-t0;
        TearDown();
        return t1/num_test;
    }

    template <void (*XOR)(const T*, const T*, T*), int NB>
    double benchmark2()
    {
        double t0=omp_get_wtime();
        for(int test=0; test<num_test; test++)
        {
#pragma omp parallel for
            for(int i=0;i<num_data; i+=NB)
            {
                XOR(&(data1[i]), &(data2[i]), &(result[i]));
            }
        }
        double t1= omp_get_wtime()-t0;
        TearDown();
        return t1/num_test;
    }
    /*
    double benchmark3_float_AVX()
    {
        double t0=omp_get_wtime();
        for(int test=0; test<num_test; test++)
        {
            const size_t loop_count=num_data/32*32; // 32 = 256/sizeof(float)*4
            __asm__ __volatile__(
                    ".intel_syntax noprefix           \n\t"
                    " xor     rcx, rcx                \n\t"
                    ".loop_start:                     \n\t"
                    " cmp     rcx, %1                 \n\t"
                    " je      .loop_end               \n\t"
                    " vmovups ymm0,  [%2+4*rcx]       \n\t"
                    " vmovups ymm1,  [%3+4*rcx]       \n\t"
                    " vmovups ymm3,  [%2+4*(rcx+8)]   \n\t"
                    " vmovups ymm4,  [%3+4*(rcx+8)]   \n\t"
                    " vmovups ymm6,  [%2+4*(rcx+16)]  \n\t"
                    " vmovups ymm7,  [%3+4*(rcx+16)]  \n\t"
                    " vmovups ymm9,  [%2+4*(rcx+24)]  \n\t"
                    " vmovups ymm10, [%3+4*(rcx+24)]  \n\t"
                    " vxorps  ymm2, ymm0, ymm1        \n\t"
                    " vxorps  ymm5, ymm3, ymm4        \n\t"
                    " vxorps  ymm8, ymm6, ymm7        \n\t"
                    " vxorps  ymm11, ymm9, ymm10      \n\t"
                    " vmovups [%4+4*rcx] ,      ymm2  \n\t"
                    " vmovups [%4+4*(rcx+8)] ,  ymm5  \n\t"
                    " vmovups [%4+4*(rcx+16)] , ymm8  \n\t"
                    " vmovups [%4+4*(rcx+24)] , ymm11 \n\t"
                    " add     rcx, 32                 \n\t"
                    " jmp     .loop_start             \n\t"
                    " .loop_end:                      \n\t"
                    " mov     rcx, %1                 \n\t"
                    " .reminder_loop_start:           \n\t"
                    " cmp     rcx, %0                 \n\t"
                    " je      .reminder_loop_end      \n\t"
                    " movss   xmm0, [%2+4*rcx]        \n\t"
                    " movss   xmm1, [%3+4*rcx]        \n\t"
                    " pxor    xmm0, xmm1              \n\t"
                    " movss   [%4+4*rcx], xmm0        \n\t"
                    " inc     rcx                     \n\t"
                    " jmp     .reminder_loop_start    \n\t"
                    " .reminder_loop_end:             \n\t"
                    :
                    : "r"(num_data), "r"(loop_count), "r"(data1), "r"(data2), "r"(result)
                    : "memory", "rcx"
                    );
        }
        double t1= omp_get_wtime()-t0;
        TearDown();
        return t1/num_test;
    }
    */

    private:
    T* result;
    T* random_data;
    T* data1;
    T* data2;
    const std::string name;
    size_t num_data;
    const int num_test;
    void re_initialize()
    {
        copy_data(num_data, random_data, data1);
        zero_clear(num_data, data2);
        zero_clear(num_data, result);
    }
    void TearDown()
    {
        std::cerr<<"src1   :";
        output_binary(std::cerr, data1[num_data/2]);
        std::cerr<<"\n";
        std::cerr<<"src2   :";
        output_binary(std::cerr, data2[num_data/2]);
        std::cerr<<"\n";
        std::cerr<<"result :";
        output_binary(std::cerr, result[num_data/2]);
        std::cerr<<"\n";

        int hit_counter=0;
        for(int i=0;i<num_data; ++i)
        {
            if(result[i] != data1[i])
            {
                std::cerr<< i <<" th element is not correctory XORed"<<std::endl;
                hit_counter++;
            }
            if(hit_counter>10)
            {
                break;
            }
        }

        re_initialize();
    }
};

int main(int argc, char *argv[])
{

    XORBenchmark<REAL_TYPE> bm=XORBenchmark<REAL_TYPE>(argc, argv);
    
    std::cout << "Elapsed time for XOR<1>:   "<< bm.benchmark2<real_xor<1>, 1 >() <<" sec"<<std::endl;
    std::cout << "Elapsed time for XOR<2>:   "<< bm.benchmark2<real_xor<2>, 2 >() <<" sec"<<std::endl;
    std::cout << "Elapsed time for XOR<4>:   "<< bm.benchmark2<real_xor<4>, 4 >() <<" sec"<<std::endl;
    std::cout << "Elapsed time for XOR<8>:   "<< bm.benchmark2<real_xor<8>, 8 >() <<" sec"<<std::endl;
    std::cout << "Elapsed time for XOR<16>:  "<< bm.benchmark2<real_xor<16>,16>() <<" sec"<<std::endl;
#if REAL_TYPE == float
    std::cout << "Elapsed time for primitive XOR: "<< bm.benchmark0() <<" sec"<<std::endl; //float only !!
//    std::cout << "Elapsed time for XOR with inline Assembler: "<< bm.benchmark3_float_AVX() <<" sec"<<std::endl; //float only !!
#endif
    std::cout << "Elapsed time for normal XOR: "<< bm.benchmark1() <<" sec"<<std::endl;

    return 0;
}
