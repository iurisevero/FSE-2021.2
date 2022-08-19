#include "modbus.hpp"
#include "helpers.hpp"
#include "crc16.hpp"
#include "uart.hpp"
#include "consts.hpp"

ssize_t assembleProtocol(unsigned char * message, unsigned char cmd, bool isRequest/*=false*/){
    unsigned char * p_message = message;
    *p_message++ = ENDERECO;
    *p_message++ = (isRequest? CMD_SOLICITACAO : CMD_ENVIO);
    *p_message++ = cmd;
    return getSize(message, p_message);
}

ssize_t sendDataRequest(unsigned char cmd){
    unsigned char * message = (unsigned char *) malloc(5);
    ssize_t _size = assembleProtocol(message, cmd, true);
    unsigned short crc = calcula_CRC(message, _size);
    addCRC(crc, &message[_size]);
    _size += sizeof(crc);
    return Uart::sendData(message, _size);
}

ssize_t sendData(int value){
    unsigned char * message = (unsigned char *) malloc(13);
    ssize_t _size = assembleProtocol(message, CMD_ENVIA_INT);

    memcpy(&message[_size], &value, sizeof(value));
    _size += sizeof(value);

    addMatricula(&message[_size]);
    _size += MATRICULA_SIZE;

    unsigned short crc = calcula_CRC(message, _size);
    addCRC(crc, &message[_size]);
    _size += sizeof(crc);
    return Uart::sendData(message, _size);
}

ssize_t sendData(float value){
    unsigned char * message = (unsigned char *) malloc(13);
    ssize_t _size = assembleProtocol(message, CMD_ENVIA_FLOAT);

    memcpy(&message[_size], &value, sizeof(value));
    _size += sizeof(value);

    addMatricula(&message[_size]);
    _size += MATRICULA_SIZE;

    unsigned short crc = calcula_CRC(message, _size);
    addCRC(crc, &message[_size]);
    _size += sizeof(crc);
    return Uart::sendData(message, _size);
}

ssize_t sendData(char * value){
    ssize_t str_size = strlen(value);
    unsigned char *  message = (unsigned char *) malloc(str_size + 11);
    ssize_t _size = assembleProtocol(message, CMD_ENVIA_STRING);
    
    message[_size++] = str_size;
    memcpy(&message[_size], value, str_size);
    _size += str_size;

    addMatricula(&message[_size]);
    _size += MATRICULA_SIZE;

    unsigned short crc = calcula_CRC(message, _size);
    addCRC(crc, &message[_size]);
    _size += sizeof(crc);
    return Uart::sendData(message, _size);
}

ssize_t receiveData(int * value){
    unsigned char * message = (unsigned char *) malloc(9);
    ssize_t expectedSize = 9;
    ssize_t _size = receiveConstantSizeData(message, expectedSize, CMD_SOLICITA_INT);
    memcpy(&value, &message[3], sizeof(value));
    free(message);
    return _size;
}

ssize_t receiveData(float * value){
    unsigned char * message = (unsigned char *) malloc(9);
    ssize_t expectedSize = 9;
    ssize_t _size = receiveConstantSizeData(message, expectedSize, CMD_SOLICITA_FLOAT);
    memcpy(&value, &message[3], sizeof(value));
    free(message);
    return _size;
}

ssize_t receiveData(char * value){
    unsigned char * message = (unsigned char *) malloc(4);
    ssize_t expectedSize = 4;
    ssize_t _size = Uart::receiveData(message, expectedSize);
    if(_size != expectedSize){
        printf("Erro! Tamanho do dado recebido nao corresponde ao esperado\n");
        return -1;
    }

    if(!checkReceivedData(message, ENDERECO, CMD_SOLICITACAO, CMD_SOLICITA_STRING)){
        printf("Erro! Mensagem recebida não segue o padrao esperado.\n");
        return -2;
    }

    if(!checkErrorBit(message)){
        printf("Erro! Mensagem recebida com bit de erro settado.\n");
        return -3;
    }
    ssize_t strSize = message[3];
    message = (unsigned char *) realloc(message, strSize + _size);
    expectedSize = _size + strSize + 2;
    _size += Uart::receiveData(&message[_size], strSize + 2);

    if(_size != expectedSize){
        printf("Erro! Tamanho do dado recebido nao corresponde ao esperado\n");
        return -1;
    }

    if(!checkCRC(message, _size)){
        printf("Erro! CRC invalido.\n");
        return -4;
    }

    memcpy(&value, &message[5], strSize);
    value[strSize] = '\0';
    free(message);
    return strSize;
}

ssize_t receiveConstantSizeData(unsigned char * message, ssize_t expectedSize, unsigned char subCmd){
    ssize_t _size = Uart::receiveData(message, expectedSize);
    if(_size != expectedSize){
        printf("Erro! Tamanho do dado recebido nao corresponde ao esperado\n");
        return -1;
    }

    if(!checkReceivedData(message, ENDERECO, CMD_SOLICITACAO, subCmd)){
        printf("Erro! Mensagem recebida não segue o padrao esperado.\n");
        return -2;
    }

    if(!checkErrorBit(message)){
        printf("Erro! Mensagem recebida com bit de erro settado.\n");
        return -3;
    }

    if(!checkCRC(message, _size)){
        printf("Erro! CRC invalido.\n");
        return -4;
    }
    return _size;
}
