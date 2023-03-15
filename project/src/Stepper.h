#ifndef STEPPER_H
# define STEPPER_H

#include "stm32l476xx.h"

void GPIO_Init(void);
void Full_Stepping_Clockwise(void);
void Full_Stepping_CounterClockwise(void);
void Half_Stepping_Clockwise(void);
void Half_Stepping_CounterClockwise(void);
void initStepperMotor();
void stopMotor();

#endif