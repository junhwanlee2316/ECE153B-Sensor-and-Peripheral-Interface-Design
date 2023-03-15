#ifndef NUNCHUCK_H
# define NUNCHUCK_H

#include "stm32l476xx.h"

#include <stdint.h>
#include <stdbool.h>
#include <cstddef>

void initChuk(void);
uint8_t readChuk(void);
int getChukDir(const uint8_t* chukBuf);

#endif