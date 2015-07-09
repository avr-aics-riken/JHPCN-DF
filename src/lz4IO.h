/*
 * JHPCN-DF - Data compression library based on
 *            Jointed Hierarchical Precision Compression Number Data Format
 *
 * Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 */

// @file lz4IO.h

#ifndef JHPCNDF_LZ4_IO_H
#define JHPCNDF_LZ4_IO_H
#include <lz4.h>
#include <lz4frame.h>
namespace JHPCNDF
{
  //@brief lz4を使用して圧縮/伸長しつつファイルIOを行うクラス
  class lz4IO :public IO
  {
    public:
      lz4IO(const size_t& arg_buffer_size): buffer_size(arg_buffer_size)
      {
        LZ4F_errorCode_t err=LZ4F_createCompressionContext(&ctx, LZ4_versionNumber());
        if(err != 0)
        {
          std::cerr<<"LZ4FCompressionContext failed!"<<std::endl;
        }
        err=LZ4F_createDecompressionContext(&dctx, LZ4_versionNumber());
        if(err != 0)
        {
          std::cerr<<"LZ4FDecompressionContext failed!"<<std::endl;
        }
        preferences.compressionLevel=0;
        preferences.autoFlush=0;
        for(int i=0; i<4;i++) preferences.reserved[i]=0;
        preferences.frameInfo.blockSizeID=LZ4F_default;
        preferences.frameInfo.blockMode=LZ4F_blockLinked;
        preferences.frameInfo.contentChecksumFlag=LZ4F_noContentChecksum;
        preferences.frameInfo.frameType=LZ4F_frame;
        preferences.frameInfo.contentSize=0;
        preferences.frameInfo.reserved[0]=0;
        preferences.reserved[1]=0;
      }
      ~lz4IO()
      {
        LZ4F_errorCode_t err=LZ4F_freeCompressionContext(ctx);
        if(LZ4F_isError(err))
        {
          std::cerr<<"LZ4F_freeCompressionContext failed!"<<std::endl;
        }
        err=LZ4F_freeDecompressionContext(dctx);
        if(LZ4F_isError(err))
        {
          std::cerr<<"LZ4F_freeDecompressionContext failed!"<<std::endl;
        }
      }

      //@brief lz4で圧縮されたデータを読み込んで伸長したうえでptrへ書き込む
      //@param ptr     読み込んだデータを格納する領域へのポインタ
      //@param size    データの1要素あたりのサイズ
      //@param nmemb   確保済の領域サイズ
      //@param stream  ファイル入力元のポインタ
      //@param ret 伸長後のデータサイズ(Byte)
      //
      //エラー発生時はstderrにメッセージを出力した上で0を返す
      //伸長後のファイルサイズが0だった場合も0が返るので注意
      size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
      {
        const int HEADER_SIZE=4;
        LZ4F_decompressOptions_t dOpt={0};
        size_t total_read_size=0;
        size_t decompressed_size=0;
        const size_t size_in_byte=size*nmemb;
        const size_t input_buffer_size=buffer_size > 11? buffer_size:11; //TODO check!!
        char* buffer = new char[input_buffer_size];
        size_t offset=0;
        char* dst=(char* )ptr;
        char* src=buffer;

        size_t read_size = ::fread(buffer, 1, (size_t)input_buffer_size, stream);
        if (ferror(stream))
        {
          std::cerr<<"file read error."<<std::endl;
          delete [] buffer;
          return 0;
        }
        for(;;)
        {
          size_t dst_size=size_in_byte;
          size_t src_size=read_size;
          size_t rt = LZ4F_decompress(dctx, dst, &dst_size, src, &src_size, &dOpt);
          if(LZ4F_isError(rt))
          {
            std::cerr<<"decompress error: "<<LZ4F_getErrorName(rt)<<std::endl;
            break;
          }

          // srcの更新
          if(src_size < read_size) {
            src+=src_size;
            read_size-=src_size;
          }else{
            read_size = ::fread(buffer, 1, (size_t)input_buffer_size, stream);
            src=buffer;
            if (ferror(stream))
            {
              std::cerr<<"file read error."<<std::endl;
              break;
            }
          }

          //dstの更新
          dst+=dst_size;

          //伸長済サイズを更新
          decompressed_size+=dst_size;
          if(decompressed_size >= size_in_byte) break;
        }

        delete [] buffer;
        return decompressed_size;
      }


