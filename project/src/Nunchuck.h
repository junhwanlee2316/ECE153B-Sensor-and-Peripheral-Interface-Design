/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Junhwan Lee, Taeho Kim
 * Section: Tuesday
 * Project
*/

#ifndef NUNCHUCK_H
# define NUNCHUCK_H

#include "stm32l476xx.h"

#include <stdint.h>
#include <stdbool.h>


void initChuk(void);
int readChuk(void);

#endif