/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Junhwan Lee, Taeho Kim
 * Section: Tuesday 7pm
 * Lab: 2A
 */


#include "EXTI.h"
#include "LED.h"

void EXTI_Init(void) {
	// Initialize User Button
	// [TODO]

	// User Button: pin 13 port C
	GPIOC->MODER &= ~(3UL << 26);					// Mode set to Input (00)
	GPIOC->PUPDR &= ~(3UL << 26);					// Reset pull up/down (10)
	GPIOC->PUPDR |= 2UL << 26;						// Set to Pull-Down (10)

	// Configure SYSCFG EXTI
	// [TODO]
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; 			// Enable SYSCFGR clock
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13; 	// Clear exticr for pin 13
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;	// Enable the syscfg external interrupt config R for line 4

	// Configure EXTI Trigger
	EXTI->FTSR1 |= EXTI_FTSR1_FT13; 				// Enable interrupt on falling edge for EXTI line 13
	
	// Enable EXTI
	EXTI->IMR1 |= EXTI_IMR1_IM13; 					// Set the interrupt mask register for EXTI line 13
	
	// Configure and Enable in NVIC
	NVIC_EnableIRQ(EXTI15_10_IRQn); 				// Enable interrupt for EXTI lines 15-10
	NVIC_SetPriority(EXTI15_10_IRQn, 0);
	
	Green_LED_Toggle();
}

// [TODO] Write Interrupt Handlers (look in startup_stm32l476xx.s to find the 
// interrupt handler names that you should use)

void EXTI15_10_IRQHandler(void) {
	// Clear interrupt pending bit
	if ((EXTI->PR1 & EXTI_PR1_PIF13) != 0) {
		EXTI->PR1 |= EXTI_PR1_PIF13; 		// clear pending bit of EXTI pending interrupt register	
		Green_LED_Toggle();					//Green_LED_Off();
	}
}