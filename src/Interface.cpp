/*
 * JHPCN-DF - Data compression library based on
 *            Jointed Hierarchical Precision Compression Number Data Format
 *
 * Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 */

// @file InterFace.cpp

#include <algorithm>
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
  namespace
  {
    template <typename T>
      size_t fwrite_helper(const T* data, size_t size, size_t nmemb, const int& key, const float& tolerance, const bool& is_relative, const std::string& enc, const bool& time_measuring, const bool& byte_swap)
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
        T* work_upper = NULL;
        T* work_lower = NULL;
        try
        {
          work_upper = new T[nmemb];
        }
        catch (std::bad_alloc)
        {
          std::cerr<<"can't allocate working memory for encode"<<std::endl;
          return 0;
        }

        FILE* fp_lower= FileInfoManager::GetInstance().get_lower_file_pointer(key);
        if(fp_lower != NULL)
        {
          try
          {
            work_lower = new T[nmemb];
          }
          catch (std::bad_alloc)
          {
            std::cerr<<"can't allocate working memory for encode"<<std::endl;
            delete [] work_upper;
            return 0;
          }
        }

#ifdef TIME_MEASURE
        if(time_measuring)
        {
          t1=omp_get_wtime()-t0;
          std::cerr<<"elapsed time for initialize: "<<t1<<" sec"<<std::endl;
          t0=omp_get_wtime();
        }
#endif
        // encode関数内部で計時しているので、この部分は計時しない
        encode<T>(nmemb, data, work_upper, work_lower, tolerance, is_relative, enc, time_measuring);
#ifdef TIME_MEASURE
        if(time_measuring)
        {
          t0=omp_get_wtime();
        }
#endif
        FILE* fp_upper = FileInfoManager::GetInstance().get_upper_file_pointer(key);
        if(byte_swap)
        {
          convert_endian<sizeof(T)>((char*)work_upper, nmemb);
        }
        FileInfoManager& FIM=FileInfoManager::GetInstance();
        IO* io=IOFactory(FIM.get_compression_method(key), FIM.get_buff_size(key));
        output_size=io->fwrite(work_upper, size, nmemb, fp_upper);
#ifdef TIME_MEASURE
        if(time_measuring)
        {
          t1=omp_get_wtime()-t0;
          std::cerr<<"elapsed time for output: "<<t1<<" sec"<<std::endl;
          t0=omp_get_wtime();
        }
#endif
        if(fp_lower != NULL)
        {
          if(byte_swap)
          {
            convert_endian<sizeof(T)>((char*)work_lower, nmemb);
          }
          io->fwrite(work_lower, size, nmemb, fp_lower);
#ifdef TIME_MEASURE
          if(time_measuring)
          {
            t1=omp_get_wtime()-t0;
            std::cerr<<"elapsed time for output lower bits: "<<t1<<" sec"<<std::endl;
          }
#endif
        }
        delete [] work_lower;
        delete [] work_upper;
        delete io;
        return output_size;
      }

    template <typename T>
      size_t fread_helper(T *data, size_t size, const int& key, const bool& byte_swap)
      {
        FileInfoManager& FIM=FileInfoManager::GetInstance();
        IO* io=IOFactory(FIM.get_compression_method(key), FileInfoManager::GetInstance().get_buff_size(key));
        FILE* fp_upper = FileInfoManager::GetInstance().get_upper_file_pointer(key);
        size_t read_size=io->fread(data, sizeof(T), size, fp_upper);

        FILE* fp_lower= FileInfoManager::GetInstance().get_lower_file_pointer(key);
        if(fp_lower!=NULL)
        {
          T* work;
          T* lower;
          try
          {
            work=new T [size];
          }
          catch (std::bad_alloc)
          {
            std::cerr<<"can't allocate working memory for decode"<<std::endl;
            return read_size;
          }
          try
          {
            lower=new T [size];
          }
          catch (std::bad_alloc)
          {
            std::cerr<<"can't allocate working memory for decode"<<std::endl;
            delete lower;
            return read_size;
          }
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
        if(byte_swap)
        {
          convert_endian<sizeof(T)>((char*)data, size);
        }
        return read_size;
      }
  }//end of unnamed namespace

  int fopen(const std::string& filename_upper, const std::string& filename_lower, const char* mode, const std::string& comp, const size_t& buff_size)
  {
    return FileInfoManager::GetInstance().create_new_entry(filename_upper, filename_lower, -1, mode, comp, buff_size);
  }
  void fclose(const int& key)
  {
    FileInfoManager::GetInstance().destroy_entry(key);
  }

  template <typename T>
    size_t fwrite(const T* ptr, size_t size, size_t nmemb, const int& key, const float& tolerance, const bool& is_relative, const std::string& enc, const bool& time_measuring, const bool& byte_swap)
    {
      FileInfoManager& FIM=FileInfoManager::GetInstance();
      FILE* fp_upper = FIM.get_upper_file_pointer(key);
      T* work;
      if(byte_swap)
      {
        work = new T[nmemb];
        for(int i =0; i<nmemb; i++)
        {
          work[i]=ptr[i];
        }
        convert_endian<sizeof(T)>((char*)work, nmemb);
      }else{
        work=const_cast<T *>(ptr);
      }

      IO* io=IOFactory(FIM.get_compression_method(key), FIM.get_buff_size(key));
      const size_t output_size=io->fwrite(work, size, nmemb, fp_upper);

      FILE* fp_lower= FileInfoManager::GetInstance().get_lower_file_pointer(key);
      if(fp_lower != NULL)
      {
        io->fwrite(work, size, nmemb, fp_lower);
      }
      delete io;
      return output_size;
    }
  template <>
    size_t fwrite(const float* ptr, size_t size, size_t nmemb,  const int& key, const float& tolerance, const bool& is_relative, const std::string& enc, const bool& time_measuring, const bool& byte_swap)
    {
      return fwrite_helper(ptr, size, nmemb, key, tolerance, is_relative, enc, time_measuring, byte_swap);
    }
  template <>
    size_t fwrite(const double* ptr, size_t size, size_t nmemb,  const int& key, const float& tolerance, const bool& is_relative, const std::string& enc, const bool& time_measuring, const bool& byte_swap)
    {
      return fwrite_helper(ptr, size, nmemb, key, tolerance, is_relative, enc, time_measuring, byte_swap);
    }

  template <typename T>
    size_t fread(T* ptr, size_t size, size_t nmemb, const int& key, const bool& byte_swap)
    {
      FileInfoManager& FIM=FileInfoManager::GetInstance();
      //seek位置を保存
      FILE* fp_upper = FIM.get_upper_file_pointer(key);
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
        io=IOFactory("gzip", FIM.get_buff_size(key));
      }else{
        io=IOFactory("stdio", FIM.get_buff_size(key));
      }

      size_t read_size=io->fread(ptr, size, nmemb, fp_upper);
      if(byte_swap)
      {
        convert_endian<sizeof(T)>((char*)ptr, size);
      }
      delete io;
      return 0;
    }

  template <>
    size_t fread(float* ptr, size_t size, size_t nmemb, const int& key, const bool& byte_swap)
    {
      return fread_helper(ptr, nmemb, key, byte_swap);
    }
  template <>
    size_t fread(double* ptr, size_t size, size_t nmemb, const int& key, const bool& byte_swap)
    {
      return fread_helper(ptr, nmemb, key, byte_swap);
    }

  template <typename T>
    void encode(const size_t& length, const T* const src, T* const dst, T* const dst_lower, const float& tolerance, const bool& is_relative, const std::string& enc, const bool time_measuring)
    {
#ifdef TIME_MEASURE
      double t0=0.0;
      double t1=0.0;
      if(time_measuring)
      {
        t0=omp_get_wtime();
      }
#endif
      Encoder<T>* tmp=EncoderFactory<T>(enc, tolerance, is_relative);
      Encoder<T>& encoder = *tmp;
      encoder(length, src, dst, dst_lower);
#ifdef TIME_MEASURE
      if(time_measuring)
      {
        t1=omp_get_wtime()-t0;
        std::cerr<<"elapsed time for encode: "<<t1<<" sec"<<std::endl;
      }
#endif
      delete tmp;
    }

  template <typename T>
    void decode(const size_t& length, const T* const src_upper, const T* const src_lower, T* const dst)
    {
      Decoder<T> decoder;
      decoder(length, src_upper, src_lower, dst);
    }
}//end of namespace JHPCNDF

