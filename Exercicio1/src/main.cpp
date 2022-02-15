#include <stdio.h>
#include <string.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART

const int CMD_SOLICITA_INT = 0xA1;
const int CMD_SOLICITA_FLOAT = 0xA2;
const int CMD_SOLICITA_STRING = 0xA3;
const int CMD_ENVIA_INT = 0xB1;
const int CMD_ENVIA_FLOAT = 0xB2;
const int CMD_ENVIA_STRING = 0xB3;
const int MATRICULA[] = {5, 5, 1, 4};

int abrir_uart(char * file_path);
int solicita_dado(int uart_filestream, int cmd);
int envia_pacote_de_dados(int uart_filestream, int cmd);
int envia_dados(int uart_filestream, int * tx_buffer, size_t size_buffer);
int envia_dados(int uart_filestream, float * tx_buffer, size_t size_buffer);
int envia_dados(int uart_filestream, unsigned char * tx_buffer, size_t size_buffer);
int recebe_dado(int uart_filestream, int cmd);
int recebe_dado(int uart_filestream, int * int_buffer);
int recebe_dado(int uart_filestream, float * float_buffer);
int recebe_dado(int uart_filestream, char * rx_buffer);

void float2Bytes(unsigned char bytes_temp[4],float float_variable){ 
  memcpy(bytes_temp, (unsigned char*) (&float_variable), 4);
}

void int2Bytes(unsigned char bytes_temp[4], int int_variable){
    for(int i=0; i < 4; ++i)
        bytes_temp[i] = (int_variable >> (8*i)) & 0xFF;
}


int main(int argc, const char * argv[]) {
    char uart_path[] = "/dev/serial0";
    int uart_filestream = abrir_uart(uart_path);
    solicita_dado(uart_filestream, CMD_SOLICITA_INT);
    sleep(1);
    recebe_dado(uart_filestream, CMD_SOLICITA_INT);
    sleep(1);
    envia_pacote_de_dados(uart_filestream, CMD_ENVIA_INT);
    sleep(1);
    close(uart_filestream);
    
    sleep(5);
    
    uart_filestream = abrir_uart(uart_path);
    solicita_dado(uart_filestream, CMD_SOLICITA_FLOAT);
    sleep(1);
    recebe_dado(uart_filestream, CMD_SOLICITA_FLOAT);
    sleep(1);
    envia_pacote_de_dados(uart_filestream, CMD_ENVIA_FLOAT);
    sleep(1);
    close(uart_filestream);

    sleep(5);

    uart_filestream = abrir_uart(uart_path);
    solicita_dado(uart_filestream, CMD_SOLICITA_STRING);
    sleep(1);
    recebe_dado(uart_filestream, CMD_SOLICITA_STRING);
    sleep(1);
    envia_pacote_de_dados(uart_filestream, CMD_ENVIA_STRING);
    sleep(1);
    close(uart_filestream);
   return 0;
}

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

int solicita_dado(int uart_filestream, int cmd){
    unsigned char pacote_solicitacao_de_dados[5];
    unsigned char *p_pacote_solicitacao_de_dados;
    p_pacote_solicitacao_de_dados = &pacote_solicitacao_de_dados[0];

    switch (cmd)
    {
    case CMD_SOLICITA_INT:
        *p_pacote_solicitacao_de_dados++ = cmd;
        break;
    case CMD_SOLICITA_FLOAT:
        *p_pacote_solicitacao_de_dados++ = cmd;
        break;
    case CMD_SOLICITA_STRING:
        *p_pacote_solicitacao_de_dados++ = cmd;
        break;
    default:
        printf("Comando invalido\n");
        return -1;
    }

    for(int i=0; i < 4; ++i)
        *p_pacote_solicitacao_de_dados++ = MATRICULA[i];

    size_t _size = (p_pacote_solicitacao_de_dados - &pacote_solicitacao_de_dados[0]);
    return envia_dados(uart_filestream, &pacote_solicitacao_de_dados[0], _size);
}

int envia_pacote_de_dados(int uart_filestream, int cmd){
    unsigned char pacote_envio_de_dados[260];
    unsigned char * p_pacote_envio_de_dados = &pacote_envio_de_dados[0];
    int int_dado = 7;
    float float_dado = 7.7777;
    char str_dado[11] = "Uma frase!";


    switch (cmd)
    {
    case CMD_ENVIA_INT:
        unsigned char int_bytes[4];
        int2Bytes(int_bytes, int_dado);
        *p_pacote_envio_de_dados++ = cmd;
        for(int i=0; i < 4; ++i)
            *p_pacote_envio_de_dados++ = int_bytes[i];
        for(int i=0; i < 4; ++i)
            *p_pacote_envio_de_dados++ = MATRICULA[i];
        break;
    case CMD_ENVIA_FLOAT:
        *p_pacote_envio_de_dados++ = cmd;
        unsigned char float_bytes[4];
        float2Bytes(float_bytes, float_dado);
        for(int i=0; i < 4; ++i)
            *p_pacote_envio_de_dados++ = float_bytes[i];
        for(int i=0; i < 4; ++i)
            *p_pacote_envio_de_dados++ = MATRICULA[i];
        break;
    case CMD_ENVIA_STRING:        
        *p_pacote_envio_de_dados++ = cmd;
        *p_pacote_envio_de_dados++ = 11;
        for(int i=0; i < 10; ++i)
            *p_pacote_envio_de_dados++ = str_dado[i];

        *p_pacote_envio_de_dados++ = '\0';

        for(int i=0; i < 4; ++i)
            *p_pacote_envio_de_dados++ = MATRICULA[i];
        break;
    default:
        printf("Comando invalido\n");
        return -1;
    }
    size_t _size = (p_pacote_envio_de_dados - &pacote_envio_de_dados[0]);
    return envia_dados(uart_filestream, &pacote_envio_de_dados[0], _size);
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

int recebe_dado(int uart_filestream, int cmd){
    switch (cmd)
    {
    case CMD_SOLICITA_INT:
        int int_buffer;
        return recebe_dado(uart_filestream, &int_buffer);
    case CMD_SOLICITA_FLOAT:
        float float_buffer;
        return recebe_dado(uart_filestream, &float_buffer);
    case CMD_SOLICITA_STRING:
        char rx_buffer[256];
        return recebe_dado(uart_filestream, &rx_buffer[0]);
    default:
        printf("Comando invalido\n");
        return -1;
    }
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
