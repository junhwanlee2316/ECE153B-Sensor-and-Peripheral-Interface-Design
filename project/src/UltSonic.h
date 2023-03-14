#ifndef ULTSONIC_H
#define ULTSONIC_H

#include "stm32l476xx.h"

void Input_Capture_Setup(void);
void TIM4_IRQHandler(void);
void Trigger_Setup(void);
void Setup(void);
uint32_t getTimeInterval(void);
#endif 
