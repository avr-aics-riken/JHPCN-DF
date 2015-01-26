/*
 * JHPCN-DF - Data compression library based on
 *            Jointed Hierarchical Precision Compression Number Data Format
 *
 * Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 */

// @file IO.h

#ifndef JHPCNDF_IO_H
#define JHPCNDF_IO_H
#include "stdio.h"
#include "zlib.h"

namespace JHPCNDF
{
    class IO
    {
        public:
            virtual size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)=0;
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

    //@brief zlibを使用して圧縮/伸長しつつファイルIOを行うクラス
    class zlibIO :public IO
    {
        public:
            // [memo] windowBitsは15(=MAX_WBITS)を指定した時はzlib形式、16+MAX_WBITSを指定した時はgzip形式での圧縮となる
            zlibIO() :buffer_size(32768), level(Z_DEFAULT_COMPRESSION), strategy(Z_DEFAULT_STRATEGY), windowBits(16+MAX_WBITS) {}
            
            //@brief zlibで圧縮されたデータを読み込んで伸長したうえでptrへ書き込む
            //@param ptr     読み込んだデータを格納する領域へのポインタ
            //@param size    データの1要素あたりのサイズ
            //@param nmemb   確保済の領域サイズ
            //@param stream  ファイル入力元のポインタ
            //
            //エラー発生時はstderrにメッセージを出力した上で0を返す
            //伸長後のファイルサイズが0だった場合も0が返るので注意
            size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
            {
                size_t output_size=0;
                z_stream z_st;
                init_zstream(&z_st);
                if(inflateInit2(&z_st, windowBits) != Z_OK)
                {
                    std::cerr<<"zlib initialization failed uncompressed output will be generated."<<std::endl;
                    return 0;
                }
                unsigned char buffer[buffer_size];

                z_st.avail_out=size*nmemb;
                z_st.next_out=(Bytef*)ptr;
                int rt=Z_OK;
                do
                {
                    int old_avail_out=z_st.avail_out;
                    z_st.avail_in = ::fread(buffer, 1, (size_t)buffer_size, stream);
                    if (ferror(stream)) {
                        inflateEnd(&z_st);
                        std::cerr<<"file read error."<<std::endl;
                        return 0;
                    }
                    int flush = feof(stream) ? Z_FINISH : Z_NO_FLUSH;
                    z_st.next_in = (Bytef*)buffer;
                    rt = inflate(&z_st, flush);
                    output_size += old_avail_out-z_st.avail_out;
                }while(rt == Z_OK);
                if(rt == Z_STREAM_ERROR)
                {
                    std::cerr<<"fatal error occurred during zlib decompression"<<std::endl;
                    return 0;
                }
                inflateEnd(&z_st);
                return output_size;
            }

            //@brief zlibを使って圧縮したデータをファイルに出力する
            //@param ptr     圧縮するデータを格納した領域へのポインタ
            //@param size    圧縮するデータの1要素の長さ
            //@param nmemb   圧縮するデータの要素数
            //@param stream  ファイル出力先のポインタ
            //
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
                unsigned char buffer[buffer_size];

                z_st.next_in=(Bytef*)ptr;
                z_st.avail_in=size*nmemb;
                int rt=Z_OK;
                do
                {
                    //圧縮用バッファをセット
                    z_st.next_out=(Bytef*)buffer;
                    z_st.avail_out=buffer_size;

                    //bufferサイズ分だけ圧縮
                    int flush=z_st.avail_in>0?Z_PARTIAL_FLUSH:Z_FINISH;
                    rt = deflate(&z_st, flush);
                    if(rt == Z_STREAM_ERROR)
                    {
                        std::cerr<<"fatal error occurred during zlib compression"<<std::endl;
                        return 0;
                    }

                    //圧縮済データをファイルに出力
                    if(buffer_size-z_st.avail_out >0)
                    {
                        output_size += ::fwrite(buffer, 1, (size_t)(buffer_size-z_st.avail_out), stream);
                    }
                }while(rt != Z_STREAM_END);

                deflateEnd(&z_st);

                return output_size;
            }

        private:
            void init_zstream(z_stream* stream)
            {
                stream->zalloc = Z_NULL;
                stream->zfree = Z_NULL;
                stream->opaque = Z_NULL;
            }
            const int buffer_size;
            int level;
            int strategy;
            int windowBits;
    };

    //@brief IO classのFactoryメソッド
    //@param name インスタンス化するクラスを指定する
    //    gzip  gzip形式での圧縮伸長を行うIOクラスを生成
    //    stdio stdioによる通常のIOを行うクラスを生成
    IO* IOFactory(const std::string& name)
    {
        IO* io=NULL;
        if(name == "gzip")
        {
            io=new zlibIO();
        }else if(name == "stdio" || name == "none"){
            io=new stdIO;
        }else{
            std::cerr<<"invalid io class name specified."<<std::endl;
            std::cerr<<"fall back to uncomressed io"<<std::endl;
            io=new stdIO;
        }
        return io;
    };

}//end of namespace JHPCNDF
#endif

