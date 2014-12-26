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
                debug_write(length, dst, src_upper, src_lower);
#endif
            }
        private:
            void debug_write(const size_t& length, const T* const org, const T* const upper, const T* const lower) const
            {
                std::cerr<<"original[0]   =";
                output_binary(org[0]);
                std::cerr<<"upper_bits[0] =";
                output_binary(upper[0]);
                if(lower!=NULL)
                {
                    std::cerr<<"lower_bits[0] =";
                    output_binary(lower[0]);
                }
                std::cerr<<"original["<<length/2<<"]   =";
                output_binary(org[length/2]);
                std::cerr<<"upper_bits["<<length/2<<"] =";
                output_binary(upper[length/2]);
                if(lower!=NULL)
                {
                    std::cerr<<"lower_bits["<<length/2<<"] =";
                    output_binary(lower[length/2]);
                }
            }
    };
}//end of namespace JHPCNDF
#endif

