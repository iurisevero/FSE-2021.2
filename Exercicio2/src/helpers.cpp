#include "helpers.hpp"
#include "crc16.hpp"

#include <stdio.h>

void float2Bytes(unsigned char tempBytes[4], float * floatVariable){ 
    memcpy(tempBytes, (unsigned char*) (floatVariable), 4);
}

void int2Bytes(unsigned char tempBytes[4], int * intVariable){
    memcpy(tempBytes, (unsigned char*) (intVariable), 4);
}

size_t getSize(unsigned char * begin, unsigned char * end){
    return end - begin;
}

void addCRC(unsigned short crc, void * dest){
    memcpy(dest, &crc, sizeof(unsigned short));
}

bool checkCRC(unsigned char * message, size_t size){
    unsigned short crc_atual;
    size_t _size = sizeof(unsigned short);
    memcpy(&crc_atual, &message[size - _size], _size);
    return crc_atual == calcula_CRC(message, size - _size);
}

bool checkErrorBit(unsigned char * message){
    unsigned char cmd = message[1];
    unsigned char mask = 0x80;
    return (cmd & mask) != 0x80;
}

void printArrHex(char * arr, size_t size){
    for(size_t i=0; i < size; ++i)
        printf("%x ", arr[i]);
    printf("\n");
}

void printArrHex(unsigned char * arr, size_t size){
    for(size_t i=0; i < size; ++i)
        printf("%x ", arr[i]);
    printf("\n");
}