#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <string.h>

void float2Bytes(unsigned char tempBytes[4],float * floatVariable);
void int2Bytes(unsigned char tempBytes[4], int * intVariable);
size_t getSize(unsigned char * begin, unsigned char * end);
void addCRC(unsigned short crc, void * dest);
bool checkCRC(unsigned char * message, size_t size);
bool checkErrorBit(unsigned char * message);
void printArrHex(char * arr, size_t size);
void printArrHex(unsigned char * arr, size_t size);

#endif