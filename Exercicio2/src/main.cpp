#include <stdio.h>
#include <string.h>
#include "uart.hpp"
#include "helpers.hpp"
#include "consts.hpp"

int main(int argc, const char * argv[]) {
    char uart_path[] = "/dev/serial0";
    int uart_filestream = abrir_uart(uart_path);
    close(uart_filestream);
    
    return 0;
}
