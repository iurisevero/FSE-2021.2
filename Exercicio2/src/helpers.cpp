#include "helpers.hpp"

#include <stdio.h>

void float2Bytes(unsigned char bytes_temp[4], float * float_variable){ 
    memcpy(bytes_temp, (unsigned char*) (float_variable), 4);
}

void int2Bytes(unsigned char bytes_temp[4], int * int_variable){
    memcpy(bytes_temp, (unsigned char*) (int_variable), 4);
}

size_t get_size(unsigned char * begin, unsigned char * end){
    return end - begin;
}

void add_crc(unsigned short crc, void * dest){
    memcpy(dest, &crc, sizeof(unsigned short));
}

void print_arr_hex(unsigned char * arr, size_t size){
    for(int i=0; i < size; ++i)
        printf("%x ", arr[i]);
    printf("\n");
}