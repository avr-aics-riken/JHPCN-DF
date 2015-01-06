/*
 * JHPCN-DF - Data compression library based on
 *            Jointed Hierarchical Precision Compression Number Data Format
 *
 * Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 */

// @file TestZeroPadding.cpp

#include "gtest/gtest.h"
#include "IO.h"

class ZlibIOTest : public ::testing::TestWithParam<size_t>
{
    protected:
    virtual void SetUp()
    {
        size = 1;
        nmemb=GetParam();
        random_data=new char [nmemb];
        read_data=new char [nmemb];
        std::ostringstream oss;
        oss<<nmemb;
        std::string filename("test_data_");
        filename+=oss.str()+".gz";
        for(size_t i=0; i<nmemb; i++)
        {
            random_data[i]='A'+rand()%26;
            read_data[i]='A';
        }

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

TEST_P(ZlibIOTest, WriteAndRead)
{
    JHPCNDF::IO* io = JHPCNDF::IOFactory("gzip");

    size_t write_size = io->fwrite(random_data, size, nmemb, fp);
    rewind(fp);
    size_t read_size = io->fread(read_data, size, nmemb, fp);

    EXPECT_EQ(write_size, read_size);
    for(size_t i=0; i<nmemb; i++)
    {
        EXPECT_EQ(random_data[i], read_data[i]);
    }
}

INSTANTIATE_TEST_CASE_P(ZlibIOTest, ZlibIOTest, ::testing::Values(1, 32767, 32768, 32769, UINT_MAX/2-1, UINT_MAX/2, UINT_MAX/2+1, UINT_MAX*1.5, UINT_MAX*3));
