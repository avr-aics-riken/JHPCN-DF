/*
 * JHPCN-DF - Data compression library based on
 *            Jointed Hierarchical Precision Compression Number Data Format
 *
 * Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 */

// @file ZeroPaddingBenchmark.cpp

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
class ZeroPaddingBenchmark
{
    public:
    ZeroPaddingBenchmark(int argc, char *argv[]):name("ZeroPadding"), num_test(100)
    {
        double t0=omp_get_wtime();
        if(argc !=3)
        {
            std::cerr<<"usage: "<<argv[0]<<" size_of_data_in_MiB number_of_bits"<<std::endl;
            exit(1);
        }
        num_data=std::atol(argv[1])*1024*1024/sizeof(T);
        n_bit=std::atoi(argv[2]);

        random_data= new T [num_data];
        RandomNumber<REAL_TYPE> generator;
        generator(num_data, random_data);
        result = new T [num_data];
        data1  = new T [num_data];
        copy_data(num_data, random_data, data1);
        double t1=omp_get_wtime()-t0;
        std::cout << "Test data type = "<< typeid(T).name()<<std::endl;
        std::cout << "Test data size = "<< argv[1] <<" MiByte"<<std::endl;
        std::cout << "zero padding width = "<< n_bit <<" bit"<<std::endl;
        std::cout << "Elapsed time for initialize: "<<t1<<" sec"<<std::endl;
        std::cout << std::endl;
        std::cout << "====" << this->name << " Benchmark start ====" << std::endl;
    }
    ~ZeroPaddingBenchmark()
    {
        delete [] result;
        delete [] data1;
    }

    double benchmark0()
    {
        double t0=omp_get_wtime();
        for(int test=0; test<num_test; test++)
        {
#pragma omp parallel for
            for(int i=0;i<num_data; i++)
            {
		real4byte tmp;
		tmp.real=data1[i];
		tmp.integer = (tmp.integer >> n_bit)<<n_bit;
                result[i] = tmp.real;
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
                result[i] = n_bit_zero_padding(data1[i], n_bit);
            }
        }
        double t1= omp_get_wtime()-t0;
        TearDown();
        return t1/num_test;
    }

    template <void (*ZERO_PADDING)(const T*, T*, const unsigned int&), int NB>
    double benchmark2()
    {
        double t0=omp_get_wtime();
        for(int test=0; test<num_test; test++)
        {
#pragma omp parallel for
            for(int i=0;i<num_data; i+=NB)
            {
                ZERO_PADDING(&(data1[i]), &(result[i]), n_bit);
            }
        }
        double t1= omp_get_wtime()-t0;
        TearDown();
        return t1/num_test;
    }

    private:
    T* result;
    T* random_data;
    T* data1;
    const std::string name;
    size_t num_data;
    const int num_test;
    int n_bit;
    void re_initialize()
    {
        copy_data(num_data, random_data, data1);
    }
    void TearDown()
    {
        std::cerr<<"src1   :";
        output_binary(std::cerr, data1[num_data/2]);
        std::cerr<<"\n";
        std::cerr<<"result :";
        output_binary(std::cerr, result[num_data/2]);
        std::cerr<<"\n";
        re_initialize();
    }
};

int main(int argc, char *argv[])
{
    ZeroPaddingBenchmark<REAL_TYPE> bm=ZeroPaddingBenchmark<REAL_TYPE>(argc, argv);
    std::cout.width(10);
    std::cout.precision(8);
    
    std::cout << "Elapsed time for ZeroPadding<1>:   "<< bm.benchmark2<n_bit_zero_padding<1>, 1 >() <<" sec"<<std::endl;
    std::cout << "Elapsed time for ZeroPadding<2>:   "<< bm.benchmark2<n_bit_zero_padding<2>, 2 >() <<" sec"<<std::endl;
    std::cout << "Elapsed time for ZeroPadding<4>:   "<< bm.benchmark2<n_bit_zero_padding<4>, 4 >() <<" sec"<<std::endl;
    std::cout << "Elapsed time for ZeroPadding<8>:   "<< bm.benchmark2<n_bit_zero_padding<8>, 8 >() <<" sec"<<std::endl;
    std::cout << "Elapsed time for ZeroPadding<16>:  "<< bm.benchmark2<n_bit_zero_padding<16>,16>() <<" sec"<<std::endl;
#if REAL_TYPE == float
//    std::cout << "Elapsed time for primitive ZeroPading: "<< bm.benchmark0() <<" sec"<<std::endl; //float only !!
#endif
    std::cout << "Elapsed time for normal ZeroPadding: "<< bm.benchmark1() <<" sec"<<std::endl;

    return 0;
}
