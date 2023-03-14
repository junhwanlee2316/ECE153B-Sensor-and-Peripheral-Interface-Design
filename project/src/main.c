/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Junhwan Lee, Taeho Kim
 * Section: Tuesday
 * Project
 */
 

#include "stm32l476xx.h"
#include "UltSonic.h"

uint32_t volatile timeInterval = 0;
uint32_t volatile dist_cm = 0;



int main(void) {	
	// Enable High Speed Internal Clock (HSI = 16 MHz)
	RCC->CR |= RCC_CR_HSION;
	while ((RCC->CR & RCC_CR_HSIRDY) == 0); // Wait until HSI is ready
	
	// Select HSI as system clock source 
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_HSI;
	while ((RCC->CFGR & RCC_CFGR_SWS) == 0); // Wait until HSI is system clock source
  
	Setup();

	while(1) {
		timeInterval = getTimeInterval();
		// Timer clock = 1MHz = 1us
		// Distance in cm = pulse width(us) / 58
		int t = timeInterval;
		if (t/1000 < 38) {
			dist_cm = t/58;
		}
	}
}