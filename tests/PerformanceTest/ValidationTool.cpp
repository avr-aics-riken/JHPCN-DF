#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include "jhpcndf.h"
#include "Utility.h"
#include <map>
#include "real_type.h"

int main(int argc, char *argv[])
{
    if(argc <5)
    {
        std::cerr<<"usage: "<<argv[0]<<" size_of_data_in_kiB original_data encoded_data_pair(s)"<<std::endl;
        exit(1);
    }

    const size_t num_data=std::atol(argv[1])*1024/sizeof(REAL_TYPE);
    REAL_TYPE* original=new REAL_TYPE [num_data];
    REAL_TYPE* work=new REAL_TYPE [num_data];

    //store encoded_data_file pairs
    std::vector<std::pair<std::string, std::string> > filenames;
    for (int i=argc-1; i>2; i-=2)
    {
        filenames.push_back(std::make_pair(argv[i-1], argv[i]));
    }

    //read original_data
    int key=JHPCNDF::fopen(argv[2], "rb");
    JHPCNDF::fread(original, sizeof(REAL_TYPE), num_data, key);
    JHPCNDF::fclose(key);

    for(std::vector<std::pair<std::string, std::string> >::iterator it=filenames.begin(); it!=filenames.end(); ++it)
    {
        std::cerr<<"check for "<<it->first<<" and "<<it->second<<" start"<<std::endl;
        int key=JHPCNDF::fopen(it->first, it->second, "rb");
        JHPCNDF::fread(work, sizeof(REAL_TYPE), num_data, key);
        JHPCNDF::fclose(key);
        for(size_t i=0; i<num_data; i+=num_data/2)
        {
            std::cerr<< i <<" th element is"<<std::endl;
            std::cerr<< "original :";
            output_binary(original[i]);
            std::cerr<< "decoded  :";
            output_binary(work[i]);
        }
    }
    return 0;
}
