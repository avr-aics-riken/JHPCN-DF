/*
 * JHPCN-DF - Data compression library based on 
 *            Jointed Hierarchical Precision Compression Number Data Format
 *
 * Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 */

// @file jhpcndf.h


#ifndef INTERFACE_H
#define INTERFACE_H
//
// Interface routines for C++
//
#ifdef __cplusplus
#include <string>
namespace JHPCNDF
{
int fopen(const std::string& filename_upper, const std::string& filename_lower = "", const char* mode = "rb");
void fclose(const int& key);
size_t fwrite(const void* ptr, size_t size, size_t nmemb, const int& key, const float& tolerance, const std::string& enc, const std::string& comp="gzip", const bool time_measuring=false);
size_t fread(void* ptr, size_t size, size_t nmemb, const int& key);
template<typename T>
void encode(const size_t& length, const T* const src, T* const dst, T* const dst_lower, const float& tolerance, const std::string& enc = "normal", const bool time_measuring=false);
template<typename T>
void decode(const size_t& length, const T* const src_upper, const T* const src_lower, T* const dst);
} //end of namespace JHPCNDF
#endif

//
// Interface routines for C
//
#ifdef __cplusplus
extern "C"
{
#endif
int JHPCNDF_fopen(const char* filename_upper, const char* filename_lower, const char* mode);
void JHPCNDF_fclose(const int key);
size_t JHPCNDF_fwrite(const void* ptr, size_t size, size_t nmemb, const int key, const float tolerance, const char* enc, const char* comp);
size_t JHPCNDF_fread(void* ptr, size_t size, size_t nmemb, const int key);
void JHPCNDF_encode_float(const size_t& length, const float* const src, float* const dst, float* const dst_lower, const char enc);
void JHPCNDF_encode_double(const size_t& length, const double* const src, double* const dst, double* const dst_lower, const char enc);
void JHPCNDF_decode_float(const size_t& length, const float* const src_upper, const float* const src_lower, float* const dst);
void JHPCNDF_decode_double(const size_t& length, const double* const src_upper, const double* const src_lower, double* const dst);
#ifdef __cplusplus
}
#endif
#endif
