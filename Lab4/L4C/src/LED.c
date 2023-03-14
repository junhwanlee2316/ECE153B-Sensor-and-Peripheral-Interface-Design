/*
 * ECE 153B - Winter 2021
 *
 * Name(s): Taeho Kim, Junhwan Lee
 * Section: Tuesday 7pm
 * Lab: 4C
 */

#include "LED.h"

void LED_Init(void) {
	// TODO
	
	// Enable GPIO Clocks
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; 
	
	GPIOA->MODER &= ~(3UL<<10); 	// Clear bits for Pin 5
	GPIOA->MODER |= 1UL<<10; 		// Set bit 4, set Pin 5 as output
	
	GPIOA->OTYPER &= ~(1UL<<5); 	// Clear bit 2
	GPIOA->OTYPER &= ~(1UL<<5); 	// Select push-pull output
		
	GPIOA->PUPDR &= ~(3UL<<10); 	// Set no pull-up, no pull-down
}

void LED_Toggle(void) {
	GPIOA->ODR ^= (1UL << 5);
}

