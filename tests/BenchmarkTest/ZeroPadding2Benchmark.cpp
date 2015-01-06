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
    ZeroPaddingBenchmark(int argc, char *argv[]):name("ZeroPadding2"), num_test(100)
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
        copy_data(num_data, random_data, data1);
        double t1=omp_get_wtime()-t0;
        std::cout << "Elapsed time for initialize: "<<t1<<" sec"<<std::endl;
        std::cout << std::endl;
        std::cout << "====" << this->name << " Benchmark start ====" << std::endl;
    }
    ~ZeroPaddingBenchmark()
    {
        delete [] result;
        delete [] data1;
    }

    template <void (*ZERO_PADDING)(const T*, T*), int NB>
    double benchmark2()
    {
        double t0=omp_get_wtime();
        for(int test=0; test<num_test; test++)
        {
#pragma omp parallel for
            for(int i=0;i<num_data; i+=NB)
            {
                ZERO_PADDING(&(data1[i]), &(result[i]));
            }
        }
        double t1= omp_get_wtime()-t0;
        re_initialize();
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
    void re_initialize()
    {
        copy_data(num_data, random_data, data1);
    }
    void TearDown()
    {
        std::cerr<<"src   :";
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
    std::cout.width(10);
    std::cout.precision(8);
#define N_BIT 10
    std::cout << "Test data type = "<<typeid(REAL_TYPE).name()<<std::endl;
    std::cout << "Test data size = "<< argv[1] <<" MiByte"<<std::endl;
    std::cout << "zero padding width = "<<N_BIT<<" bit"<<std::endl;

    auto bm=ZeroPaddingBenchmark<REAL_TYPE>(argc, argv);
    std::cout << "Elapsed time for ZeroPadding<  1,"<<N_BIT<<" >: "<< bm.benchmark2<n_bit_mask<  1, N_BIT>,   1>() <<" sec"<<std::endl;
//    std::cout << "Elapsed time for ZeroPadding<  2,"<<N_BIT<<" >: "<< bm.benchmark2<n_bit_mask<  2, N_BIT>,   2>() <<" sec"<<std::endl;
//    std::cout << "Elapsed time for ZeroPadding<  4,"<<N_BIT<<" >: "<< bm.benchmark2<n_bit_mask<  4, N_BIT>,   4>() <<" sec"<<std::endl;
//    std::cout << "Elapsed time for ZeroPadding<  8,"<<N_BIT<<" >: "<< bm.benchmark2<n_bit_mask<  8, N_BIT>,   8>() <<" sec"<<std::endl;
//    std::cout << "Elapsed time for ZeroPadding< 16,"<<N_BIT<<" >: "<< bm.benchmark2<n_bit_mask< 16, N_BIT>,  16>() <<" sec"<<std::endl;

    return 0;
}
