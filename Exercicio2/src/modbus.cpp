#include "modbus.hpp"
#include "helpers.hpp"
#include "crc16.hpp"
#include "consts.hpp"

size_t monta_solicitacao(unsigned char * message, unsigned char cmd){
    // message = (unsigned char *) malloc(5);
    unsigned char * p_message = message;
    *p_message++ = ENDERECO;
    *p_message++ = CMD_SOLICITACAO;
    *p_message++ = cmd;
    size_t _size = getSize(message, p_message);
    unsigned short crc = calcula_CRC(message, _size);
    addCRC(crc, p_message);
    return (_size + 2);
}

size_t monta_envio(unsigned char * message, int value){
    // message = (unsigned char *) malloc(9);
    unsigned char * p_message = message;
    *p_message++ = ENDERECO;
    *p_message++ = CMD_ENVIO;
    *p_message++ = CMD_ENVIA_INT;

    unsigned char int_bytes[4];
    int2Bytes(int_bytes, &value);
    for(int i=0; i < 4; ++i)
        *p_message++ = int_bytes[i];

    size_t _size = getSize(message, p_message);
    unsigned short crc = calcula_CRC(message, _size);
    addCRC(crc, p_message);
    return (_size + 2);
}

size_t monta_envio(unsigned char * message, float value){
    // message = (unsigned char *) malloc(9);
    unsigned char * p_message = message;
    *p_message++ = ENDERECO;
    *p_message++ = CMD_ENVIO;
    *p_message++ = CMD_ENVIA_FLOAT;

    unsigned char float_bytes[4];
    float2Bytes(float_bytes, &value);
    for(int i=0; i < 4; ++i)
        *p_message++ = float_bytes[i];

    size_t _size = getSize(message, p_message);
    unsigned short crc = calcula_CRC(message, _size);
    addCRC(crc, p_message);
    return (_size + 2);
}

size_t monta_envio(unsigned char * message, char * value){
    size_t str_size = strlen(value);
    // message = (unsigned char *) malloc(str_size + 7);
    unsigned char * p_message = message;
    *p_message++ = ENDERECO;
    *p_message++ = CMD_ENVIO;
    *p_message++ = CMD_ENVIA_STRING;
    *p_message++ = str_size;

    for(size_t i=0; i < str_size; ++i)
        *p_message++ = (unsigned char) value[i];
    *p_message++ = '\0';

    size_t _size = getSize(message, p_message);
    unsigned short crc = calcula_CRC(message, _size);
    addCRC(crc, p_message);
    return (_size + 2);
}
