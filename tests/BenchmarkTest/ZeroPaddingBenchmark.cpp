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
        num_data=std::stoull(argv[1])*1024*1024/sizeof(T);
        n_bit=std::stoi(argv[2]);

        random_data = initialize_data<T>(num_data);
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
        for(int i=0; i<num_test; i++)
        {
            for(int j=0;j<num_data; j++)
            {
		real4byte tmp;
		tmp.real=data1[i];
		tmp.integer = (tmp.integer >> n_bit)<<n_bit;
                result[i] = tmp.real;
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
                result[i] = n_bit_zero_padding(data1[i], n_bit);
            }
        }
        double t1= omp_get_wtime()-t0;
        re_initialize();
        return t1;
    }

    template <void (*ZERO_PADDING)(const T*, T*, const unsigned int&), int NB>
    double benchmark2()
    {
        double t0=omp_get_wtime();
        for(int i=0; i<num_test; i++)
        {
            for(int j=0;j<num_data/NB; j++)
            {
                ZERO_PADDING(&(data1[i]), &(result[i]), n_bit);
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
    const std::string name;
    size_t num_data;
    const int num_test;
    int n_bit;
    void re_initialize()
    {
        copy_data(num_data, random_data, data1);
    }
};

int main(int argc, char *argv[])
{
    auto bm=ZeroPaddingBenchmark<REAL_TYPE>(argc, argv);
    
#if REAL_TYPE == float
    std::cout << "Elapsed time for primitive ZeroPading: "<< bm.benchmark0() <<" sec"<<std::endl; //float only !!
#endif
    std::cout << "Elapsed time for normal ZeroPadding: "<< bm.benchmark1() <<" sec"<<std::endl;
    std::cout << "Elapsed time for ZeroPadding<1>:  "<< bm.benchmark2<n_bit_zero_padding<1>, 1 >() <<" sec"<<std::endl;
    std::cout << "Elapsed time for ZeroPadding<2>:  "<< bm.benchmark2<n_bit_zero_padding<2>, 2 >() <<" sec"<<std::endl;
    std::cout << "Elapsed time for ZeroPadding<4>:  "<< bm.benchmark2<n_bit_zero_padding<4>, 4 >() <<" sec"<<std::endl;
    std::cout << "Elapsed time for ZeroPadding<8>:  "<< bm.benchmark2<n_bit_zero_padding<8>, 8 >() <<" sec"<<std::endl;
    std::cout << "Elapsed time for ZeroPadding<16>: "<< bm.benchmark2<n_bit_zero_padding<16>,16>() <<" sec"<<std::endl;
    std::cout << "Elapsed time for ZeroPadding<32>: "<< bm.benchmark2<n_bit_zero_padding<32>,32>() <<" sec"<<std::endl;
    std::cout << "Elapsed time for ZeroPadding<64>: "<< bm.benchmark2<n_bit_zero_padding<64>,64>() <<" sec"<<std::endl;
    std::cout << "Elapsed time for ZeroPadding<128>: "<< bm.benchmark2<n_bit_zero_padding<128>,128>() <<" sec"<<std::endl;
    std::cout << "Elapsed time for ZeroPadding<256>: "<< bm.benchmark2<n_bit_zero_padding<256>,256>() <<" sec"<<std::endl;

    return 0;
}