      //@brief lz4を使って圧縮したデータをファイルに出力する
      //@param ptr     圧縮するデータを格納した領域へのポインタ
      //@param size    圧縮するデータの1要素の長さ
      //@param nmemb   圧縮するデータの要素数
      //@param stream  ファイル出力先のポインタ
      //@ret   出力したファイルサイズ(Byte)
      //
      //
      //lz4の初期化に失敗した場合は、メッセージを出力した上で無圧縮のファイルを出力する
      //圧縮中にエラーが発生した場合は、メッセージを出力した上で0を返す
      size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
      {
        size_t output_size=0;
        LZ4F_compressOptions_t cOpt={0};
        const size_t dst_size = LZ4F_compressBound(buffer_size, &preferences)+19;
        unsigned char* dst = new unsigned char[dst_size];

        const size_t size_in_byte=size*nmemb;
        unsigned int reminder = size_in_byte%buffer_size;
        const int num_block = size_in_byte/buffer_size;

        char* src=(char *)ptr;

        size_t header_size = LZ4F_compressBegin(ctx, dst, dst_size, &preferences);
        if (LZ4F_isError(header_size))
        {
          std::cerr<<"Header generation failed: "<<LZ4F_getErrorName(header_size)<<std::endl;
          delete [] dst;
          return 0;
        }
        size_t rt=::fwrite(dst, 1, header_size, stream);
        if(rt!=header_size)
        {
          std::cerr<<"Header write failed: "<<LZ4F_getErrorName(header_size)<<std::endl;
          delete [] dst;
          return 0;
        }

        for(int i=0; i<num_block;i++)
        {
          size_t compressed_size=LZ4F_compressUpdate(ctx, dst, dst_size, src, buffer_size, &cOpt);
          if(LZ4F_isError(compressed_size))
          {
            std::cerr<<"Compress failed: "<<LZ4F_getErrorName(compressed_size)<<std::endl;
          }
          size_t rt=::fwrite(dst, 1, compressed_size, stream);
          if(rt != compressed_size)
          {
            std::cerr<<"file output failed! "<<std::endl;
            delete [] dst;
            return output_size;
          }
          output_size += buffer_size;
          src+=buffer_size;
        }
        if(reminder > 0)
        {
          size_t compressed_size=LZ4F_compressUpdate(ctx, dst, dst_size, src, reminder, &cOpt);
          if(LZ4F_isError(compressed_size))
          {
            std::cerr<<"Compress failed: "<<LZ4F_getErrorName(compressed_size)<<std::endl;
          }
          size_t rt = ::fwrite(dst, 1, compressed_size, stream);
          if(rt != compressed_size)
          {
            std::cerr<<"file output failed! "<<std::endl;
            delete [] dst;
            return output_size;
          }
          output_size += reminder;
          src+=reminder;
        }
        size_t footer_size = LZ4F_compressEnd(ctx, dst, dst_size, &cOpt);
        if (LZ4F_isError(footer_size))
        {
          std::cerr<<"footer generation failed: "<<LZ4F_getErrorName(header_size)<<std::endl;
          delete [] dst;
          return 0;
        }
        rt = ::fwrite(dst, 1, footer_size, stream);
        if(rt != footer_size)
        {
          std::cerr<<"file output failed! "<<std::endl;
          delete [] dst;
          return output_size;
        }

        delete [] dst;
        return output_size;
      }

    private:
      const int buffer_size;
      LZ4F_preferences_t preferences;
      LZ4F_compressionContext_t ctx;
      LZ4F_decompressionContext_t dctx;
  };

}//end of namespace JHPCNDF
#endif