//
// implementation of C Interface routines
//
int JHPCNDF_fopen(const char* filename_upper, const char* filename_lower, const char* mode, const char* comp, const size_t buff_size)
{
  return JHPCNDF::fopen(filename_upper, filename_lower, mode, comp, buff_size);
}
void JHPCNDF_fclose(const int key)
{
  JHPCNDF::fclose(key);
}

size_t JHPCNDF_fwrite_float(const float* ptr, size_t size, size_t nmemb, const int key, const float tolerance, const int is_relative, const char* enc)
{
  return JHPCNDF::fwrite(ptr, size, nmemb, key, tolerance, is_relative, enc);
}
size_t JHPCNDF_fwrite_double(const double *ptr, size_t size, size_t nmemb, const int key, const float tolerance, const int is_relative, const char* enc)
{
  return JHPCNDF::fwrite(ptr, size, nmemb, key, tolerance, is_relative, enc);
}
size_t JHPCNDF_fwrite(const void *ptr, size_t size, size_t nmemb, const int key, const char* enc)
{
  return JHPCNDF::fwrite((char*)ptr, 1, size*nmemb, key, 0.1, enc);
}
size_t JHPCNDF_fread_float(float* ptr, size_t size, size_t nmemb, const int key)
{
  return JHPCNDF::fread(ptr, size, nmemb, key);
}
size_t JHPCNDF_fread_double(double *ptr, size_t size, size_t nmemb, const int key)
{
  return JHPCNDF::fread(ptr, size, nmemb, key);
}
size_t JHPCNDF_fread(void *ptr, size_t size, size_t nmemb, const int key)
{
  return JHPCNDF::fread((char*)ptr, 1, size*nmemb, key);
}
void JHPCNDF_encode_float(const size_t length, const float* const src, float* const dst, float* const dst_lower, const float tolerance, const int is_relative, const char* enc)
{
  JHPCNDF::encode<float>(length, src, dst, dst_lower, tolerance, is_relative, enc);
}
void JHPCNDF_encode_double(const size_t length, const double* const src, double* const dst, double* const dst_lower, const float tolerance, const int is_relative, const char* enc)
{
  JHPCNDF::encode<double>(length, src, dst, dst_lower, tolerance, is_relative, enc);
}
void JHPCNDF_decode_float(const size_t length, const float* const src_upper, const float* const src_lower, float* const dst)
{
  JHPCNDF::decode<float>(length, src_upper, src_lower, dst);
}
void JHPCNDF_decode_double(const size_t length, const double* const src_upper, const double* const src_lower, double* const dst)
{
  JHPCNDF::decode<double>(length, src_upper, src_lower, dst);
}

