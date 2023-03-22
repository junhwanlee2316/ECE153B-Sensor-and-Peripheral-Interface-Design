#ifndef SERVO_H
# define SERVO_H

#include "stm32l476xx.h"

void SERVO_Pin_Init(void);
void TIM5_CH1_Init(void);
void Servo_Move_Left_90_Degree(void);
void Servo_Move_Center(void);
void Servo_Move_Right_90_Degree(void);
void initServo(void);
#endif