#include "helpers.hpp"

void float2Bytes(unsigned char bytes_temp[4],float float_variable){ 
  memcpy(bytes_temp, (unsigned char*) (&float_variable), 4);
}

void int2Bytes(unsigned char bytes_temp[4], int int_variable){
    for(int i=0; i < 4; ++i)
        bytes_temp[i] = (int_variable >> (8*i)) & 0xFF;
}
