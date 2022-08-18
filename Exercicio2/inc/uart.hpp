#ifndef UART_HPP
#define UART_HPP

#include <stdio.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART
#include "helpers.hpp"

int openUart(char * file_path);
int sendData(int uartFilestream, unsigned char * txBuffer, size_t sizeBuffer);
int receiveData(int uartFilestream, char * rxBuffer);

#endif