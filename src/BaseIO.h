/*
 * JHPCN-DF - Data compression library based on
 *            Jointed Hierarchical Precision Compression Number Data Format
 *
 * Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 */

// @file BaseIO.h
#ifndef JHPCNDF_BASE_IO_H
#define JHPCNDF_BASE_IO_H
#include <stdio.h>

namespace JHPCNDF
{

  class IO
  {
    public:
      //@brief ファイルからデータを読み取る
      //@param ptr     読み込んだデータを格納する領域へのポインタ
      //@param size    データの1要素あたりのサイズ
      //@param nmemb   確保済の領域サイズ
      //@param stream  ファイル入力元のポインタ
      //@ret 読み取ったデータサイズ
      virtual size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)=0;

      //@brief ファイルへデータを書き込む
      //@param ptr     データを格納した領域へのポインタ
      //@param size    データの1要素あたりのサイズ
      //@param nmemb   データの要素数
      //@param stream  ファイル出力先のポインタ
      //
      //@ret 出力したデータの圧縮前のサイズ
      virtual size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)=0;
  };

  //@brief stdioを使ってバイナリIOを行うクラス
  class stdIO :public IO
  {
    public:
      size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
      {
        return ::fread(ptr, size, nmemb, stream);
      }
      size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
      {
        return ::fwrite(ptr, size, nmemb, stream);
      }
  };

}//end of namespace JHPCNDF
#endif
