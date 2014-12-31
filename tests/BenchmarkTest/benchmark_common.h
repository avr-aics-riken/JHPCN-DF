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
#include <random>
#include <string>

#ifndef REAL_TYPE
#define REAL_TYPE float
#endif

template <typename T>
T* initialize_data(const size_t& num_data)
{
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    std::uniform_real_distribution<> dist(-1.0, 1.0);

    T* data= new T[num_data];
    for(size_t i=0; i<num_data; i++)
    {
        data[i]=dist(engine);
    }

    return data;
}

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
#endif
