/*
 * JHPCN-DF - Data compression library based on
 *            Jointed Hierarchical Precision Compression Number Data Format
 *
 * Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 */

// @file zlibIO.h

#ifndef JHPCNDF_ZLIB_IO_H
#define JHPCNDF_ZLIB_IO_H
#include <zlib.h>
#include <limits.h>
#include <vector>
namespace JHPCNDF
{
  //@brief zlibを使用して圧縮/伸長しつつファイルIOを行うクラス
  class zlibIO :public IO
  {
      public:
          // [memo] windowBitsは15(=MAX_WBITS)を指定した時はzlib形式、16+MAX_WBITSを指定した時はgzip形式での圧縮となる
          zlibIO(const size_t& arg_buffer_size, const int& arg_level=Z_DEFAULT_COMPRESSION, const int& arg_st=Z_DEFAULT_STRATEGY) :
            buffer_size(arg_buffer_size),
            level(arg_level),
            strategy(arg_st),
            windowBits(16+MAX_WBITS),
            block_size(UINT_MAX) {}
          
          //@brief zlibで圧縮されたデータを読み込んで伸長したうえでptrへ書き込む
          //
          //引数、戻り値はBaseIO.hを参照のこと
          //なお、本ルーチンはエラー発生時にstderrへメッセージを出力した上で0を返す
          //伸長後のファイルサイズが0だった場合も0が返るので注意
          size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
          {
              size_t output_size=0;
              z_stream z_st;
              init_zstream(&z_st);
              if(inflateInit2(&z_st, windowBits) != Z_OK)
              {
                  std::cerr<<"zlib initialization failed."<<std::endl;
                  return 0;
              }

              unsigned char* buffer = new unsigned char[buffer_size];
              const size_t size_in_byte=size*nmemb;
              unsigned int reminder = size_in_byte%block_size;
              const int num_block = size_in_byte/block_size;

              std::vector<size_t> offsets;
              if(reminder > 0)
              {
                  offsets.push_back(reminder);
              }
              for(int i=1; i<= size_in_byte/block_size; i++)
              {
                  offsets.push_back(reminder+i*block_size);
              }
              int offset_index=0;

              // 出力バッファの初期設定
              z_st.avail_out=offsets[0];
              z_st.next_out=(Bytef*)ptr;

              //入力バッファの初期設定
              //ファイルをbuffer_size分読んで入力バッファをセット
              z_st.avail_in = ::fread(buffer, 1, (size_t)buffer_size, stream);
              size_t read_size = z_st.avail_in;
              if (ferror(stream)) {
                  inflateEnd(&z_st);
                  std::cerr<<"file read error."<<std::endl;
                  delete [] buffer;
                  return 0;
              }
              z_st.next_in = (Bytef*)buffer;

              //伸張処理の開始
              int rt=Z_OK;
              do
              {
                  // bufferにあるデータを伸張
                  int old_avail_out=z_st.avail_out;
                  int flush = feof(stream) ? Z_FINISH : Z_NO_FLUSH;
                  rt = inflate(&z_st, flush);
                  output_size += old_avail_out-z_st.avail_out;

                  if(rt == Z_NEED_DICT || rt == Z_DATA_ERROR || rt == Z_STREAM_ERROR || rt == Z_MEM_ERROR)
                  {
                      return fatal_error(&z_st, buffer);
                  }else{
                      // 出力バッファが無くなっていたらバッファ領域を再設定
                      if(z_st.avail_out == 0)
                      {
                          z_st.avail_out=block_size;
                          z_st.next_out=(Bytef*)(ptr)+offsets[offset_index++];
                      }
                      //入力バッファが無くなっていたらファイルから読み込み
                      if(z_st.avail_in == 0)
                      {
                          z_st.avail_in = ::fread(buffer, 1, (size_t)buffer_size, stream);
                          read_size += z_st.avail_in;
                          if (ferror(stream)) {
                              inflateEnd(&z_st);
                              std::cerr<<"file read error."<<std::endl;
                              return 0;
                          }
                          z_st.next_in = (Bytef*)buffer;
                      }
                  }
              }while(rt != Z_STREAM_END);

              inflateEnd(&z_st);
              delete [] buffer;
              return output_size;
          }


