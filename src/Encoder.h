/*
 * JHPCN-DF - Data compression library based on
 *            Jointed Hierarchical Precision Compression Number Data Format
 *
 * Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 */

// @file Encoder.h

#ifndef ENCODER_H
#define ENCODER_H
#include <cmath>
#include "Utility.h"

namespace JHPCNDF
{
    //@ エンコーダのインターフェースを規定する抽象クラス
    //
    //下位bit側の生成はあまりバリエーションが無いのでデフォルト実装も提供する
    template <typename T>
    class Encoder
    {
        public:
            virtual void operator()(const size_t& length, const T* const src, T* const dst, T* const dst_lower=NULL) const=0;
            virtual void make_upper_bits(const size_t& length, const T* const src, T* const dst) const=0;
            virtual void make_lower_bits(const size_t& length, const T* const src, T* const dst, T* const dst_lower) const
            {
#ifdef USE_OPENMP
#pragma omp parallel for
#endif
                for (size_t i=0; i<length; i++)
                {
                    real_xor<1>(&(src[i]), &(dst[i]), &(dst_lower[i]));
                }
            }
        protected:
            void debug_write(const size_t& index, const T* const org, const T* const upper) const
            {
                std::cerr<<"original["<<index<<"]   = ";
                output_binary(std::cerr, org[index]);
                std::cerr<<"\n";
                std::cerr<<"upper_bits["<<index<<"] = ";
                output_binary(std::cerr, upper[index]);
                std::cerr<<"\n";
            }
    };

    //@brief srcをdstへコピーするだけの動作確認用ダミーエンコーダ
    template <typename T>
    class DummyEncoder:public Encoder<T>
    {
        public:
            void operator()(const size_t& length, const T* const src, T* const dst, T* const dst_lower=NULL) const
            {
                make_upper_bits(length, src, dst);
                if(dst_lower != NULL) this->make_lower_bits(length, src, dst, dst_lower);
            }
            void make_upper_bits(const size_t& length, const T* const src, T* const dst) const
            {
#ifdef USE_OPENMP
#pragma omp parallel for
#endif
                for(size_t i=0;i<length;i++)
                {
                    dst[i]=src[i];
                }
            }
    };

    //@brief naive実装
    template <typename T>
    class NormalEncoder:public Encoder<T>
    {
        public:
            NormalEncoder(const float& arg_tolerance, const bool& arg_is_relative): tolerance(arg_tolerance), is_relative(arg_is_relative) {}
            void operator()(const size_t& length, const T* const src, T* const dst, T* const dst_lower=NULL) const
            {
                make_upper_bits(length, src, dst);
                if(dst_lower != NULL) this->make_lower_bits(length, src, dst, dst_lower);
            }
        private:
            void make_upper_bits(const size_t& length, const T* const src, T* const dst) const
            {
#ifdef USE_OPENMP
#pragma omp parallel for
#endif
                for (size_t i=0; i<length; i++)
                {
                    double tolerance=this->tolerance;
                    if(is_relative)
                    {
                        tolerance*=src[i];
                    }
                    unsigned int split_position=get_initial_split_position(src[i], tolerance);
#ifdef DEBUG
                    if(i==0 || i==length/2)
                    {
                        std::cerr<< "initial split_position : "<<split_position <<std::endl;
                    }
#endif
                    n_bit_zero_padding<1>(&(src[i]), &(dst[i]), split_position);
                    while(! is_converged<T, 1>(&(src[i]), &(dst[i]), tolerance))
                    {
                        update_split_position(&split_position);
                        n_bit_zero_padding<1>(&(src[i]), &(dst[i]), split_position);
                    }
#ifdef DEBUG
                    if(i==0 || i==length/2)
                    {
                        std::cerr<< "final split_position   : "<<split_position <<std::endl;
                        this->debug_write(i, src, dst);
                    }
#endif
                }
            }
            unsigned int get_initial_split_position(const T& value, const double& tolerance) const
            {
                double logallo = std::log(tolerance)/std::log(2.0);
                int tmp;
                frexp(value, &tmp);
                int tmp2=(int)(-logallo+tmp-1);
                
                if (sizeof(T) == 4)
                {
                    unsigned int split_position=23-tmp2+1;
                    if (split_position>23) split_position=23;
                    return split_position;
                }else{
                    unsigned int split_position=52-tmp2+1;
                    if (split_position>52) split_position=52;
                    return split_position;
                }
            }
            void update_split_position(unsigned int* split_position) const
            {
                --(*split_position);
            }
            const float tolerance;
            const bool  is_relative;
    };

