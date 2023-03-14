/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Junhwan Lee, Taeho Kim
 * Section: Tuesday 7pm
 * Lab: 1A
 */
#include "stm32l476xx.h"
#define RCC_AHB2ENR_GPIOAEN ((uint32_t)0x00000001U)
#define RCC_AHB2ENR_GPIOCEN ((uint32_t)0x00000004U)

void Init(){
    // Enable HSI
    RCC->CR |= ((uint32_t)RCC_CR_HSION);
    while ( (RCC->CR & (uint32_t) RCC_CR_HSIRDY) == 0 );

    // Select HSI as system clock source
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSI;
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) == 0 );
		
		
		
    // Enable GPIO Clock
    // [TODO]
	  // Enable Clock for port A and C
		RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOCEN);
		
    // Initialize Green LD
    // [TODO]
		// set pin 5 to output
		GPIOA->MODER &= ~(3UL<<10); // Clear bits for Pin 5
		GPIOA->MODER |= 1UL<<10; // Set bit 4, set Pin 5 as output
	
		GPIOA->OTYPER &= ~(1UL<<5); // Clear bit 2
		GPIOA->OTYPER &= ~(1UL<<5); // Select push-pull output
		
		GPIOA->PUPDR &= ~(3UL<<10); // Set no pull-up, no pull-down

    // Initialize User Button
    // [TODO]
		// set pin 13 to input
		GPIOC->MODER &= ~(3UL << 26);	// Mode register pin 13
		GPIOC->PUPDR &= ~(3UL << 26);
		
}

int main(void){
    // Initialization
    Init();
		
	  // boolean variable to determine led state
		int ledState = 0; // initially set to off
	
    // Polling to Check for User Button Presses
    // [TODO]
		while(1){
			
			uint32_t mask = 1UL<<13;	// reading pin 13
			uint32_t input = (GPIOC->IDR & mask) == mask; // read input
			uint32_t mask1 = 1UL<<5; // reading pin 5
			
			// if prev.state and button is pressed
			if(input == 0 && !ledState){
				GPIOA -> ODR ^= mask1;	// toggle green led
				ledState = 1;	// set ledState to 1
			}
			
			// if button is not pressed(released)
			if(input == 1){
				ledState = 0;	// set ledState to 0
			}
		}
		
    return 0; // End of program
}
