/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Taeho Kim, Junhwan Lee
 * Section: Tuesday 7pm
 * Lab: 6B
 */

#include "stm32l476xx.h"

void SERVO_Pin_Init(){
	// [TODO]
	
	// Enable the clock of GPIO Port A
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;	
	
	// Configure PA 0 (Servo Motor) as Alternate Function Mode
	GPIOA->MODER &= ~(GPIO_MODER_MODE0);	
	GPIOA->MODER |= GPIO_MODER_MODE0_1;
	
	// Set alternate function of Pin 0 to TIM5 CH1
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL0);	
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL0_1;	// 0010 (AF2)
	
	// Configure PA 0 to No Pull-Up, No Pull-Down
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD0);
}

void TIM5_CH1_Init(){
	// [TODO]
	
	// Enable clock for TIM5
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM5EN;
	
	// Set TIM5->CR1 for the counting direction to be up
	TIM5->CR1 &= ~(TIM_CR1_DIR);
	
	// Set the prescaler TIM5->PSC to count at 100kHz (PSC = 39)
	TIM5->PSC &= ~(TIM_PSC_PSC);
	TIM5->PSC = 39;
	
	// Set the TIM5->ARR register to a value that will give a period of 0.02s (50Hz)
	TIM5->ARR &= ~(TIM_ARR_ARR);
	TIM5->ARR = 1999;
	
	// Clear the OC1M field in the TIM5->CCMR1 register and select PWM Mode 1 (OC1M = 0110)
	TIM5->CCMR1 &= ~(TIM_CCMR1_OC1M);
	TIM5->CCMR1 |= (TIM_CCMR1_OC1M_2 + TIM_CCMR1_OC1M_1);
	
	// Enable the Output 1 preload enable in TIM5->CCMR1
	TIM5->CCMR1 |= TIM_CCMR1_OC1PE;
	
	// Enable output of Channel 1 (CH1) by setting the CC1E bit in TIM5->CCER
	TIM5->CCER |= TIM_CCER_CC1E; 
	
	// Set the output compare register for channel 1 TIM5->CCR1 to have a duty cycle of 50% (CCR = half of ARR)
	TIM5->CCR1 &= ~(TIM_CCR1_CCR1);
	TIM5->CCR1 = 1000;
	
	// Enable counter by setting CEN bit in TIM5->CR1 
	TIM5->CR1 |= TIM_CR1_CEN;
}

void Servo_Move_Left_90_Degree(){
	// function to move the servo to +90 degrees
	// [TODO]
	
	// pulse width = 1ms 
	// Duty cycle = CRR/(ARR + 1)
	// period = 20ms
	// duty cycle = 1ms/.02 = 0.05 = 5%
	// CCR = .05 * 2000 = 100
	TIM5->CCR1 = 50;
}

void Servo_Move_Center(){
	// function to move the servo to 0 degrees
	// [TODO] 
	
	//	pulse width = 1.5ms 
	//	Duty cycle = CRR/(ARR + 1)
	//	period = 20ms
	//	duty cycle = 1.5ms/.02 = 0.075 = 7.5%
	//	CCR = .075 * 2000 = 150
	TIM5->CCR1 = 150;
}

void Servo_Move_Right_90_Degree(){
	// function to move the servo to -90 degrees
	// [TODO]

	//	pulse width = 2ms 
	//	Duty cycle = CRR/(ARR + 1)
	//	period = 20ms
	//	duty cycle = 2ms/.02 = 0.1 = 10%
	//	CCR = 0.1 * 2000 = 200
	TIM5->CCR1 = 200;
}


int main() {
		int i;
	
		SERVO_Pin_Init();
		TIM5_CH1_Init();
		
    while(1) {
			// Move servo to +90 degrees, 0 degrees, and -90 degrees
			// [TODO]
			Servo_Move_Left_90_Degree();
			for(i=0;i<100000;i++); 		// delay
			Servo_Move_Center();
			for(i=0;i<100000;i++);
			Servo_Move_Right_90_Degree();
			for(i=0;i<100000;i++);
    }
}