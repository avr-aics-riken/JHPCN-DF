#include <stdlib.h>
#include <stdio.h>
#include "jhpcndf.h"

#define NUM_DATA 100000
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
    REAL_TYPE* random_data= malloc(num_data*sizeof(REAL_TYPE));
    for(int i=0; i<num_data; i++)
    {
        random_data[i]=rand()/((REAL_TYPE)(RAND_MAX)+1);
    }

    // Open file
    int key=JHPCNDF_fopen("upper_bits.gz", "lower_bits.gz", "w+b");

    // Encode and write
#ifdef REAL_8_BYTE
    JHPCNDF_fwrite_double(random_data, sizeof(REAL_TYPE), num_data, key, tolerance, "normal", "gzip");
#else
    JHPCNDF_fwrite_float(random_data, sizeof(REAL_TYPE), num_data, key, tolerance, "normal", "gzip");
#endif

    // Close file
    JHPCNDF_fclose(key);


    //
    // JHPCN-DF Read example
    //

    REAL_TYPE* work = malloc(num_data*sizeof(REAL_TYPE));

    // OpenFile
    int key2=JHPCNDF_fopen("upper_bits.gz", "lower_bits.gz", "rb");

    // Read and decode
#ifdef REAL_8_BYTE
    JHPCNDF_fread_double(work, sizeof(REAL_TYPE), num_data, key2);
#else
    JHPCNDF_fread_float(work, sizeof(REAL_TYPE), num_data, key2);
#endif

    // Close file
    JHPCNDF_fclose(key2);


    // compare files
    int error_count=0;
    for(int i=0; i<num_data; i++)
    {
        if (random_data[i] != work[i])
        {
            if(error_count <10)
            {
                fprintf(stderr, "%d th data is not same.\n",i);
            }
            error_count++;
        }
    }
    if (error_count >0)
    {
        fprintf(stderr, "%d entry is not correctly restored.\n", error_count);
    }else{
        fprintf(stderr, "%d data is correctly write and read.\n", num_data);
    }

    return 0;
}