    //@ 線形探査で切り分け位置を探すエンコーダ
    template <typename T>
    class LinearSearchEncoder:public Encoder<T>
    {
        public:
            LinearSearchEncoder(const float& arg_tolerance, const bool& arg_is_relative): tolerance(arg_tolerance), is_relative(arg_is_relative) {}
            void operator()(const size_t& length, const T* const src, T* const dst, T* const dst_lower=NULL) const
            {
                make_upper_bits(length, src, dst);
                if(dst_lower != NULL) this->make_lower_bits(length, src, dst, dst_lower);
            }
        private:
            void make_upper_bits(const size_t& length, const T* const src, T* const dst) const
            {
#ifdef USE_OPENMP
#pragma omp parallel for
#endif
                for (size_t i=0; i<length; i++)
                {
                    double tolerance=this->tolerance;
                    if(is_relative)
                    {
                        tolerance*=src[i];
                    }
                    unsigned int split_position=get_initial_split_position();
#ifdef DEBUG
                    if(i==0 || i==length/2)
                    {
                        std::cerr<< "initial split_position : "<<split_position <<std::endl;
                    }
#endif
                    n_bit_zero_padding<1>(&(src[i]), &(dst[i]), split_position);
                    while(! is_converged<T, 1>(&(src[i]), &(dst[i]), tolerance))
                    {
                        update_split_position(&split_position);
                        n_bit_zero_padding<1>(&(src[i]), &(dst[i]), split_position);
                    }
#ifdef DEBUG
                    if(i==0 || i==length/2)
                    {
                        std::cerr<< "final split_position   : "<<split_position <<std::endl;
                        this->debug_write(i, src, dst);
                    }
#endif
                }
            }
            unsigned int get_initial_split_position(void) const
            {
                if (sizeof(T) == 4)
                {
                    return 23;
                }else{
                    return 52;
                }
            }
            void update_split_position(unsigned int* split_position) const
            {
                --(*split_position);
            }
            const float tolerance;
            const bool  is_relative;
    };

    //@ 上位bitと下位bitの切り分けを8bit単位にまるめたエンコーダ
    template <typename T>
    class ByteAligndEncoder:public Encoder<T>
    {
        public:
            ByteAligndEncoder(const float& arg_tolerance, const bool& arg_is_relative): tolerance(arg_tolerance), is_relative(arg_is_relative)
            {
                if (sizeof(T) == 4)
                {
                    split_positions=new int [5];
                    split_positions[0]=23;
                    split_positions[1]=16;
                    split_positions[2]=8;
                    split_positions[3]=4;
                    split_positions[4]=0;
                }else{
                    split_positions=new int [9];
                    split_positions[0]=52;
                    split_positions[1]=48;
                    split_positions[2]=40;
                    split_positions[3]=32;
                    split_positions[4]=24;
                    split_positions[5]=16;
                    split_positions[6]=8;
                    split_positions[7]=4;
                    split_positions[8]=0;
                }
            }
            void operator()(const size_t& length, const T* const src, T* const dst, T* const dst_lower=NULL) const
            {
                make_upper_bits(length, src, dst);
                if(dst_lower != NULL) this->make_lower_bits(length, src, dst, dst_lower);
            }
        private:
            void make_upper_bits(const size_t& length, const T* const src, T* const dst) const
            {
#ifdef USE_OPENMP
#pragma omp parallel for
#endif
                for (size_t i=0; i<length; i++)
                {
                    double tolerance=this->tolerance;
                    if(is_relative)
                    {
                        tolerance*=src[i];
                    }
                    int j=0;
                    unsigned int split_position=split_positions[j];
#ifdef DEBUG
                    if(i==0 || i==length/2)
                    {
                        std::cerr<< "initial split_position : "<<split_position <<std::endl;
                    }
#endif
                    n_bit_zero_padding<1>(&(src[i]), &(dst[i]), split_position);
                    while(! is_converged<T, 1>(&(src[i]), &(dst[i]), tolerance))
                    {
                        split_position=split_positions[++j];
                        n_bit_zero_padding<1>(&(src[i]), &(dst[i]), split_position);
                    }
#ifdef DEBUG
                    if(i==0 || i==length/2)
                    {
                        std::cerr<< "final split_position   : "<<split_position <<std::endl;
                        this->debug_write(i, src, dst);
                    }
#endif
                }
            }
            const float tolerance;
            int*  split_positions;
            const bool  is_relative;
    };

