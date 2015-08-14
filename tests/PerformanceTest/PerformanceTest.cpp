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
#include <unistd.h>
#include "jhpcndf.h"
#include "Utility.h"


//計時関数
#ifdef USE_OPENMP
#include <omp.h>
#else
#include <sys/time.h>
#endif
namespace{
double wtime(void)
{
#ifdef USE_OPENMP
  return omp_get_wtime();
#else
  struct timeval  tv;
  gettimeofday(&tv, NULL);
  return (double)tv.tv_sec+(double)tv.tv_usec*1e-6;
#endif
}
}

// 乱数生成
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

void usage_and_exit(const char* cmd, const int& err_code)
{
  std::cerr<<"usage: "<<cmd<<" [-t tolerance] [-b buffer_size] [-e encoder] [-n number_of_data] "<< std::endl;
  exit(err_code);
}

void argument_parser(int argc, char *argv[], float* tolerance, size_t* buffer_size, std::string* encoder, size_t* num_data, std::string* comp)
{
  int results=0;
  while((results=getopt(argc,argv,"t:b:e:n:c:")) != -1)
  {
    switch(results)
    {
      case 't':
        *tolerance=(float)std::atof(optarg);
        break;
      case 'b':
        *buffer_size=std::atol(optarg);
        break;
      case 'e':
        *encoder=optarg;
        break;
      case 'n':
        *num_data=std::atol(optarg);
        break;
      case 'c':
        *comp=optarg;
        break;
      case '?':
        usage_and_exit(argv[0], -1);
        break;
    }
  }
}

int main(int argc, char *argv[])
{
  double t0=wtime();
  float tolerance=0.5;
  size_t buffer_size=32768;
  std::string encoder="binary_search";
  size_t num_data=1000;
  std::string comp="gzip";

  argument_parser(argc, argv, &tolerance, &buffer_size, &encoder, &num_data, &comp);

  std::cerr <<"=========================================="<<std::endl;
  std::cerr <<" Test settings"<<std::endl;
  std::cerr <<"   data size   = "<< num_data*sizeof(REAL_TYPE) <<" Byte"<<std::endl;
  std::cerr <<"   encoder     = "<<encoder<<std::endl;
  std::cerr <<"   tolerance   = "<<tolerance<<std::endl;
  std::cerr <<"   buffer size = "<<buffer_size <<" Byte"<<std::endl; 
  std::cerr <<"   compression method = "<<comp<<std::endl;
  std::cerr <<"=========================================="<<std::endl<<std::endl;

  REAL_TYPE* random_data= new REAL_TYPE [num_data];
  RandomNumber<REAL_TYPE> generator;
  generator(num_data, random_data);

  std::string filename("input_data");
  int key=JHPCNDF::fopen(filename+"_"+comp, "", "w+b", comp);
  JHPCNDF::fwrite(random_data, sizeof(REAL_TYPE), num_data, key, 1.0, true, "dummy");
  JHPCNDF::fclose(key);

  double t_init=wtime()-t0;
  std::cerr << "Elapsed time for initialize performance test: "<<t_init<<" sec"<<std::endl;

  std::ostringstream oss1;
  std::ostringstream oss2;
  oss1<<tolerance;
  oss2<<buffer_size;
  filename=encoder+"_"+comp+"_"+oss1.str()+"_"+oss2.str();
  key=JHPCNDF::fopen(filename, filename+"_lower", "w+b", comp, buffer_size);
  JHPCNDF::fwrite(random_data, sizeof(REAL_TYPE), num_data, key, tolerance, true, encoder, true);
  JHPCNDF::fclose(key);

  std::cerr<<std::endl;
  std::cerr<< "dump "<<num_data/2<<" th data"<<std::endl;
  std::cerr<<std::setw(filename.size()+1)<<"original data"<<" : "<<std::scientific<<random_data[num_data/2]<<" : ";
  output_binary(std::cerr, random_data[num_data/2]);
  std::cerr<<std::endl;
  delete [] random_data;

  REAL_TYPE* work = new REAL_TYPE [num_data];
  key=JHPCNDF::fopen(filename, "", "rb", comp);
  JHPCNDF::fread(work, sizeof(REAL_TYPE), num_data, key);
  JHPCNDF::fclose(key);

  std::cerr<<std::setw(filename.size()+1)<<filename <<" : "<<std::scientific<<work[num_data/2]<<" : ";
  output_binary(std::cerr, work[num_data/2]);
  std::cerr<<std::endl;

  delete [] work;
  return 0;
}
