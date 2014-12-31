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
            void debug_write(const size_t& length, const T* const org, const T* const upper, const T* const lower) const
            {
                std::cerr<<"original[0]   =";
                output_binary(org[0]);
                std::cerr<<"upper_bits[0] =";
                output_binary(upper[0]);
                if(lower!=NULL)
                {
                    std::cerr<<"lower_bits[0] =";
                    output_binary(lower[0]);
                }
                std::cerr<<"original["<<length/2<<"]   =";
                output_binary(org[length/2]);
                std::cerr<<"upper_bits["<<length/2<<"] =";
                output_binary(upper[length/2]);
                if(lower!=NULL)
                {
                    std::cerr<<"lower_bits["<<length/2<<"] =";
                    output_binary(lower[length/2]);
                }
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
#ifdef DEBUG
                this->debug_write(length, src, dst, dst_lower);
#endif
            }
        private:
            void make_upper_bits(const size_t& length, const T* const src, T* const dst) const
            {
                for (size_t i=0; i<length; i++)
                {
                    unsigned int split_position=get_initial_split_position(src[i]);
                    n_bit_zero_padding<1>(&(src[i]), &(dst[i]), split_position);
                    while(! is_converged<T, 1>(&(src[i]), &(dst[i]), tolerance))
                    {
                        update_split_position(&split_position);
                        n_bit_zero_padding<1>(&(src[i]), &(dst[i]), split_position);
                    }
                }
            }
            unsigned int get_initial_split_position(const T& value) const
            {
                static double  logallo = std::log(tolerance)/std::log(2.0);
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
#ifdef DEBUG
                this->debug_write(length, src, dst, dst_lower);
#endif
            }
        private:
            void make_upper_bits(const size_t& length, const T* const src, T* const dst) const
            {
                for (size_t i=0; i<length; i++)
                {
                    unsigned int split_position=get_initial_split_position();
                    n_bit_zero_padding<1>(&(src[i]), &(dst[i]), split_position);
                    while(! is_converged<T, 1>(&(src[i]), &(dst[i]), tolerance))
                    {
                        update_split_position(&split_position);
                        n_bit_zero_padding<1>(&(src[i]), &(dst[i]), split_position);
                    }
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

    //@brief 特定の分割位置で強制的に0埋めするエンコーダ
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
            void make_upper_bits(const size_t& length, const T* const src, T* const dst)
            {
                for (size_t i=0; i<length; i++)
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
        }else if(name == "dummy"){
            enc=new DummyEncoder<T>;
        }else{
            std::cerr<<"invalid encoder name specified."<<std::endl;
            std::cerr<<"fall back to normal encoder"<<std::endl;
            enc=new NormalEncoder<T>(tolerance);
        }
        return enc;
    }
}//end of namespace JHPCNDF
#endif
