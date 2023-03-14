/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Junhwan Lee, Taeho Kim
 * Section: Tuesday
 * Project
 */


#include "UltSonic.h"
#include <stdio.h> 

uint32_t volatile currentValue = 0;
uint32_t volatile lastValue = 0;
uint32_t volatile overflowCount = 0;
uint32_t volatile timeInterval = 0;
uint32_t volatile dist_cm = 0;


void Input_Capture_Setup() {

	// 1. Set up PB6

	// Enable GPIO Poart B Clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;	

	// Configure PB6 to alternative function TIM4_CH1
	GPIOB->MODER &= ~GPIO_MODER_MODE6; 
	GPIOB->MODER |= GPIO_MODER_MODE6_1; 
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL6;		// TIM4_CH1 is on AF2 for PB6 uses AFR[0]
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6_1; 	
	
	// Set PB6 to no pull-up, no pull-down.
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR6; 
	
	// 2. Enable Timer 4 in RCC APB1ENRx.
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;
	
	// 3. Set the prescaler to 15
	TIM4->PSC &= ~TIM_PSC_PSC; 				
	TIM4->PSC = 15; 						// 16Mhz / (1+15) = 1MHz
	
	// 4. Enable auto reload preload in the control register and set the auto reload value to its maximum value.
	TIM4->CR1 |= TIM_CR1_ARPE;
	TIM4->ARR = TIM_ARR_ARR;				// Set the auto reload value to its maximum value
	
	// 5. In the capture/compare mode register, set the input capture mode bits such that the input capture is mapped to timer input 1.
	TIM4->CCMR1 &= ~TIM_CCMR1_CC1S; 		
	TIM4->CCMR1 |= TIM_CCMR1_CC1S_0; 		// set bit one so input capture bit mapped to timer input 1
	
	// 6. In the capture/compare enable register, set bits to capture both rising/falling edges and enable capturing.
	TIM4->CCER |= (TIM_CCER_CC1NP | TIM_CCER_CC1P);
	TIM4->CCER |= TIM_CCER_CC1E;
	
	// 7. In the DMA/Interrupt enable register, enable both interrupt and DMA requests. In addition, enable the update interrupt.
	TIM4->DIER |= (TIM_DIER_CC1IE | TIM_DIER_UDE);
	TIM4->DIER |= TIM_DIER_UIE;

	// 8. Enable update generation in the event generation register.
	TIM4->EGR |= TIM_EGR_UG;
	
	// 9. Clear the update interrupt flag.
	TIM4->SR &= ~TIM_SR_UIF;
	
	// 10. Set the direction of the counter and enable the counter in the control register.
	// DIR = 0 => countup
	TIM4->CR1 &= ~TIM_CR1_DIR; 			
	TIM4->CR1 |= TIM_CR1_CEN;
	
	// 11. Enable the interrupt (TIM4 IRQn) in the NVIC and set its priority to 2.
	NVIC_EnableIRQ(TIM4_IRQn);
	NVIC_SetPriority(TIM4_IRQn, 2);
}

void TIM4_IRQHandler(void) {
	// Check if overflow

	// Overflow
	if (TIM4->SR & TIM_SR_UIF) {
		// Clear the update interrupt flag
		TIM4->SR &= ~TIM_SR_UIF;
		overflowCount++;
	}
	
	// Check if input capture flag is triggered
	if (TIM4->SR & TIM_SR_CC1IF) {

		// Clear the interrupt flag
		TIM4->SR &= ~TIM_SR_CC1IF;

		// Rising edge
  		if (GPIOB->IDR & GPIO_IDR_ID6) { 
			lastValue = TIM4->CCR1; 		// find current CCR
		}

		// Falling edge
		else {
			currentValue = TIM4->CCR1; 		// Current CCR
			timeInterval = currentValue + (overflowCount * TIM_CCR1_CCR1) - lastValue;
			overflowCount = 0; 
		}
	}
}

void Trigger_Setup() {

	// 1. Set up PA9
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;	// Enable the clock for GPIO Port A

	// Configure PA9 to be used as alternative function TIM1 CH2.
	GPIOA->MODER &= ~GPIO_MODER_MODE9;		
	GPIOA->MODER |= 2UL << 18;						
	GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL9;		
	GPIOA->AFR[1] |= GPIO_AFRH_AFSEL9_0; 	
	
	// Set PA9 to no pull-up, no pull-down.
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD9;	
	
	// Set the output type of PA9 to push-pull.
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT9; // bit = 0 --> push pull
	
	// Set PA9 to very high output speed
	GPIOA->OSPEEDR &= ~(3UL << 18);				
	GPIOA->OSPEEDR |=  (3UL << 18);				
	
	// 2. Enable Timer 1 in RCC APB2ENR
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	
	// 3. Set the prescaler to 15
	TIM1->PSC &= ~TIM_PSC_PSC; 
	TIM1->PSC = 15; 					// 16Mhz / (1+15) = 1MHz
	
	// 4. Enable auto reload preload in the control register and set the auto reload value to its maximum value.
	TIM1->CR1 |= TIM_CR1_ARPE;
	TIM1->ARR = TIM_ARR_ARR;
	TIM1->ARR = 31;						// without this the HCSR04 has outputs of ~76ms on no object
	
	// 5. Set the CCR value that will trigger the sensor.
	// PWM signal that will trigger the ultrasonic sensor
	// Pulse Width = PW = (CCR-1) * (clock period) = 10us
	// CCR = PW / (clock period) + 1 = (10us / 1 MHz) + 1 = 10 + 1 = 11
	TIM1->CCR2 &= ~TIM_CCR2_CCR2; // reset CCR
	TIM1->CCR2 = 11;

	// 6. Set output compare mode to PWM mode 1 and enable output compare preload.
  TIM1->CCMR1 &= ~TIM_CCMR1_OC2M; 
  TIM1->CCMR1 |= (TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2PE); // PWM Mode 1 by setting bits 1,2 (0110)

  // 7. Enable the output in the capture/compare enable register
  TIM1->CCER |= TIM_CCER_CC2E;
    
  // 8. Set the bits for main output enable and set off-state selection for run mode
  TIM1->BDTR |= (TIM_BDTR_MOE | TIM_BDTR_OSSR);
    
  // 9. Enable update generation in the event generation register.
  TIM1->EGR &= ~TIM_EGR_UG;
  TIM1->EGR |= TIM_EGR_UG;
    
  // 10. Enable update interrupt.
  TIM1->DIER |= TIM_DIER_UIE;
    
  // Clear the update interrupt flag.
  TIM1->SR &= ~TIM_SR_UIF;
    
  // 11. Set the direction of the counter to count down
	// DIR = 1 => countsdown
  TIM1->CR1 |= TIM_CR1_DIR;
  TIM1->CR1 |= TIM_CR1_CEN;
}

void Setup(){
	Input_Capture_Setup();
	Trigger_Setup();
}

uint32_t getTimeInterval(){
	return timeInterval;
}