    //@brief 二分探査で分割位置を探索するエンコーダ
    template <typename T>
    class BinarySearchEncoder:public Encoder<T>
    {
        public:
            BinarySearchEncoder(const float& arg_tolerance, const bool& arg_is_relative): tolerance(arg_tolerance), is_relative(arg_is_relative) {}
            void operator()(const size_t& length, const T* const src, T* const dst, T* const dst_lower=NULL) const
            {
                make_upper_bits(length, src, dst);
                if(dst_lower != NULL) this->make_lower_bits(length, src, dst, dst_lower);
            }
        private:
            void make_upper_bits(const size_t& length, const T* const src, T* const dst) const
            {
#ifdef USE_OPENMP
#pragma omp parallel for
#endif
                for (size_t i=0; i<length; i++)
                {
                    double tolerance=this->tolerance;
                    if(is_relative)
                    {
                        tolerance*=src[i];
                    }

                    unsigned int left = get_fraction_length();
                    unsigned int split_position=left/2; //center
                    unsigned int right= 0;
#ifdef DEBUG
                    if(i==0 || i==length/2)
                    {
                        std::cerr<< "initial split_position : "<<split_position <<std::endl;
                    }
#endif
                    n_bit_zero_padding<1>(&(src[i]), &(dst[i]), split_position);
                    while(left >= right)
                    {
#ifdef DEBUG
                        if(i==0 || i==length/2)
                        {
                            std::cerr<< "left, split_position, right = "<<left<<","<<split_position <<","<<right<<std::endl;
                        }
#endif
                        if(is_converged<T, 1>(&(src[i]), &(dst[i]), tolerance))
                        {
                            right=split_position+1;
                        }else{
                            left=split_position-1;
                        }
                        split_position=(left+right)/2;
                        n_bit_zero_padding<1>(&(src[i]), &(dst[i]), split_position);
                    }

#ifdef DEBUG
                    if(i==0 || i==length/2)
                    {
                        std::cerr<< "final split_position   : "<<split_position <<std::endl;
                        this->debug_write(i, src, dst);
                    }
#endif
                }
            }
            unsigned int get_fraction_length(void) const
            {
                if (sizeof(T) == 4)
                {
                    return 23;
                }else{
                    return 52;
                }
            }
            const float tolerance;
            const bool  is_relative;
    };

    //@brief 特定の分割位置以下のビットを0埋めするエンコーダ
    template <typename T>
    class NbitFilter:public Encoder<T>
    {
        public:
            NbitFilter(unsigned int arg_split_position):split_position(arg_split_position) {}
            void operator()(const size_t& length, const T* const src, T* const dst, T* const dst_lower=NULL) const
            {
                make_upper_bits(length, src, dst);
                if(dst_lower != NULL) this->make_lower_bits(length, src, dst, dst_lower);
            }
        private:
            void make_upper_bits(const size_t& length, const T* const src, T* const dst) const
            {
#ifdef USE_OPENMP
#pragma omp parallel for
#endif
                for (int i=0;i<length; i++)
                {
                    n_bit_zero_padding<1>(&(src[i]), &(dst[i]), split_position);
                }
            }
            unsigned int split_position;
    };

    template <typename T>
    Encoder<T>* EncoderFactory(const std::string& name, const float& tolerance, const bool& is_relative)
    {
        Encoder<T>* enc=NULL;
        if(name == "original")
        {
            enc=new NormalEncoder<T>(tolerance, is_relative);
        }else if(name == "byte_aligned"){
            enc=new ByteAligndEncoder<T>(tolerance, is_relative);
        }else if(name == "linear_search"){
            enc=new LinearSearchEncoder<T>(tolerance, is_relative);
        }else if(name == "binary_search"){
            enc=new BinarySearchEncoder<T>(tolerance, is_relative);
        }else if(name == "dummy"){
            enc=new DummyEncoder<T>;
        }else if(name == "nbit_filter"){
            const unsigned int n_bit = tolerance >0? tolerance:-tolerance;
            enc=new NbitFilter<T>(n_bit);
        }else{
            std::cerr<<"invalid encoder name specified."<<std::endl;
            std::cerr<<"fall back to normal encoder"<<std::endl;
            enc=new NormalEncoder<T>(tolerance, is_relative);
        }
        return enc;
    }
}//end of namespace JHPCNDF
#endif
