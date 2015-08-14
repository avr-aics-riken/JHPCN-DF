#include <stdlib.h>
#include <stdio.h>
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

  REAL_TYPE* encoded= malloc(num_data*sizeof(REAL_TYPE));
  REAL_TYPE* encoded_lower= malloc(num_data*sizeof(REAL_TYPE));


  // Encode
#ifdef _REAL_IS_DOUBLE_
  JHPCNDF_encode_double(num_data, random_data,  encoded, encoded_lower, tolerance, 1, "binary_search");
#else
  JHPCNDF_encode_float(num_data, random_data,  encoded, encoded_lower, tolerance, 1, "binary_search");
#endif


  //
  // JHPCN-DF Read example
  //

  REAL_TYPE* work = malloc(num_data*sizeof(REAL_TYPE));

  // Read and decode
#ifdef _REAL_IS_DOUBLE_
  JHPCNDF_decode_double(num_data, encoded, encoded_lower, work);
#else
  JHPCNDF_decode_float(num_data, encoded, encoded_lower, work);
#endif



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
