/*
 * JHPCN-DF - Data compression library based on
 *            Jointed Hierarchical Precision Compression Number Data Format
 *
 * Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 */

// @file InterFace.cpp

#include "jhpcndf.h"
#include "Utility.h"
#include "FileInfoManager.h"
#include "Encoder.h"
#include "Decoder.h"
#include "IO.h"
#ifdef TIME_MEASURE
#include <omp.h>
#endif

//
// implementation of C++ Interface routines
//
namespace JHPCNDF
{
    template <typename T>
    class InterFaceHelper
    {
            size_t fwrite(T* data, size_t size, const int& key, const float& tolerance, const std::string& enc, const std::string& comp, const bool time_measuring)
            {
#ifdef TIME_MEASURE
                double t0=0.0;
                double t1=0.0;
                if(time_measuring)
                {
                    t0=omp_get_wtime();
                }
#endif
                size_t output_size=0;
                T* work_upper = new T[size];
                T* work_lower = NULL;

                FILE* fp_lower= FileInfoManager::GetInstance().get_lower_file_pointer(key);
                if(fp_lower != NULL)
                {
                    work_lower = new T[size];
                }

#ifdef TIME_MEASURE
                if(time_measuring)
                {
                    t1=omp_get_wtime()-t0;
                    std::cerr<<"elapsed time for initialize: "<<t1<<" sec"<<std::endl;
                    t0=omp_get_wtime();
                }
#endif
                encode<T>(size, data, work_upper, work_lower, tolerance, enc);
#ifdef TIME_MEASURE
                if(time_measuring)
                {
                    t1=omp_get_wtime()-t0;
                    std::cerr<<"elapsed time for encode: "<<t1<<" sec"<<std::endl;
                    t0=omp_get_wtime();
                }
#endif

                FILE* fp_upper = FileInfoManager::GetInstance().get_upper_file_pointer(key);
                IO* io=IOFactory(comp);
                output_size=io->fwrite(work_upper, sizeof(T), size, fp_upper);

                if(fp_lower != NULL)
                {
                    io->fwrite(work_lower, sizeof(T), size, fp_lower);
                }
#ifdef TIME_MEASURE
                if(time_measuring)
                {
                    t1=omp_get_wtime()-t0;
                    std::cerr<<"elapsed time for output: "<<t1<<" sec"<<std::endl;
                }
#endif
                delete [] work_lower;
                delete [] work_upper;
                delete io;
                return output_size;
            }

            size_t fread(T *data, size_t size, const int& key)
            {
                //seek位置を保存
                FILE* fp_upper = FileInfoManager::GetInstance().get_upper_file_pointer(key);
                FILE* fp_lower = FileInfoManager::GetInstance().get_lower_file_pointer(key);
                long org_upper_position=ftell(fp_upper);
                //先頭2byte分を読み込んでヘッダを判定(gzip or not)
                unsigned char work[2];
                if(::fread(work, 1, 2, fp_upper)!=2)
                {
                    std::cerr<<"upper bits file read failed"<<std::endl;
                }
                fseek(fp_upper, org_upper_position, 0);

                //読み込んだヘッダを元にIOクラスを作成
                IO* io;
                if(work[0]==0x1f && work[1] == 0x8b)
                {
                    io=IOFactory("gzip");
                }else{
                    io=IOFactory("stdio");
                }

                io->fread(data, sizeof(T), size, fp_upper);
                if(fp_lower!=NULL)
                {
                    T* work=new T [size];
                    T* lower=new T [size];
                    io->fread(lower, sizeof(T), size, fp_lower);
                    decode<T>(size, data, lower, work);
                    delete lower;
                    for(size_t i=0;i<size;i++)
                    {
                        data[i]=work[i];
                    }
                    delete work;
                }
                delete io;
            }
            friend size_t fwrite(const void *ptr, size_t size, size_t nmemb,  const int& key, const float& tolerance, const std::string& enc, const std::string& comp, const bool time_measuring);
            friend size_t fread(void *ptr, size_t size, size_t nmemb, const int& key);


    };

