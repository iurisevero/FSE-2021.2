#ifndef UART_HPP
#define UART_HPP

#include <stdio.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART
#include "helpers.hpp"

int abrir_uart(char * file_path);
int envia_dados(int uart_filestream, unsigned char * tx_buffer, size_t size_buffer);
int recebe_dado(int uart_filestream, int * int_buffer);
int recebe_dado(int uart_filestream, float * float_buffer);
int recebe_dado(int uart_filestream, char * rx_buffer);

#endif