//
// implementation of Fortran Interface routines
//
extern "C"
{
  //subroutine jhpcndf_open(unit, ufile, lfile, action, comp, buff_size)
  void jhpcndf_open__(int* unit, const char* ufile, const char* lfile, const char* action, const char* comp, const size_t buff_size)
  {
    std::string mode(action);
    std::transform(mode.begin(), mode.end(), mode.begin(), ::tolower);
    mode=mode=="write"?"w+b":"rb";
    *unit = JHPCNDF::fopen(ufile, lfile, mode.c_str(), comp, buff_size);
  }

  //subroutine jhpcndf_close(unit)
  void jhpcndf_close__(int* unit)
  {
    JHPCNDF::fclose(*unit);
  }

  //subroutine jhpcndf_write_real4(unit, recl, data, tol, enc)
  void jhpcndf_write_real4__(int* unit, size_t* recl, float* data, float* tolerance, bool* is_relative, const char* enc)
  {
    JHPCNDF::fwrite(data, sizeof(float), *recl, *unit, *tolerance, *is_relative, enc);
  }
  //subroutine jhpcndf_write_real8(unit, recl, data, tol, enc)
  void jhpcndf_write_real8__(int* unit, size_t* recl, float* data, float* tolerance, bool* is_relative, const char* enc)
  {
    JHPCNDF::fwrite(data, sizeof(double), *recl, *unit, *tolerance, *is_relative, enc);
  }
  //subroutine jhpcndf_write_integer4(unit, recl, data, tol, enc)
  void jhpcndf_write_integer4__(int* unit, size_t* recl, int* data, float* tolerance, const char* enc)
  {
    JHPCNDF::fwrite(data, 1, (*recl)*4, *unit, *tolerance, 1, enc);
  }
  //subroutine jhpcndf_write_integer8(unit, recl, data, tol, enc)
  void jhpcndf_write_integer8__(int* unit, size_t* recl, long long* data, float* tolerance, const char* enc)
  {
    JHPCNDF::fwrite(data, 1, (*recl)*8, *unit, *tolerance, 1, enc);
  }
  //subroutine jhpcndf_write_character(unit, recl, data, tol, enc)
  void jhpcndf_write_character__(int* unit, size_t* recl, char* data, float* tolerance, const char* enc)
  {
    JHPCNDF::fwrite(data, 1, *recl, *unit, *tolerance, 1, enc);
  }
  //subroutine jhpcndf_read_real4(unit, recl, data)
  void jhpcndf_read_real4__(int* unit, size_t* recl, float* data)
  {
    JHPCNDF::fread(data, 4, *recl, *unit);
  }
  //subroutine jhpcndf_read_real8(unit, recl, data)
  void jhpcndf_read_real8__(int* unit, size_t* recl, double* data)
  {
    JHPCNDF::fread(data, 8, *recl, *unit);
  }
  //subroutine jhpcndf_read_integer4(unit, recl, data)
  void jhpcndf_read_integer4__(int* unit, size_t* recl, int* data)
  {
    JHPCNDF::fread(data, 4, *recl, *unit);
  }
  //subroutine jhpcndf_read_integer8(unit, recl, data)
  void jhpcndf_read_integer8__(int* unit, size_t* recl, long long* data)
  {
    JHPCNDF::fread(data, 8, *recl, *unit);
  }
  //subroutine jhpcndf_read_character(unit, recl, data)
  void jhpcndf_read_character__(int* unit, size_t* recl, char* data)
  {
    JHPCNDF::fread(data, 1, *recl, *unit);
  }

  void jhpcndf_encode_real4__(const size_t length, const float* const src, float* const dst, float* const dst_lower, const float tolerance, bool* is_relative, const char* enc)
  {
    JHPCNDF::encode<float>(length, src, dst, dst_lower, tolerance, is_relative, enc);
  }
  void jhpcndf_encode_real8__(const size_t length, const double* const src, double* const dst, double* const dst_lower, const float tolerance, bool* is_relative, const char* enc)
  {
    JHPCNDF::encode<double>(length, src, dst, dst_lower, tolerance, is_relative, enc);
  }
  void jhpcndf_decode_real4__(const size_t length, const float* const src_upper, const float* const src_lower, float* const dst)
  {
    JHPCNDF::decode<float>(length, src_upper, src_lower, dst);
  }
  void jhpcndf_decode_real8__(const size_t length, const double* const src_upper, const double* const src_lower, double* const dst)
  {
    JHPCNDF::decode<double>(length, src_upper, src_lower, dst);
  }
}


