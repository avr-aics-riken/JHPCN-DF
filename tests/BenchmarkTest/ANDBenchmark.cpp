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
class ANDBenchmark
{
    public:
    ANDBenchmark(int argc, char *argv[]):name("AND"), num_test(100)
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
    ~ANDBenchmark()
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
	        tmp1.integer &= tmp2.integer;
                result[i] = tmp1.real;
            }
        }
        double t1= omp_get_wtime()-t0;
        TearDown();
        return t1/num_test;
    }
    template <void (*AND)(const T*, const T*, T*), int NB>
    double benchmark2()
    {
        double t0=omp_get_wtime();
        for(int test=0; test<num_test; test++)
        {
#pragma omp parallel for
            for(int i=0;i<num_data; i+=NB)
            {
                AND(&(data1[i]), &(data2[i]), &(result[i]));
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
    T* data2;
    const std::string name;
    size_t num_data;
    const int num_test;
    void re_initialize()
    {
        copy_data(num_data, random_data, data1);
        zero_clear(num_data, data2);
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
        re_initialize();
    }
};

int main(int argc, char *argv[])
{

    ANDBenchmark<REAL_TYPE> bm=ANDBenchmark<REAL_TYPE>(argc, argv);
    std::cout.width(10);
    std::cout.precision(8);
    
    std::cout << "Elapsed time for AND<1>:   "<< bm.benchmark2<real_and<1>, 1 >() <<" sec"<<std::endl;
    std::cout << "Elapsed time for AND<2>:   "<< bm.benchmark2<real_and<2>, 2 >() <<" sec"<<std::endl;
    std::cout << "Elapsed time for AND<4>:   "<< bm.benchmark2<real_and<4>, 4 >() <<" sec"<<std::endl;
    std::cout << "Elapsed time for AND<8>:   "<< bm.benchmark2<real_and<8>, 8 >() <<" sec"<<std::endl;
    std::cout << "Elapsed time for AND<16>:  "<< bm.benchmark2<real_and<16>,16>() <<" sec"<<std::endl;
#if REAL_TYPE == float
    std::cout << "Elapsed time for primitive AND: "<< bm.benchmark0() <<" sec"<<std::endl; //float only !!
#endif

    return 0;
}
