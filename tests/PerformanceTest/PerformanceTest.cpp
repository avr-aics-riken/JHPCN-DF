/*
 * JHPCN-DF - Data compression library based on
 *            Jointed Hierarchical Precision Compression Number Data Format
 *
 * Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 */

// @file PerformanceTest.cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include "jhpcndf.h"
#include "Utility.h"
#include <omp.h>
#include "real_type.h"
#if __cplusplus >= 201103L
#include <random>
#endif

namespace{
    template<typename T>
    class RandomNumber
    {
public:
            RandomNumber()
            {
#if __cplusplus >= 201103L
                std::random_device seed_gen;
                engine=std::mt19937_64(seed_gen());
                dist=std::uniform_real_distribution<T>(0.0,1.0);
#endif
            }
            void operator ()(const size_t& length,  T* data)
            {
                for(size_t i=0; i<length; i++)
                {
#if __cplusplus >= 201103L
                    data[i]=dist(engine);
#else
                    data[i]=(T)std::rand()/((T)(RAND_MAX)+1);
#endif
                }
            }
private:
#if __cplusplus >= 201103L
            std::mt19937_64 engine;
            std::uniform_real_distribution<T> dist;
#endif
    };
}

int main(int argc, char *argv[])
{
    double t0=omp_get_wtime();
    if(argc < 3)
    {
        std::cerr<<"usage: "<<argv[0]<<" number_of_data min_tolerance encoder_name..." << std::endl;
        std::cerr<<"  encoder=[normal | byte_aligned | linear_search | binary_search | nbit_filter]" << std::endl;
        exit(1);
    }

    const size_t num_data=std::atol(argv[1]);

    REAL_TYPE* random_data= new REAL_TYPE [num_data];
    RandomNumber<REAL_TYPE> generator;
    generator(num_data, random_data);

    float tolerance=(float)std::atof(argv[2]);
    double t_init=omp_get_wtime()-t0;

    int key=JHPCNDF::fopen("input_data.gz", "", "w+b");
    JHPCNDF::fwrite(random_data, sizeof(REAL_TYPE), num_data, key, 1.0, "dummy", "gzip");
    JHPCNDF::fclose(key);

    std::cerr << "Test data size = "<< num_data*sizeof(REAL_TYPE) <<" Byte"<<std::endl;
    std::cerr << "Elapsed time for initialize: "<<t_init<<" sec"<<std::endl;

    std::vector<std::string> encorders;
    for(int i=3; i<argc;i++)
    {
        encorders.push_back(argv[i]);
    }

    std::vector<std::string> filenames;
    const std::string filename("encoded");
    while(tolerance<=1)
    {
        std::cerr<< "tolerance = "<<tolerance<<std::endl;
        for(std::vector<std::string>::iterator it=encorders.begin(); it!=encorders.end(); ++it)
        {
            std::cerr<<"Test for "<<*it<<" encoder"<<std::endl;

            std::ostringstream oss;
            oss<<tolerance;
            filenames.push_back(filename+"_"+*it+"_"+oss.str()+".gz");
            int key=JHPCNDF::fopen(filename+"_"+*it+"_"+oss.str()+".gz", filename+"_"+*it+"_"+oss.str()+"lower"+".gz", "w+b");
            JHPCNDF::fwrite(random_data, sizeof(REAL_TYPE), num_data, key, tolerance, true, *it, "gzip", true);
            JHPCNDF::fclose(key);
            std::cerr<< std::endl;
        }
        tolerance*=10;
    }
    std::cerr<< std::endl;


    // 最も長いファイル名の長さを求める
    size_t max=0;
    for(std::vector<std::string>::iterator it=filenames.begin(); it!=filenames.end(); ++it)
    {
        max = it->size() > max ? it->size() : max;
    }
    std::cerr<< "dump "<<num_data/2<<" th data"<<std::endl;
    std::cerr<<std::setw(max) <<"original data"<<" : ";
    std::cerr<<std::scientific<<random_data[num_data/2]<<" : ";
    output_binary(std::cerr, random_data[num_data/2]);
    std::cerr<<std::endl;
    delete [] random_data;

    REAL_TYPE* work = new REAL_TYPE [num_data];
    for(std::vector<std::string>::iterator it=filenames.begin(); it!=filenames.end(); ++it)
    {
        std::cerr<<std::setw(max) << *it <<" : ";
        int key=JHPCNDF::fopen(*it, "", "rb");
        JHPCNDF::fread(work, sizeof(REAL_TYPE), num_data, key);
        JHPCNDF::fclose(key);
        std::cerr<<std::scientific<<work[num_data/2]<<" : ";
        output_binary(std::cerr, work[num_data/2]);
        std::cerr<<std::endl;
    }

    delete [] work;
    return 0;
}
