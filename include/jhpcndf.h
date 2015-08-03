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
    //@brief ファイルを開く
    //@param filename_upper 上位bit側のデータを格納するファイルの名前
    //@param filename_lower 下位bit側のデータを格納するファイルの名前
    //@param mode           ファイルopen時のモードを示す文字列(通常のfopenと同じ）
    //@param comp           圧縮形式
    //compに指定できる圧縮形式は以下の2種類がある
    //  none:        圧縮しない
    //  gzip_n_m:    gzip形式で圧縮
    //               n, m はzlibに渡すオプションで、nは圧縮レベル(1～9)、mはstrategy(1～4)を表す
    //               '_'以降は省略可能
    //               省略した場合や範囲外の値が設定された場合はzlibのデフォルト値が使われる。
    //               それぞれの値の意味はzlib.hを参照のこと
    //               なお、本来は圧縮レベルに0(=無圧縮)も指定できるが
    //               本ライブラリでは0を指定した時はデフォルト値(-1)を使う。
    //               無圧縮にする場合は、compに"none"を指定すること
    //   lz4_nn:     lz4形式で圧縮
    //               nnはlz4ライブラリに渡すオプションで、圧縮レベル(0～16)を表す
    //               ビルド時に-DUSE_LZ4オプションを指定していなかった場合は、無効なオプションとして扱われる
    //
    //@param buff_size      圧縮/伸張する際のバッファサイズ(単位はbyte)
    //@ret   開いたファイルを識別するためのID番号
    int fopen(const std::string& filename_upper, const std::string& filename_lower = "", const char* mode = "rb", const std::string& comp = "gzip", const size_t& buff_size=32768);


    //@brief JHPCNDF::fopenで開いたファイルを閉じる
    //@param key 閉じるファイルを識別するためのID番号
    void fclose(const int& key);


    //@brief 渡されたデータを圧縮した上でファイルに出力する
    //@param ptr            出力するデータ
    //@param size           出力するデータの1wordの長さ(float=4, double=8で固定）
    //@param nmemb          出力するデータの要素数
    //@param key            出力先ファイルを識別するためのID番号
    //@param tolerance      許容誤差
    //@param is_relative    許容誤差を相対値で指定するかどうかのフラグ
    //@param enc            使用するエンコーダの種類
    //@param time_measuring trueが指定されると、処理にかかった時間を計測し標準エラー出力ヘ出力する
    //@param byte_swap      ファイル出力時にエンディアン変換を行う
    //
    //encに指定できるエンコーダは以下の6種類がある
    //  original:      論文どおりの実装
    //  linear_search: 分割位置を線形探索により上位bitから順に探す
    //  binary_search: 分割位置を二分探索で探す
    //  byte_aligned:  上位bitと下位bitの分割位置を8*n bitの位置に制限する
    //  nbit_filter:   指定されたbit位置（tolerance) 以下を0埋めする
    //  dummy:         分割しない（全てのデータを上位bit側に出力する)
    template <typename T>
    size_t fwrite(const T* ptr, size_t size, size_t nmemb, const int& key, const float& tolerance, const bool& is_relative=true, const std::string& enc="binary_search", const bool& time_measuring = false, const bool& byte_swap=false);



    //@brief 指定されたファイルからデータを読み込む
    //@param ptr       ファイルから読み込んだデータを格納する領域
    //@param size      読み込むデータの1wordの長さ(float=4, double=8で固定）
    //@param nmemb     読み込むデータの要素数
    //@param key       読み込むファイルを識別するためのID番号
    //@param byte_swap 読み込んだデータにエンディアン変換を行う
    //
    //ptrが指す領域は事前に確保する必要あり
    //nmembに指定する値は、圧縮済のデータを読む場合であっても圧縮前のデータ数を渡す必要がある
    template <typename T>
    size_t fread(T* ptr, size_t size, size_t nmemb, const int& key, const bool& byte_swap=false);


    //@brief メモリ上でJHPCN-DFによるデータのエンコードを行う
    //@param length         元データの要素数
    //@param src            元データ
    //@param dst            エンコード後の上位bit側データ
    //@param dst_lower      エンコード後の下位bit側データ
    //@param tolerance      許容誤差
    //@param is_relative    許容誤差を相対値で指定するかどうかのフラグ
    //@param enc            使用するエンコーダの種類
    //@param time_measuring trueが指定されると、処理にかかった時間を計測し標準エラー出力ヘ出力する
    //
    //dst, dst_lowerが指す領域は事前に確保する必要あり
    //encに指定できるエンコーダはfwriteの項を参照のこと
    //
    template<typename T>
    void encode(const size_t& length, const T* const src, T* const dst, T* const dst_lower, const float& tolerance, const bool& is_relative=true, const std::string& enc = "binary_search", const bool time_measuring = false);


    //@beief メモリ上でJHPCN-DFによるデータのデコードを行う
    //@param length         元データの要素数
    //@param src_upper      エンコード済の上位bit側データ
    //@param src_lower      エンコード済の下位bit側データ
    //@param dst            デコード後のデータ
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
//@brief JHPCNDF::fopenに対する C言語用インターフェース
int JHPCNDF_fopen(const char* filename_upper, const char* filename_lower, const char* mode, const char* comp, const size_t buff_size);

//@brief JHPCNDF::fcloseに対する C言語用インターフェース
void JHPCNDF_fclose(const int key);

//@brief JHPCNDF::fwriteに対する C言語用インターフェース(float版)
size_t JHPCNDF_fwrite_float(const float* ptr, size_t size, size_t nmemb, const int key, const float tolerance, const int is_relative, const char* enc);

//@brief JHPCNDF::fwriteに対する C言語用インターフェース(double版)
size_t JHPCNDF_fwrite_double(const double * ptr, size_t size, size_t nmemb, const int key, const float tolerance, const int is_relative, const char* enc);

//@brief JHPCNDF::fwriteに対する C言語用インターフェース(その他版)
size_t JHPCNDF_fwrite(const void* ptr, size_t size, size_t nmemb, const int key, const char* enc);

//@brief JHPCNDF::freadに対する C言語用インターフェース(float版)
size_t JHPCNDF_fread_float(float* ptr, size_t size, size_t nmemb, const int key);

//@brief JHPCNDF::freadに対する C言語用インターフェース(double版)
size_t JHPCNDF_fread_double(double* ptr, size_t size, size_t nmemb, const int key);

//@brief JHPCNDF::freadに対する C言語用インターフェース(その他版)
size_t JHPCNDF_fread(void* ptr, size_t size, size_t nmemb, const int key);

//@brief JHPCNDF::encode<float>に対する C言語用インターフェース
void JHPCNDF_encode_float(const size_t length, const float* const src, float* const dst, float* const dst_lower, const float tolerance, const int is_relative, const char* enc);

//@brief JHPCNDF::encode<double>に対する C言語用インターフェース
void JHPCNDF_encode_double(const size_t length, const double* const src, double* const dst, double* const dst_lower, const float tolerance, const int is_relative, const char* enc);

//@brief JHPCNDF::decode<float>に対する C言語用インターフェース
void JHPCNDF_decode_float(const size_t length, const float* const src_upper, const float* const src_lower, float* const dst);

//@brief JHPCNDF::decode<double>に対する C言語用インターフェース
void JHPCNDF_decode_double(const size_t length, const double* const src_upper, const double* const src_lower, double* const dst);
#ifdef __cplusplus
}
#endif

#endif
