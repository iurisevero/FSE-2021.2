#include "helpers.hpp"
#include "crc16.hpp"
#include "consts.hpp"

#include <stdio.h>

ssize_t getSize(unsigned char * begin, unsigned char * end){
    return end - begin;
}

void addCRC(unsigned short crc, void * dest){
    memcpy(dest, &crc, sizeof(unsigned short));
}

bool checkCRC(unsigned char * message, ssize_t size){
    unsigned short crc_atual;
    ssize_t _size = sizeof(unsigned short);
    memcpy(&crc_atual, &message[size - _size], _size);
    return crc_atual == calcula_CRC(message, size - _size);
}

bool checkErrorBit(unsigned char * message){
    unsigned char cmd = message[1];
    unsigned char mask = 0x80;
    return (cmd & mask) != 0x80;
}

bool checkReceivedData(
    unsigned char * message,
    unsigned char expectedAddr,
    unsigned char expectedCode,
    unsigned char expectedSubCode
){
    if(message[0] != expectedAddr)    return false;
    if(message[1] != expectedCode)    return false;
    if(message[3] != expectedSubCode) return false;
}

void addMatricula(void * dest){
    memcpy(dest, MATRICULA, MATRICULA_SIZE);
}

void printArrHex(unsigned char * arr, ssize_t size){
    for(ssize_t i=0; i < size; ++i)
        printf("%x ", arr[i]);
    printf("\n");
}