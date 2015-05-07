#include <iostream>
#include <cstdlib>
#include "jhpcndf.h"

#define NUM_DATA 10000
#ifdef REAL_8_BYTE
#define REAL_TYPE double
#else
#define REAL_TYPE float
#endif

int main(int argc, char *argv[])
{

    //
    // JHPCN-DF Write example
    //

    // SetUp Data
    const float tolerance=0.01;
    const int num_data=NUM_DATA;
    REAL_TYPE* random_data= new REAL_TYPE[num_data];
    for(int i=0; i<num_data; i++)
    {
        random_data[i]=std::rand()/((REAL_TYPE)(RAND_MAX)+1);
    }

    // Open file
    int key=JHPCNDF::fopen("upper_bits.gz", "lower_bits.gz", "w+b");

    // Encode and write
    JHPCNDF::fwrite(random_data, sizeof(REAL_TYPE), num_data, key, tolerance);

    // Close file
    JHPCNDF::fclose(key);


    //
    // JHPCN-DF Read example
    //

    REAL_TYPE* work = new REAL_TYPE[num_data];

    // OpenFile
    int key2=JHPCNDF::fopen("upper_bits.gz", "lower_bits.gz", "rb");

    // Read and decode
    JHPCNDF::fread(work, sizeof(REAL_TYPE), num_data, key2);

    // Close file
    JHPCNDF::fclose(key2);


    // compare files
    int error_count=0;
    for(int i=0; i<num_data; i++)
    {
        if (random_data[i] != work[i])
        {
            if(error_count <10)
            {
                std::cerr << i << " th data is not same."<<std::endl;
            }
            error_count++;
        }
    }
    if (error_count >0)
    {
        std::cerr << error_count << " entry is not correctly restored."<<std::endl;
    }else{
        std::cerr << num_data <<" data is correctly write and read."<<std::endl;
    }

    return 0;
}
