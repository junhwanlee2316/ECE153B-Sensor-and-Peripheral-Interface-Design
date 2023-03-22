/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Junhwan Lee, Taeho Kim
 * Section: Tuesday
 * Project
*/

#ifndef STEPPER_H
# define STEPPER_H

#include "stm32l476xx.h"

void GPIO_Init(void);
void Full_Stepping_Clockwise(int delay);
void Full_Stepping_CounterClockwise(void);
void initStepperMotor(void);
void stopMotor(void);

#endif