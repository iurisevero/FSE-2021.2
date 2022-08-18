#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "uart.hpp"
#include "helpers.hpp"
#include "consts.hpp"
#include "crc16.hpp"
#include "modbus.hpp"
#include "consts.hpp"

int main(int argc, const char * argv[]) {
    // char uartPath[] = "/dev/serial0";
    // int uartFilestream = openUart(uartPath);
    // close(uartFilestream);
    unsigned char * message;

    message = (unsigned char *) malloc(5);
    size_t size = monta_solicitacao(message, CMD_SOLICITA_INT);
    printf("size: %ld\n", size);
    printArrHex(message, size);
    bool a = checkCRC(message, size);
    printf("Bool: %d\n", a);
    a = checkErrorBit(message);
    printf("Bool: %d\n", a);
    free(message);

    message = (unsigned char *) malloc(9);
    size = monta_envio(message, 7);
    printf("size: %ld\n", size);
    printArrHex(message, size);
    a = checkCRC(message, size);
    printf("Bool: %d\n", a);
    a = checkErrorBit(message);
    printf("Bool: %d\n", a);
    free(message);

    message = (unsigned char *) malloc(9);
    size = monta_envio(message, 7.0f);
    printf("size: %ld\n", size);
    printArrHex(message, size);
    a = checkCRC(message, size);
    printf("Bool: %d\n", a);
    a = checkErrorBit(message);
    printf("Bool: %d\n", a);
    free(message);

    message = (unsigned char *) malloc(12);
    size = monta_envio(message, "Hello");
    printf("size: %ld\n", size);
    printArrHex(message, size);
    a = checkCRC(message, size);
    printf("Bool: %d\n", a);
    a = checkErrorBit(message);
    printf("Bool: %d\n", a);
    free(message);
    return 0;
}


