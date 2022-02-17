#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "uart.hpp"
#include "helpers.hpp"
#include "consts.hpp"
#include "crc16.hpp"

const unsigned char ENDERECO = 0x01;
const unsigned char CMD_SOLICITACAO = 0x23;
const unsigned char CMD_SOLICITA_INT = 0xA1;
const unsigned char CMD_SOLICITA_FLOAT = 0xA2;
const unsigned char CMD_SOLICITA_STRING = 0xA3;
const unsigned char CMD_ENVIO = 0x16;
const unsigned char CMD_ENVIA_INT = 0xB1;
const unsigned char CMD_ENVIA_FLOAT = 0xB2;
const unsigned char CMD_ENVIA_STRING = 0xB3;

unsigned char * monta_solicitacao(unsigned char * mensagem, unsigned char cmd, size_t * size);
bool verifica_crc(unsigned char * mensagem, size_t size);
bool verifica_bit_de_erro(unsigned char * mensagem);

int main(int argc, const char * argv[]) {
    // char uart_path[] = "/dev/serial0";
    // int uart_filestream = abrir_uart(uart_path);
    // close(uart_filestream);
    size_t size;
    unsigned char * mensagem = monta_solicitacao(mensagem, CMD_SOLICITA_INT, &size);
    printf("%d\n", size);
    bool a = verifica_crc(mensagem, size);
    printf("Bool: %d\n", a);
    a = verifica_bit_de_erro(mensagem);
    printf("Bool: %d\n", a);
    free(mensagem);
    return 0;
}


unsigned char * monta_solicitacao(unsigned char * mensagem, unsigned char cmd, size_t * size){
    mensagem = (unsigned char *) malloc(5);
    unsigned char * p_mensagem = mensagem;
    *p_mensagem++ = ENDERECO;
    *p_mensagem++ = CMD_SOLICITACAO;
    *p_mensagem++ = cmd;
    size_t _size = get_size(mensagem, p_mensagem);
    unsigned short crc = calcula_CRC(mensagem, _size);
    add_crc(crc, p_mensagem);
    *size = (_size + 2);
    return mensagem;
}

unsigned char * monta_envio(unsigned char * mensagem, int valor, size_t * size){
    mensagem = (unsigned char *) malloc(9);
    unsigned char * p_mensagem = mensagem;
    *p_mensagem++ = ENDERECO;
    *p_mensagem++ = CMD_ENVIO;
    *p_mensagem++ = CMD_ENVIA_INT;

    unsigned char int_bytes[4];
    int2Bytes(int_bytes, valor);
    for(int i=0; i < 4; ++i)
        *p_mensagem++ = int_bytes[i];

    size_t _size = get_size(mensagem, p_mensagem);
    unsigned short crc = calcula_CRC(mensagem, _size);
    add_crc(crc, p_mensagem);
    *size = (_size + 2);
    return mensagem;
}

unsigned char * monta_envio(unsigned char * mensagem, float valor, size_t * size){
    mensagem = (unsigned char *) malloc(9);
    unsigned char * p_mensagem = mensagem;
    *p_mensagem++ = ENDERECO;
    *p_mensagem++ = CMD_ENVIO;
    *p_mensagem++ = CMD_ENVIA_FLOAT;

    unsigned char float_bytes[4];
    float2Bytes(float_bytes, valor);
    for(int i=0; i < 4; ++i)
        *p_mensagem++ = float_bytes[i];

    size_t _size = get_size(mensagem, p_mensagem);
    unsigned short crc = calcula_CRC(mensagem, _size);
    add_crc(crc, p_mensagem);
    *size = (_size + 2);
    return mensagem;
}

unsigned char * monta_envio(unsigned char * mensagem, char * valor, size_t * size){
    size_t str_size = strlen(valor);
    mensagem = (unsigned char *) malloc(str_size + 7);
    unsigned char * p_mensagem = mensagem;
    *p_mensagem++ = ENDERECO;
    *p_mensagem++ = CMD_ENVIO;
    *p_mensagem++ = CMD_ENVIA_STRING;
    *p_mensagem++ = str_size;

    for(size_t i=0; i < str_size; ++i)
        *p_mensagem++ = (unsigned char) valor[i];
    *p_mensagem++ = '\0';

    size_t _size = get_size(mensagem, p_mensagem);
    unsigned short crc = calcula_CRC(mensagem, _size);
    add_crc(crc, p_mensagem);
    *size = (_size + 2);
    return mensagem;
}

bool verifica_crc(unsigned char * mensagem, size_t size){
    unsigned short crc_atual;
    memcpy(&crc_atual, &mensagem[size-2], sizeof(unsigned short));
    return crc_atual == calcula_CRC(mensagem, size-2);
}

bool verifica_bit_de_erro(unsigned char * mensagem){
    unsigned char cmd = mensagem[1];
    unsigned char mask = 0x80;
    return (cmd & mask) == 0x80;
}