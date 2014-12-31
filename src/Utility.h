/*
 * JHPCN-DF - Data compression library based on
 *            Jointed Hierarchical Precision Compression Number Data Format
 *
 * Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 */

// @file Utility.h

#ifndef UTILITY_H
#define UTILITY_H
#include <stdint.h>
#include <bitset>
#include <iostream>

namespace
{
    //@brief 2つの値の差が許容誤差以下かどうかを調べる
    template <typename T, unsigned int BLOCK_SIZE>
    bool is_converged(const T* const src1, const T* const src2, const float& tolerance)
    {
        for (unsigned int i=0; i< BLOCK_SIZE; i++)
        {
            if((src1[i]-src2[i])*(src1[i]-src2[i])>tolerance*tolerance) return false;
        }
        return true;
    }

    //@brief float のbit演算用共用体
    union real4byte
    {
        float real;
        int32_t integer;
    };

    //@brief double のbit演算用共用体
    union real8byte
    {
        double real;
        int64_t integer;
    };

    //@brief 下位nビットを0埋めした値を返す(32bit版）
    float n_bit_zero_padding(const float& value,  const unsigned int& n_bit)
    {
        real4byte tmp;
        tmp.real=value;
        tmp.integer = (tmp.integer >> n_bit)<<n_bit;
        return tmp.real;
    }

    //@brief 下位nビットを0埋めした値を返す(64bit版）
    double n_bit_zero_padding(const double& value,  const unsigned int& n_bit)
    {
        real8byte tmp;
        tmp.real=value;
        tmp.integer = (tmp.integer >> n_bit)<<n_bit;
        return tmp.real;
    }

    //@brief srcの下位nビットを0埋めした値をdstに格納する (32bit版)
    template <unsigned int N_WORD>
    void n_bit_zero_padding(const float* src, float* dst, const unsigned int& n_bit)
    {
        real4byte tmp[N_WORD];
        for(unsigned int i=0;i<N_WORD;i++)
        {
            tmp[i].real=src[i];
            tmp[i].integer = (tmp[i].integer >> n_bit)<<n_bit;
            dst[i]=tmp[i].real;
        }
    }

    //@brief srcの下位nビットを0埋めした値をdstに格納する (64bit版)
    template <unsigned int N_WORD>
    void n_bit_zero_padding(const double* src, double* dst, const unsigned int& n_bit)
    {
        real8byte tmp[N_WORD];
        for(unsigned int i=0;i<N_WORD;i++)
        {
            tmp[i].real=src[i];
            tmp[i].integer = (tmp[i].integer >> n_bit)<<n_bit;
            dst[i]=tmp[i].real;
        }
    }


    //@brief src1とsrc2のXORを返す（32bit版）
    float real_xor(const float& src1, const float& src2)
    {
        real4byte tmp1;
        real4byte tmp2;
        tmp1.real=src1;
        tmp2.real=src2;
        tmp1.integer ^= tmp2.integer;
        return tmp1.real;
    }

    //@brief src1とsrc2のXORを返す（64bit版）
    double real_xor(const double& src1, const double& src2)
    {
        real8byte tmp1;
        real8byte tmp2;
        tmp1.real=src1;
        tmp2.real=src2;
        tmp1.integer ^= tmp2.integer;
        return tmp1.real;
    }



    //@brief src1とsrc2のXORをdstに格納（32bit版）
    template <int N_WORD>
    void real_xor(const float* src1, const float* src2, float* dst)
    {
        real4byte tmp1[N_WORD];
        real4byte tmp2[N_WORD];
        for(int i=0; i<N_WORD; i++)
        {
            tmp1[i].real=src1[i];
            tmp2[i].real=src2[i];
            tmp1[i].integer^=tmp2[i].integer;
            dst[i]=tmp1[i].real;
        }
    }

    //@brief src1とsrc2のXORをdstに格納（64bit版）
    template <int N_WORD>
    void real_xor(const double* src1, const double* src2, double* dst)
    {
        real8byte tmp1[N_WORD];
        real8byte tmp2[N_WORD];
        for(int i=0; i<N_WORD; i++)
        {
            tmp1[i].real=src1[i];
            tmp2[i].real=src2[i];
            tmp1[i].integer^=tmp2[i].integer;
            dst[i]=tmp1[i].real;
        }
    }

    //@brief src1とsrc2のANDをdstに格納（32bit版）
    template <int N_WORD>
    void real_and(const float* src1, const float* src2, float* dst)
    {
        real4byte tmp1[N_WORD];
        real4byte tmp2[N_WORD];
        for(int i=0; i<N_WORD; i++)
        {
            tmp1[i].real=src1[i];
            tmp2[i].real=src2[i];
            tmp1[i].integer&=tmp2[i].integer;
            dst[i]=tmp1[i].real;
        }
    }

    //@brief src1とsrc2のANDをdstに格納（64bit版）
    template <int N_WORD>
    void real_and(const double* src1, const double* src2, double* dst)
    {
        real8byte tmp1[N_WORD];
        real8byte tmp2[N_WORD];
        for(int i=0; i<N_WORD; i++)
        {
            tmp1[i].real=src1[i];
            tmp2[i].real=src2[i];
            tmp1[i].integer&=tmp2[i].integer;
            dst[i]=tmp1[i].real;
        }
    }

    //@brief src1とsrc2のORをdstに格納（32bit版）
    template <int N_WORD>
    void real_or(const float* src1, const float* src2, float* dst)
    {
        real4byte tmp1[N_WORD];
        real4byte tmp2[N_WORD];
        for(int i=0; i<N_WORD; i++)
        {
            tmp1[i].real=src1[i];
            tmp2[i].real=src2[i];
            tmp1[i].integer|=tmp2[i].integer;
            dst[i]=tmp1[i].real;
        }
    }

    //@brief src1とsrc2のORをdstに格納（64bit版）
    template <int N_WORD>
    void real_or(const double* src1, const double* src2, double* dst)
    {
        real8byte tmp1[N_WORD];
        real8byte tmp2[N_WORD];
        for(int i=0; i<N_WORD; i++)
        {
            tmp1[i].real=src1[i];
            tmp2[i].real=src2[i];
            tmp1[i].integer|=tmp2[i].integer;
            dst[i]=tmp1[i].real;
        }
    }



    //@brief floatを2進数に変換してstdoutへ出力
    void output_binary(const float& data)
    {
        real4byte tmp;
        tmp.real=data;
        std::cout << static_cast<std::bitset<32> >(tmp.integer)<<std::endl;
    }

    //@brief doubleを2進数に変換してstdoutへ出力
    void output_binary(const double& data)
    {
        real4byte tmp;
        tmp.real=data;
        std::cerr << static_cast<std::bitset<64> >(tmp.integer)<<std::endl;
    }
}//end of unnamed namespace
#endif
