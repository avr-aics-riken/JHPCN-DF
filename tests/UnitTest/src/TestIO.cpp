/*
 * JHPCN-DF - Data compression library based on
 *            Jointed Hierarchical Precision Compression Number Data Format
 *
 * Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 */

// @file TestIO.cpp

#include "gtest/gtest.h"
#include "IO.h"

class IOTest : public ::testing::TestWithParam<std::tr1::tuple <const char*, size_t> >
{
  protected:
    IOTest():size(1),random_data(NULL), read_data(NULL){}
    void init_data(const size_t& n)
    {
      for(size_t i=0; i<n; i++)
      {
        random_data[i]='A'+i%26;
        read_data[i]='a';
      }
    }
    virtual void SetUp()
    {
      nmemb=std::tr1::get<1>(GetParam());
      random_data=new char [nmemb];
      read_data=new char [nmemb];

      std::ostringstream oss;
      oss<<nmemb;
      std::string filename(std::tr1::get<0>(GetParam()));
      filename += "_";
      filename +=oss.str()+".gz";

      fp=fopen(filename.c_str(), "w+b");
    }
    virtual void TearDown()
    {
      fclose(fp);
      delete [] random_data;
      delete [] read_data;
    }
    FILE* fp;
    size_t size;
    size_t nmemb;
    char* random_data;
    char* read_data;
};

TEST_P(IOTest, WriteAndRead)
{
  ASSERT_TRUE(random_data != NULL);
  ASSERT_TRUE(read_data != NULL);
  init_data(nmemb);
  JHPCNDF::IO* io = JHPCNDF::IOFactory(std::tr1::get<0>(GetParam()), 32768);
  ASSERT_TRUE(io != NULL);

  size_t write_size = io->fwrite(random_data, size, nmemb, fp);
  fflush(fp);
  rewind(fp);
  size_t read_size = io->fread(read_data, size, nmemb, fp);

  EXPECT_EQ(write_size, read_size);
  for(size_t i=0; i<nmemb; i++)
  {
    ASSERT_EQ(random_data[i], read_data[i]) << "i = "<< i;
  }
}

INSTANTIATE_TEST_CASE_P(GzipTest, IOTest, ::testing::Combine(
      ::testing::Values("gzip", "gzip_1"),
      ::testing::Values(1,
                        32767,
                        32768,
                        32769,
                        2147483647,
                        2147483648, //2GB
                        2147483649,
                        4294967295,
                        4294967296, //4GB
                        4294967297)
      ));
#ifdef USE_LZ4
INSTANTIATE_TEST_CASE_P(LZ4Test, IOTest, ::testing::Combine(
      ::testing::Values("lz4"),
      ::testing::Values(1, 
                        32767,
                        32768,
                        32769,
                        7942900,
                        79429000,
                        794290000,
                        2147483647,
                        2147483648, //2GB
                        2147483649,
                        4294967295,
                        4294967296, //4GB
                        4294967297)
      ));
#endif
