#include "uart.hpp"
#include "helpers.hpp"

#include <stdlib.h>

int openUart(char * file_path){
    int uartFilestream = -1;
    uartFilestream = open(file_path, O_RDWR | O_NOCTTY | O_NDELAY);      //Open in non blocking read/write mode
    if (uartFilestream == -1)
    {
        printf("Erro - Não foi possível iniciar a UART.\n");
        return uartFilestream;
    }
    else
    {
        printf("UART inicializada!\n");
    }    
    struct termios options;
    tcgetattr(uartFilestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;     //<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uartFilestream, TCIFLUSH);
    tcsetattr(uartFilestream, TCSANOW, &options);

    return uartFilestream;
}

int sendData(int uartFilestream, unsigned char * txBuffer, size_t sizeBuffer){
    if (uartFilestream != -1){
        printf("Escrevendo caracteres na UART ...");
        int count = write(uartFilestream, txBuffer, sizeBuffer);
        if (count < 0){
            printf("UART TX error\n");
            return count;
        }
        else{
            printf("Escrito.\n");
            return 0;
        }
    }
    
    printf("Filestream da UART inválido\n");
    return -1;
}

int receiveData(int uartFilestream, char * rxBuffer, int _size){
    if (uartFilestream != -1){
        int actual_size = 0, expected_size = _size;
        do{
            char * aux_buffer = (char *) malloc(expected_size + 1);
            int rx_length = read(uartFilestream, (void *) aux_buffer, expected_size);
            if (rx_length < 0){
                printf("Erro na leitura.\n"); //An error occured (will occur if there are no bytes)
                return rx_length;
            }
            else if (rx_length == 0){
                printf("Nenhum dado disponível.\n"); //No data waiting
                return actual_size;
            }

            // Debug
            aux_buffer[rx_length] = '\0';
            printf("%i Bytes lidos\n", rx_length);
            printArrHex(aux_buffer, rx_length);

            memcpy(&rxBuffer[actual_size], aux_buffer, rx_length);
            actual_size += rx_length;
            expected_size -= rx_length;
            
            free(aux_buffer);
        } while(expected_size);
    }
    printf("Filestream da UART inválido\n");
    return -1;
}