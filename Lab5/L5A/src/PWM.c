#include "PWM.h"

void PWM_Init(){
    // Enable GPIO Port A Clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;  // Enables clock A
	
	// Enable TIM2 Clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN; // Enables clock TIM2
	
	// Configure PA5
	GPIOA->MODER &= ~(GPIO_MODER_MODE5); // clear bits for PA5 mode
	GPIOA->MODER |= GPIO_MODER_MODE5_1;	// set mode of PA5 to alternative function
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5; //set PA5 to very high outout speed
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT5);    // set output type as push-pull
	GPIOA->PUPDR  &= ~(GPIO_PUPDR_PUPD5);   // set to no PUPD
	
	// Configure PWM Output for TIM2 CH 1
	GPIOA->AFR[0] = (GPIOA->AFR[0] & GPIO_AFRL_AFSEL5_0) | GPIO_AFRL_AFSEL5_0;

	// Set the direction such that the timer counts up.
	TIM2->CR1 &= ~(TIM_CR1_DIR);

	// Set the prescaler value.
	TIM2->PSC = 0x0003U;

	// Set the auto-reload value
	TIM2->ARR = 0x03E7U;
	TIM2->CCMR1 &= ~(TIM_CCMR1_OC1M); 	// Clear the output compare mode bits
	TIM2->CCMR1 |= TIM_CCMR1_OC1M_2;  	// Set the output compare mode bits to PWM mode 1.
	TIM2->CCMR1 |= TIM_CCMR1_OC1M_1; 
	TIM2->CCMR1 |= TIM_CCMR1_OC1PE; 	// enable output preload
	TIM2->CCER &= ~(TIM_CCER_CC1P); 	// set output polarity to high (bit 0)
	TIM2->CCER |= TIM_CCER_CC1E; 		// enable channel 1 output
	TIM2->CCR1 |= 500;
	TIM2->CR1 |= TIM_CR1_CEN;
	
}
