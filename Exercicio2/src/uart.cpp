#include "uart.hpp"

int abrir_uart(char * file_path){
    int uart_filestream = -1;
    uart_filestream = open(file_path, O_RDWR | O_NOCTTY | O_NDELAY);      //Open in non blocking read/write mode
    if (uart_filestream == -1)
    {
        printf("Erro - Não foi possível iniciar a UART.\n");
        return uart_filestream;
    }
    else
    {
        printf("UART inicializada!\n");
    }    
    struct termios options;
    tcgetattr(uart_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;     //<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart_filestream, TCIFLUSH);
    tcsetattr(uart_filestream, TCSANOW, &options);

    return uart_filestream;
}

int envia_dados(int uart_filestream, unsigned char * tx_buffer, size_t size_buffer){
    if (uart_filestream != -1){
        printf("Escrevendo caracteres na UART ...");
        int count = write(uart_filestream, tx_buffer, size_buffer);
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

int recebe_dado(int uart_filestream, int * int_buffer){
    if (uart_filestream != -1){
        int rx_length = read(uart_filestream, (void*)int_buffer, 4);
        if (rx_length < 0){
            printf("Erro na leitura.\n"); //An error occured (will occur if there are no bytes)
            return rx_length;
        }
        else if (rx_length == 0){
            printf("Nenhum dado disponível.\n"); //No data waiting
            return 0;
        }
        else{
            printf("%i Bytes lidos : %d\n", rx_length, *int_buffer);
            return rx_length;
        }
    }
    printf("Filestream da UART inválido\n");
    return -1;
}

int recebe_dado(int uart_filestream, float * float_buffer){
    if (uart_filestream != -1){
        int rx_length = read(uart_filestream, (void*)float_buffer, 4);
        if (rx_length < 0){
            printf("Erro na leitura.\n"); //An error occured (will occur if there are no bytes)
            return rx_length;
        }
        else if (rx_length == 0){
            printf("Nenhum dado disponível.\n"); //No data waiting
            return 0;
        }
        else{
            printf("%i Bytes lidos : %f\n", rx_length, *float_buffer);
            return rx_length;
        }
    }
    printf("Filestream da UART inválido\n");
    return -1;
}

int recebe_dado(int uart_filestream, char * rx_buffer){
    if (uart_filestream != -1){
        char _size;
        int rx_length = read(uart_filestream, (void*)&_size, 1);
        if (rx_length < 0){
            printf("Erro na leitura.\n"); //An error occured (will occur if there are no bytes)
            return rx_length;
        }
        else if (rx_length == 0){
            printf("Nenhum dado disponível.\n"); //No data waiting
            return 0;
        }
        else{
            printf("A String recebida possui %d bytes.\n", (int) _size);

            rx_length = read(uart_filestream, (void*)rx_buffer, _size);
            if (rx_length < 0){
                printf("Erro na leitura.\n"); //An error occured (will occur if there are no bytes)
                return rx_length;
            }
            else if (rx_length == 0){
                printf("Nenhum dado disponível.\n"); //No data waiting
                return 0;
            }
            else{
                rx_buffer[rx_length] = '\0';
                printf("%i Bytes lidos : %s\n", rx_length, rx_buffer);
                return rx_length;
            }
        }
    }
    printf("Filestream da UART inválido\n");
    return -1;
}