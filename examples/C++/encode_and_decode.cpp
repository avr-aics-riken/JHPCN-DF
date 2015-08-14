#include <iostream>
#include <cstdlib>
#include "jhpcndf.h"

#define NUM_DATA 100000

#ifdef _REAL_IS_DOUBLE_
#define REAL_TYPE double
#else
#define REAL_TYPE float
#endif

int main(int argc, char *argv[])
{

    //
    // JHPCN-DF Encode example
    //

    // SetUp Data
    const float tolerance=0.01;
    const int num_data=NUM_DATA;
    REAL_TYPE* random_data= new REAL_TYPE[num_data];
    for(int i=0; i<num_data; i++)
    {
        random_data[i]=std::rand()/((REAL_TYPE)(RAND_MAX)+1);
    }

    REAL_TYPE* encoded = new REAL_TYPE[num_data];
    REAL_TYPE* encoded_lower = new REAL_TYPE[num_data];

    // encode
    JHPCNDF::encode(num_data, random_data, encoded, encoded_lower, tolerance);


    //
    // JHPCN-DF Decode example
    //

    REAL_TYPE* work = new REAL_TYPE[num_data];

    // decode
    JHPCNDF::decode(num_data, encoded, encoded_lower, work);

    delete [] encoded;
    delete [] encoded_lower;

    // compare original and decoded data
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
        std::cerr << num_data <<" data is correctly encoded and decoded."<<std::endl;
    }

    return 0;
}
