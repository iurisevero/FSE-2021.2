#ifndef MODBUS_HPP
#define MODBUS_HPP

#include <stdlib.h>

ssize_t assembleProtocol(
    unsigned char * message,
    unsigned char cmd,
    bool isRequest=false
);
ssize_t sendDataRequest(unsigned char cmd);
ssize_t sendData(int value);
ssize_t sendData(float value);
ssize_t sendData(char * value);
ssize_t receiveData(int * value);
ssize_t receiveData(float * value);
ssize_t receiveData(char * value);
ssize_t receiveConstantSizeData(
    unsigned char * message,
    ssize_t expectedSize,
    unsigned char subCmd
);

#endif