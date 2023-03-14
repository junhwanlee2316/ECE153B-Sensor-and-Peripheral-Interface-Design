/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Junhwan Lee, Taeho Kim
 * Section: Tuesday 7pm
 * Lab: 2C
 */

#ifndef __STM32L476G_NUCLEO_SYSTICK_H
#define __STM32L476G_NUCLEO_SYSTICK_H

#include "stm32l476xx.h"

void SysTick_Init(void);
void SysTick_Handler(void);

void delay(uint32_t T);

#endif
