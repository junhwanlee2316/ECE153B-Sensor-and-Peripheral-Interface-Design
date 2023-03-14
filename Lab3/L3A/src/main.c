/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Junhwan Lee, Taeho Kim
 * Section: Tuesday
 * Lab: 3A
 */

#include "stm32l476xx.h"

const unsigned long preset_PSC = 10;
const unsigned long preset_ARR = 1999;
unsigned long preset_CCR = 1000;
 
void PWM_Init() {
	// Enable the Clock of GPIO Port A in RCC
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;	
	
	// Enable the Clock of Timer 2 in RCC
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN; 	
	
	// Configure PA5
	GPIOA->MODER &= ~GPIO_MODER_MODE5;		
	GPIOA->MODER |= 2UL << 10;				// Configure PA5 to Alternative Function Mode
	GPIOA->OSPEEDR &= ~(3UL << 10);				
	GPIOA->OSPEEDR |=  (3UL << 10);			// Configure PA5 to Very High Output Speed
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5;		// Configure PA5 to No Pull-Up, No Pull-Down	
	
	// Configure PWM Output for TIM2 CH 1
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL5;		// Configure and Select the Alternative Function for PA5
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL5_0; 	// Timer 2 Channel 1

	// Configure the PWM output for Timer 2 Channel 1

	// Set upcounting
	TIM2->CR1 &= ~TIM_CR1_DIR;				

	// Set the prescaler value.
	TIM2->PSC &= ~TIM_PSC_PSC; 						
	TIM2->PSC |= preset_PSC;

	// Set the auto-reload value.
	// 4MHz clock, period of 10ms, solve 0.01=2*(1+ARR)*(4*10^6 / 10), ARR = 1999
	TIM2->ARR &= ~TIM_ARR_ARR;
	TIM2->ARR |= preset_ARR;	

	// Configure the channel to be used in output compare mode.
	TIM2->CCMR1 &= ~TIM_CCMR1_OC1M;		// Clear the output compare mode bits.
	TIM2->CCMR1 |= TIM_CCMR1_OC1M_1;	// Set the output compare mode bits to PWM mode 1.
	TIM2->CCMR1 |= TIM_CCMR1_OC1M_2;	
	TIM2->CCMR1 |= TIM_CCMR1_OC1PE;		// Enable output preload.

	TIM2->CCER &= ~TIM_CCER_CC1P;		// Set the output polarity for compare 1 to active high.
	
	TIM2->CCER |= TIM_CCER_CC1E;		// Enable the channel 1 output.

	// Set the capture/compare value.  duty cycle of the PWM output is 50%. 
	TIM2->CCR1 &= ~TIM_CCR1_CCR1;		// Reset CCR1 Register
	TIM2->CCR1 |= preset_CCR;			// Set CCR to make 50% duty cycle

	// Enable the counter.
	TIM2->CR1 |= TIM_CR1_CEN;
}
 
int main() {
	// Initialization - We will use the default 4 MHz clock
	PWM_Init();
	
	// Periodic Dimming
	int i;
	while(1) {
		// green LED change its brightness periodically
		if (preset_CCR) {
			preset_CCR--; 				// Decrease brigtness
		}
		else {
			preset_CCR = preset_ARR+1; 	// Reset brightness
		}
		
		TIM2->CCR1 = preset_CCR;		// Set the duty cycle

		for(i=0; i<1000; ++i); 			// Some Delay
	}
	
	return 0;
}
