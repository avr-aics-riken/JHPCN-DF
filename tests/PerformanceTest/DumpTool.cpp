/*
 * JHPCN-DF - Data compression library based on
 *            Jointed Hierarchical Precision Compression Number Data Format
 *
 * Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 */

// @file ValidationTool.cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <cstdlib>
#include "jhpcndf.h"
#include "Utility.h"
#include "real_type.h"

int main(int argc, char *argv[])
{
    if(argc <5)
    {
        std::cerr<<"usage: "<<argv[0]<<" number_of_data indices original_data encoded_data..."<<std::endl;
        std::cerr<<"  indices must be camma separated numbers without any space"<<std::endl;
        std::cerr<<"    good:"<<std::endl;
        std::cerr<<"     0,6,2,31"<<std::endl;
        std::cerr<<"    bad:"<<std::endl;
        std::cerr<<"     0-5,  7, 2"<<std::endl;
        exit(1);
    }

    const size_t num_data=std::atol(argv[1]);

    std::list<size_t> indices;
    std::istringstream iss(argv[2]);
    std::string tmp;
    while(getline(iss, tmp, ','))
    {
        indices.push_back(std::atol(tmp.c_str()));
    }

    indices.sort();
    for(std::list<size_t>::iterator it=indices.begin(); it != indices.end(); ++it)
    {
        if (*it>=num_data)
        {
            std::cerr<<"indices which greater than num_data is omitted"<<std::endl;
            indices.erase(it, indices.end());
            break;
        }
    }


    REAL_TYPE* work=new REAL_TYPE [num_data];

    std::vector<std::string> filenames;
    for (int i=3; i<argc; i++)
    {
        filenames.push_back(argv[i]);
    }

    // 最も長いファイル名の長さを求める
    size_t max=0;
    for(std::vector<std::string>::iterator it=filenames.begin(); it!=filenames.end(); ++it)
    {
        max = it->size() > max ? it->size() : max;
    }


    //ファイルを読んで指定された位置の値を出力する
    for(std::list<size_t>::iterator it_index=indices.begin(); it_index != indices.end(); ++it_index)
    {
        for(std::vector<std::string>::iterator it=filenames.begin(); it!=filenames.end(); ++it)
        {
            std::cerr<<std::setw(max) << *it <<" : ";
            int key=JHPCNDF::fopen(*it, "", "rb");
            JHPCNDF::fread(work, sizeof(REAL_TYPE), num_data, key);
            JHPCNDF::fclose(key);
            output_binary(std::cerr, work[*it_index]);
            std::cerr<<":"<<std::scientific<<work[*it_index]<<std::endl;
        }
    }
    return 0;
}