    int fopen(const std::string& filename_upper, const std::string& filename_lower, const char * mode)
    {
        return FileInfoManager::GetInstance().create_new_entry(filename_upper, filename_lower, -1, mode);
    }
    void fclose(const int& key)
    {
        FileInfoManager::GetInstance().destroy_entry(key);
    }
    size_t fwrite(const void *ptr, size_t size, size_t nmemb,  const int& key, const float& tolerance, const std::string& enc, const std::string& comp, const bool time_measuring)
    {
        if(size==sizeof(float))
        {
            InterFaceHelper<float> helper;
            return helper.fwrite((float*)ptr, nmemb, key, tolerance, enc, comp, time_measuring);
        }else if(size==sizeof(double)){
            InterFaceHelper<double> helper;
            return helper.fwrite((double*)ptr, nmemb, key, tolerance, enc, comp, time_measuring);
        }else{
            std::cerr<<"unsupported data lengh: "<<size<<std::endl;
            return 0;
        }
    }

    size_t fread(void *ptr, size_t size, size_t nmemb, const int& key)
    {
        if(size==sizeof(float))
        {
            InterFaceHelper<float> helper;
            return helper.fread((float*)ptr, nmemb, key);
        }else if(size==sizeof(double)){
            InterFaceHelper<double> helper;
            return helper.fread((double*)ptr, nmemb, key);
        }else{
            std::cerr<<"unsupported data lengh: "<<size<<std::endl;
            return 0;
        }
    }

    template <typename T>
    void encode(const size_t& length, const T* const src, T* const dst, T* const dst_lower, const float& tolerance, const std::string& enc, const bool time_measuring)
    {
        Encoder<T>* tmp=EncoderFactory<T>(enc, tolerance);
        Encoder<T>& encoder = *tmp;
        encoder(length, src, dst, dst_lower);
        delete tmp;
    }

    template <typename T>
    void decode(const size_t& length, const T* const src_upper, const T* const src_lower, T* const dst)
    {
        Decoder<T> decoder;
        decoder(length, src_upper, src_lower, dst);
    }
}

//
// implementation of C Interface routines
//
int JHPCNDF_fopen(const char* filename_upper, const char* filename_lower, const char * mode)
{
    return JHPCNDF::fopen(filename_upper, filename_lower, mode);
}
void JHPCNDF_fclose(const int key)
{
    JHPCNDF::fclose(key);
}
size_t JHPCNDF_fwrite(const void *ptr, size_t size, size_t nmemb, const int key, const float tolerance, const char* enc, const char* comp)
{
    return JHPCNDF::fwrite(ptr, size, nmemb, key, tolerance, enc, comp);
}
size_t JHPCNDF_fread(void *ptr, size_t size, size_t nmemb, const int key)
{
    return JHPCNDF::fread(ptr, size, nmemb, key);
}
void JHPCNDF_encode_float(const size_t& length, const float* const src, float* const dst, float* const dst_lower, const char enc)
{
    JHPCNDF::encode<float>(length, src, dst, dst_lower, enc);
}
void JHPCNDF_encode_double(const size_t& length, const double* const src, double* const dst, double* const dst_lower, const char enc)
{
    JHPCNDF::encode<double>(length, src, dst, dst_lower, enc);
}
void JHPCNDF_decode_float(const size_t& length, const float* const src_upper, const float* const src_lower, float* const dst)
{
    JHPCNDF::decode<float>(length, src_upper, src_lower, dst);
}
void JHPCNDF_decode_double(const size_t& length, const double* const src_upper, const double* const src_lower, double* const dst)
{
    JHPCNDF::decode<double>(length, src_upper, src_lower, dst);
}

//
// implementation of Fortran Interface routines
//
