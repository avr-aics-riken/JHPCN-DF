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

int main(int argc, char *argv[])
{
    double t0=omp_get_wtime();
    if(argc !=3)
    {
        std::cerr<<"usage: "<<argv[0]<<" size_of_data_in_kiB tolerance"<<std::endl;
        exit(1);
    }
    /*
    C++0x 以降用の<random>が必要
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());

    std::uniform_real_distribution<> dist(-1.0, 1.0);
    */

    const size_t num_data=std::atol(argv[1])*1024/sizeof(REAL_TYPE);

    REAL_TYPE* random_data= new REAL_TYPE [num_data];
    for(size_t i=0; i<num_data; i++)
    {
//        random_data[i]=dist(engine);
        random_data[i]=(REAL_TYPE)std::rand()/((REAL_TYPE)(RAND_MAX)+1);
    }

    REAL_TYPE* encoded_data = new REAL_TYPE [num_data];
    float tolerance=(float)std::atof(argv[2]);
    double t_init=omp_get_wtime()-t0;

    int key=JHPCNDF::fopen("input_data.gz", "", "w+b");
    JHPCNDF::fwrite(random_data, sizeof(REAL_TYPE), num_data, key, 1.0, "dummy", "gzip");
    JHPCNDF::fclose(key);

    std::cerr << "Test data size = "<< argv[1] <<" kiByte"<<std::endl;
    std::cerr << "Elapsed time for initialize: "<<t_init<<" sec"<<std::endl;

    std::vector<std::string> encorders;
    encorders.push_back("normal");
    encorders.push_back("byte_aligned");

    const std::string filename("encoded");
    std::cerr<< "output "<<num_data/2<<" th element as binary"<<std::endl;
    output_binary(random_data[num_data/2]);
    while(tolerance<1)
    {
        for(std::vector<std::string>::iterator it=encorders.begin(); it!=encorders.end(); ++it)
        {
            std::cerr<< "Test for "<<*it<<" encoder"<<std::endl;

            std::ostringstream oss;
            oss<<tolerance;
            int key=JHPCNDF::fopen(filename+"_"+*it+oss.str()+".gz", filename+"_"+*it+oss.str()+"lower"+".gz", "w+b");
            JHPCNDF::fwrite(random_data, sizeof(REAL_TYPE), num_data, key, tolerance, *it, "gzip", true);
            JHPCNDF::fclose(key);
        }
        tolerance*=10;
    }
    std::cerr<< std::endl;

    delete [] random_data;
    delete [] encoded_data;
    return 0;
}
