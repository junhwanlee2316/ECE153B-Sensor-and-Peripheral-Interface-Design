<<<<<<< Updated upstream
#ifndef __STM32L476G_DISCOVERY_ULTSONIC_H
#define __STM32L476G_DISCOVERY_ULTSONIC_H

#include "stm32l476xx.h"


=======
/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Junhwan Lee, Taeho Kim
 * Section: Tuesday
 * Project
*/

#ifndef ULTSONIC_H
#define ULTSONIC_H

#include "stm32l476xx.h"

void Input_Capture_Setup(void);
void TIM5_IRQHandler(void);
void Trigger_Setup(void);
void Setup(void);
uint32_t getTimeInterval(void);
>>>>>>> Stashed changes
#endif