          //@brief zlibを使って圧縮したデータをファイルに出力する
          //引数、戻り値はBaseIO.hを参照のこと
          //
          //zlibの初期化に失敗した場合は、メッセージを出力した上で無圧縮のファイルを出力する
          //圧縮中にエラーが発生した場合は、メッセージを出力した上で0を返す
          size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
          {
              size_t output_size=0;
              z_stream z_st;
              init_zstream(&z_st);

              if(deflateInit2(&z_st, level, Z_DEFLATED, windowBits, 8, strategy) != Z_OK)
              {
                  std::cerr<<"zlib initialization failed uncompressed output will be generated."<<std::endl;
                  return ::fwrite(ptr, size, nmemb, stream);
              }

              unsigned char* buffer = new unsigned char[buffer_size];
              const size_t size_in_byte=size*nmemb;
              unsigned int reminder = size_in_byte%block_size;
              const int num_block = size_in_byte/block_size;

              std::vector<size_t> offsets;
              if(reminder > 0)
              {
                  offsets.push_back(reminder);
              }
              for(int i=1; i<= size_in_byte/block_size; i++)
              {
                  offsets.push_back(reminder+i*block_size);
              }
              int offset_index=0;
                  

              // 出力バッファの初期設定
              z_st.avail_out=buffer_size;
              z_st.next_out=(Bytef*)buffer;

              //入力バッファの初期設定
              z_st.avail_in = offsets[0];
              z_st.next_in = (Bytef*)ptr;

              int rt=Z_OK;
              int flush=Z_NO_FLUSH;
              do
              {
                  //bufferサイズ分だけ圧縮
                  rt = deflate(&z_st, flush);
                  if(rt== Z_STREAM_ERROR)
                  {
                      return fatal_error(&z_st, buffer);
                  }else{
                      //出力バッファが無くなっていたらファイルへ出力してバッファを再設定
                      if(z_st.avail_out ==0)
                      {
                        size_t written_size = ::fwrite(buffer, 1, (size_t)(buffer_size), stream);
                        if(written_size >= buffer_size)
                        {
                          output_size += buffer_size;
                        }else{
                          std::cerr<<"file output failed! "<<std::endl;
                          delete [] buffer;
                          return output_size;
                        }
                        z_st.avail_out=buffer_size;
                        z_st.next_out=(Bytef*)buffer;
                      }
                      //入力バッファが無くなっていたらバッファ領域を再設定
                      if(z_st.avail_in == 0)
                      {
                          //最後のブロックまで圧縮していたらflush parameterを切り替える
                          //まだブロックが残っていれば入力バッファを進める
                          if(offset_index == offsets.size()-1)
                          {
                              flush=Z_FINISH;
                          }else{
                              z_st.avail_in=block_size;
                              z_st.next_in=(Bytef*)(ptr)+offsets[offset_index++];
                          }
                      }
                  }
              }while(rt != Z_STREAM_END);

              //bufferにまだファイルに書いていないデータが残っていたら書き出す
              size_t leftover=buffer_size-z_st.avail_out;
              if(leftover>0)
              {
                size_t written_size = ::fwrite(buffer, 1, leftover, stream);
                if(written_size >= leftover)
                {
                  output_size += leftover;
                }else{
                  std::cerr<<"file output failed! "<<std::endl;
                  delete [] buffer;
                  return output_size;
                }
              }

              deflateEnd(&z_st);
              delete [] buffer;
              return output_size;
          }

      private:
          void init_zstream(z_stream* stream)
          {
              stream->zalloc = Z_NULL;
              stream->zfree = Z_NULL;
              stream->opaque = Z_NULL;
          }

          int fatal_error(z_stream* z_st, unsigned char* buffer)
          {
              std::cerr<<"fatal error occurred during the processing of zlib"<<std::endl;
              delete [] buffer;
              return 0;
          }

          const int buffer_size;
          const size_t block_size;

          // instanceが生成された後で変更されるとややこしいのでsetterは作らないこと！
          int level;
          int strategy;
          int windowBits;
  };

}//end of namespace JHPCNDF
#endif
