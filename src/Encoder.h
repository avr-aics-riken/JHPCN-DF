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
            NormalEncoder(const float& arg_tolerance): tolerance(arg_tolerance) {}
            void operator()(const size_t& length, const T* const src, T* const dst, T* const dst_lower=NULL) const
            {
                make_upper_bits(length, src, dst);
                if(dst_lower != NULL) this->make_lower_bits(length, src, dst, dst_lower);
            }
        private:
            void make_upper_bits(const size_t& length, const T* const src, T* const dst) const
            {
                for (size_t i=0; i<length; i++)
                {
                    unsigned int split_position=get_initial_split_position(src[i]);
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
            unsigned int get_initial_split_position(const T& value) const
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
    };

    //@ 線形探査で切り分け位置を探すエンコーダ
    template <typename T>
    class LinearSearchEncoder:public Encoder<T>
    {
        public:
            LinearSearchEncoder(const float& arg_tolerance): tolerance(arg_tolerance) {}
            void operator()(const size_t& length, const T* const src, T* const dst, T* const dst_lower=NULL) const
            {
                make_upper_bits(length, src, dst);
                if(dst_lower != NULL) this->make_lower_bits(length, src, dst, dst_lower);
            }
        private:
            void make_upper_bits(const size_t& length, const T* const src, T* const dst) const
            {
                for (size_t i=0; i<length; i++)
                {
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
                    return 54;
                }
            }
            void update_split_position(unsigned int* split_position) const
            {
                --(*split_position);
            }
            const float tolerance;
    };

    //@ 上位bitと下位bitの切り分けを8bit単位にまるめたエンコーダ
    template <typename T>
    class ByteAligndEncoder:public Encoder<T>
    {
        public:
            ByteAligndEncoder(const float& arg_tolerance): tolerance(arg_tolerance) {}
            void operator()(const size_t& length, const T* const src, T* const dst, T* const dst_lower=NULL) const
            {
                make_upper_bits(length, src, dst);
                if(dst_lower != NULL) this->make_lower_bits(length, src, dst, dst_lower);
            }
        private:
            void make_upper_bits(const size_t& length, const T* const src, T* const dst) const
            {
                for (size_t i=0; i<length; i++)
                {
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
                    return 16; // 23/8*8
                }else{
                    return 48; // 54/8*8
                }
            }
            void update_split_position(unsigned int* split_position) const
            {
                (*split_position)-=8;
            }
            const float tolerance;
    };

    //@brief 二分探査で23bitから下を順に探していくエンコーダ
    template <typename T>
    class BinarySearchEncoder:public Encoder<T>
    {
        public:
            BinarySearchEncoder(const float& arg_tolerance): tolerance(arg_tolerance) {}
            void operator()(const size_t& length, const T* const src, T* const dst, T* const dst_lower=NULL) const
            {
                make_upper_bits(length, src, dst);
                if(dst_lower != NULL) this->make_lower_bits(length, src, dst, dst_lower);
            }
        private:
            void make_upper_bits(const size_t& length, const T* const src, T* const dst) const
            {
                for (size_t i=0; i<length; i++)
                {
                    unsigned int split_position=get_initial_split_position();
#ifdef DEBUG
                    if(i==0 || i==length/2)
                    {
                        std::cerr<< "initial split_position : "<<split_position <<std::endl;
                    }
#endif
                    n_bit_zero_padding<1>(&(src[i]), &(dst[i]), split_position);
                    for(int stage=2;stage<get_initial_split_position()*2;stage*=2)
                    {
                        update_split_position(&split_position, stage, is_converged<T, 1>(&(src[i]), &(dst[i]), tolerance));
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
                    return 12; // 23/8*8
                }else{
                    return 27; // 54/8*8
                }
            }
            void update_split_position(unsigned int* split_position, const int& stage, const bool& is_converged) const
            {
                (*split_position)=is_converged ? (*split_position)+get_initial_split_position()/stage : (*split_position)-get_initial_split_position()/stage;
            }
            const float tolerance;
    };

    //@brief 特定の分割位置以下をのビットを0埋めするエンコーダ
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
                for (int i=0;i<length; i++)
                {
                    n_bit_zero_padding<1>(&(src[i]), &(dst[i]), split_position);
                }
            }
            unsigned int split_position;
    };

    template <typename T>
    Encoder<T>* EncoderFactory(const std::string& name, const float& tolerance)
    {
        Encoder<T>* enc=NULL;
        if(name == "normal")
        {
            enc=new NormalEncoder<T>(tolerance);
        }else if(name == "byte_aligned"){
            enc=new ByteAligndEncoder<T>(tolerance);
        }else if(name == "linear_search"){
            enc=new LinearSearchEncoder<T>(tolerance);
        }else if(name == "binary_search"){
            enc=new BinarySearchEncoder<T>(tolerance);
        }else if(name == "dummy"){
            enc=new DummyEncoder<T>;
        }else if(name == "nbit_filter"){
            const unsigned int n_bit = tolerance >0? tolerance:-tolerance;
            enc=new NbitFilter<T>(n_bit);
        }else{
            std::cerr<<"invalid encoder name specified."<<std::endl;
            std::cerr<<"fall back to normal encoder"<<std::endl;
            enc=new NormalEncoder<T>(tolerance);
        }
        return enc;
    }
}//end of namespace JHPCNDF
#endif
