/*
 * JHPCN-DF - Data compression library based on
 *            Jointed Hierarchical Precision Compression Number Data Format
 *
 * Copyright (c) 2014-2015 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 */

// @file Decoder.h


#ifndef DCODER_H
#define DCODER_H
#include "Utility.h"

namespace JHPCNDF
{
    template <typename T>
    class Decoder 
    {
        public:
            void operator()(const size_t& length, const T* const src_upper, const T* const src_lower, T* const dst) const
            {
                for (size_t i=0;i<length;i++)
                {
                    real_or<1>(&(src_upper[i]), &(src_lower[i]), &(dst[i]));
                }
#ifdef DEBUG
                debug_write(0, dst, src_upper, src_lower);
                debug_write(length/2, dst, src_upper, src_lower);
#endif
            }
        private:
            void debug_write(const size_t& index, const T* const org, const T* const upper, const T* const lower) const
            {
                std::cerr<<"original["<<index<<"]   = ";
                output_binary(std::cerr, org[index]);
                std::cerr<<"\n";
                std::cerr<<"upper_bits["<<index<<"] = ";
                output_binary(std::cerr, upper[index]);
                std::cerr<<"\n";
                if(lower!=NULL)
                {
                    std::cerr<<"lower_bits["<<index<<"] = ";
                    output_binary(std::cerr, lower[0]);
                    std::cerr<<"\n";
                }
            }
    };
}//end of namespace JHPCNDF
#endif

