#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <string.h>

void float2Bytes(unsigned char bytes_temp[4],float float_variable);
void int2Bytes(unsigned char bytes_temp[4], int int_variable);
size_t get_size(unsigned char * begin, unsigned char * end);
void add_crc(unsigned short crc, void * dest);

#endif