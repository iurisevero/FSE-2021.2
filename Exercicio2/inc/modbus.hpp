#ifndef MODBUS_HPP
#define MODBUS_HPP

#include <stdlib.h>

size_t monta_solicitacao(unsigned char * message, unsigned char cmd);
size_t monta_envio(unsigned char * message, int value);
size_t monta_envio(unsigned char * message, float value);
size_t monta_envio(unsigned char * message, char * value);

#endif