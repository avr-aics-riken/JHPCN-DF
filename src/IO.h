/*
 * JHPCN-DF - Data compression library based on
 *            Jointed Hierarchical Precision Compression Number Data Format
 *
 * Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 */

// @file IOFactory.h

#ifndef JHPCNDF_IO_H
#define JHPCNDF_IO_H

#include <string>
#include <sstream>
#include <cstdlib>
#include "BaseIO.h"
#include "zlibIO.h"
#ifdef USE_LZ4
#include "lz4IO.h"
#endif

namespace JHPCNDF
{
    //@brief IO classのFactoryメソッド
    //@param name インスタンス化するクラスを指定する
    //    gzip  gzip形式での圧縮伸長を行うIOクラスを生成
    //    stdio stdioによる通常のIOを行うクラスを生成
    //    lz4   lz4形式での圧縮伸長を行うIOクラスを生成(USE_LZ4が定義されている時のみ有効
    //@param buff_size  stdio以外のIOクラス内部で使用するバッファサイズ(Byte単位)
    IO* IOFactory(const std::string& name, const size_t& buff_size)
    {
        IO* io=NULL;
        if(name.substr(0,4) == "gzip")
        {
          int level=Z_DEFAULT_COMPRESSION;
          std::string::size_type n = name.find_first_of('_');
          if(n!=std::string::npos)
          {
            std::stringstream oss;
            oss<<name.substr(n+1,1);
            int tmp_level=std::atoi(oss.str().c_str());
            if (tmp_level>0 && tmp_level<=9) // level=0(無圧縮)の時はDEFAULT値を使うようにする
            {
              level=tmp_level;
            }
          }

          int strategy=Z_DEFAULT_STRATEGY;
          n = name.find_first_of('_', n+1);
          if(n!=std::string::npos)
          {
            std::stringstream oss;
            oss<<name.substr(n+1,1);
            int tmp_strategy=std::atoi(oss.str().c_str());
            if (tmp_strategy>=0 && tmp_strategy<=4)
            {
              strategy=tmp_strategy;
            }
          }

          io=new zlibIO(buff_size, level, strategy);
        }else if(name == "stdio" || name == "none"){
          io=new stdIO;
#ifdef USE_LZ4
        }else if(name.substr(0,3) == "lz4"){
          int level=0;
          std::string::size_type n = name.find_first_of('_');
          if(n!=std::string::npos)
          {
            std::stringstream oss;
            oss<<name.substr(n+1,2);
            int tmp_level=std::atoi(oss.str().c_str());
            if (tmp_level>=0 && tmp_level<=16)
            {
              level=tmp_level;
            }
          }
          io=new lz4IO(buff_size, level);
#endif
        }else{
            std::cerr<<"invalid io class name specified."<<std::endl;
            std::cerr<<"fall back to uncomressed io"<<std::endl;
            io=new stdIO;
        }
        return io;
    };

}//end of namespace JHPCNDF
#endif