//
// explicit instantiation
//
namespace JHPCNDF
{
  template 
    size_t fwrite<bool>(const bool* ptr, size_t size, size_t nmemb, const int& key, const float& tolerance, const bool& is_relative, const std::string& enc, const bool& time_measuring, const bool& byte_swap);
  template 
    size_t fwrite<char>(const char* ptr, size_t size, size_t nmemb, const int& key, const float& tolerance, const bool& is_relative, const std::string& enc, const bool& time_measuring, const bool& byte_swap);
  template 
    size_t fwrite<unsigned char>(const unsigned char* ptr, size_t size, size_t nmemb, const int& key, const float& tolerance, const bool& is_relative, const std::string& enc, const bool& time_measuring, const bool& byte_swap);
  template 
    size_t fwrite<signed char>(const signed char* ptr, size_t size, size_t nmemb, const int& key, const float& tolerance, const bool& is_relative, const std::string& enc, const bool& time_measuring, const bool& byte_swap);
  template 
    size_t fwrite<short>(const short* ptr, size_t size, size_t nmemb, const int& key, const float& tolerance, const bool& is_relative, const std::string& enc, const bool& time_measuring, const bool& byte_swap);
  template 
    size_t fwrite<unsigned short>(const unsigned short* ptr, size_t size, size_t nmemb, const int& key, const float& tolerance, const bool& is_relative, const std::string& enc, const bool& time_measuring, const bool& byte_swap);
  template 
    size_t fwrite<int>(const int* ptr, size_t size, size_t nmemb, const int& key, const float& tolerance, const bool& is_relative, const std::string& enc,  const bool& time_measuring, const bool& byte_swap);
  template 
    size_t fwrite<unsigned int>(const unsigned int* ptr, size_t size, size_t nmemb, const int& key, const float& tolerance, const bool& is_relative, const std::string& enc, const bool& time_measuring, const bool& byte_swap);
  template 
    size_t fwrite<long>(const long* ptr, size_t size, size_t nmemb, const int& key, const float& tolerance, const bool& is_relative, const std::string& enc, const bool& time_measuring, const bool& byte_swap);
  template 
    size_t fwrite<unsigned long>(const unsigned long* ptr, size_t size, size_t nmemb, const int& key, const float& tolerance, const bool& is_relative, const std::string& enc, const bool& time_measuring, const bool& byte_swap);

