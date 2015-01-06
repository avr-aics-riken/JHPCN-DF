/*
 * JHPCN-DF - Data compression library based on
 *            Jointed Hierarchical Precision Compression Number Data Format
 *
 * Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 */

// @file benchmark_common.h

#ifndef BENCHMARK_COMMON_H
#define BENCHMARK_COMMON_H
#if __cplusplus >= 201103L
#include <random>
#endif
#include <string>

#ifndef REAL_TYPE
#define REAL_TYPE float
#endif

namespace
{
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

template <typename T>
void copy_data(const size_t& num_data, T* src, T* dst)
{
    for(size_t i=0; i<num_data; i++)
    {
        dst[i]=src[i];
    }
}

template <typename T>
void zero_clear(const size_t& num_data, T* data)
{
    for(size_t i=0; i<num_data; i++)
    {
        data[i]=0;
    }
}
}
#endif
