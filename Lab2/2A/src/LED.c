/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Junhwan Lee, Taeho Kim
 * Section: Tuesday 7pm
 * Lab: 2A
 */

#include "LED.h"

int LEDSTATE = 1; // Variable which indicates the state of LED

void LED_Init(void) {
	// Enable GPIO Clocks
	// [TODO]
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOCEN | RCC_AHB2ENR_GPIOAEN);	// Enable GPIO Port C and A
	
	
	// Initialize Green LED
	// set pin 5 to output
	GPIOA->MODER &= ~(3UL<<10); // Clear bits for Pin 5
	GPIOA->MODER |= 1UL<<10; 	// Set bit 4, set Pin 5 as output
	
	GPIOA->OTYPER &= ~(1UL<<5); // Clear bit 2
	GPIOA->OTYPER &= ~(1UL<<5); // Select push-pull output
		
	GPIOA->PUPDR &= ~(3UL<<10); // Set no pull-up, no pull-down
}

void Green_LED_Off(void) {
	GPIOA->ODR &= ~GPIO_ODR_OD5; 	// Write 0 to ODR bit 5
}

void Green_LED_On(void) {
	GPIOA->ODR |= 1UL<<5; 		// Write 1 to ODR bit 5
}

void Green_LED_Toggle(void) {
	// if LED is ON
	if (LEDSTATE == 1) {
		LEDSTATE = 0; 			// LEDSTATE = 0
		Green_LED_Off();		// Turn off LED
	}
	else {
		LEDSTATE = 1;			// LEDSTATE = 1
		Green_LED_On();			// Tunr on LED
	}
}