  template
    size_t fread<char>(char* ptr, size_t size, size_t nmemb, const int& key, const bool& byte_swap);
  template
    size_t fread<unsigned char>(unsigned char* ptr, size_t size, size_t nmemb, const int& key, const bool& byte_swap);
  template
    size_t fread<int>(int* ptr, size_t size, size_t nmemb, const int& key, const bool& byte_swap);
  template
    size_t fread<unsigned int>(unsigned int* ptr, size_t size, size_t nmemb, const int& key, const bool& byte_swap);
  template
    size_t fread<long>(long* ptr, size_t size, size_t nmemb, const int& key, const bool& byte_swap);
  template
    size_t fread<unsigned long>(unsigned long* ptr, size_t size, size_t nmemb, const int& key, const bool& byte_swap);

  template
    void encode<float>(const size_t& length, const float* const src, float* const dst, float* const dst_lower, const float& tolerance, const bool& is_relative, const std::string& enc, const bool time_measuring);
  template
    void encode<double>(const size_t& length, const double* const src, double* const dst, double* const dst_lower, const float& tolerance, const bool& is_relative, const std::string& enc, const bool time_measuring);

  template
    void decode<float>(const size_t& length, const float* const src_upper, const float* const src_lower, float* const dst);
  template
    void decode<double>(const size_t& length, const double* const src_upper, const double* const src_lower, double* const dst);
}//end of namespace JHPCNDF
