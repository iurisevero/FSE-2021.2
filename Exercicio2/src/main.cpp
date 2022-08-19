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
    char uartPath[] = "/dev/serial0";
    int uartFilestream = Uart::openUart(uartPath);

    ssize_t rx_length;

    sendData(7);
    sleep(1);
    sendDataRequest(CMD_SOLICITA_INT);
    sleep(1);
    int receivedInt;
    rx_length = receiveData(&receivedInt);
    printf("%li Bytes lidos: %d\n", rx_length, receivedInt);
    sleep(1);

    sendData(7.77f);
    sleep(1);
    sendDataRequest(CMD_SOLICITA_INT);
    sleep(1);
    float receivedFloat;
    rx_length = receiveData(&receivedFloat);
    printf("%li Bytes lidos: %f\n", rx_length, receivedFloat);
    sleep(1);
    
    sendData("Sete");
    sleep(1);
    sendDataRequest(CMD_SOLICITA_INT);
    sleep(1);
    char receivedString[256];
    rx_length = receiveData(receivedString);
    printf("%li Bytes lidos: %s\n", rx_length, receivedString);

    close(uartFilestream);
    return 0;
}


