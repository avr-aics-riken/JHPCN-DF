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
        num_data=std::stoull(argv[1])*1024*1024/sizeof(T);

        random_data = initialize_data<T>(num_data);
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
        for(int i=0; i<num_test; i++)
        {
            for(int j=0;j<num_data; j++)
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
        re_initialize();
        return t1;
    }
    double benchmark1()
    {
        double t0=omp_get_wtime();
        for(int i=0; i<num_test; i++)
        {
            for(int j=0;j<num_data; j++)
            {
                result[i] = real_xor(data1[i], data2[i]);
            }
        }
        double t1= omp_get_wtime()-t0;
        re_initialize();
        return t1;
    }

    template <void (*XOR)(const T*, const T*, T*), int NB>
    double benchmark2()
    {
        double t0=omp_get_wtime();
        for(int i=0; i<num_test; i++)
        {
            for(int j=0;j<num_data/NB; j++)
            {
                XOR(&(data1[i]), &(data2[i]), &(result[i]));
            }
        }
        double t1= omp_get_wtime()-t0;
        re_initialize();
        return t1;
    }

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
    }
};

int main(int argc, char *argv[])
{

    auto bm=XORBenchmark<REAL_TYPE>(argc, argv);
    
#if REAL_TYPE == float
    std::cout << "Elapsed time for primitive XOR: "<< bm.benchmark0() <<" sec"<<std::endl; //float only !!
#endif
    std::cout << "Elapsed time for normal XOR: "<< bm.benchmark1() <<" sec"<<std::endl;
    std::cout << "Elapsed time for XOR<1>:  "<< bm.benchmark2<real_xor<1>, 1 >() <<" sec"<<std::endl;
    std::cout << "Elapsed time for XOR<2>:  "<< bm.benchmark2<real_xor<2>, 2 >() <<" sec"<<std::endl;
    std::cout << "Elapsed time for XOR<4>:  "<< bm.benchmark2<real_xor<4>, 4 >() <<" sec"<<std::endl;
    std::cout << "Elapsed time for XOR<8>:  "<< bm.benchmark2<real_xor<8>, 8 >() <<" sec"<<std::endl;
    std::cout << "Elapsed time for XOR<16>: "<< bm.benchmark2<real_xor<16>,16>() <<" sec"<<std::endl;
    std::cout << "Elapsed time for XOR<32>: "<< bm.benchmark2<real_xor<32>,32>() <<" sec"<<std::endl;
    std::cout << "Elapsed time for XOR<64>: "<< bm.benchmark2<real_xor<64>,64>() <<" sec"<<std::endl;
    std::cout << "Elapsed time for XOR<128>: "<< bm.benchmark2<real_xor<128>,128>() <<" sec"<<std::endl;
    std::cout << "Elapsed time for XOR<256>: "<< bm.benchmark2<real_xor<256>,256>() <<" sec"<<std::endl;

    return 0;
}
