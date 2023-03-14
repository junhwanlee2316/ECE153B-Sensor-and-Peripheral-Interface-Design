#include "SysClock.h"

void System_Clock_Init(void) {
	// Select MSI as system clock source
	RCC->CFGR &= ~RCC_CFGR_SW; // [TODO] - Replace with value that will make MSI the system clock source
	RCC->CFGR |= RCC_CFGR_SW_MSI;
	
	// Set MSI clock range
	RCC->CR &= ~RCC_CR_MSIRANGE;
	RCC->CR |= RCC_CR_MSIRANGE_7; // [TODO] - Replace with value that will make range 8 MHz
	
	// Use the MSI clock range that is defined in RCC_CR
	RCC->CR |= RCC_CR_MSIRGSEL; // [TODO] - Replace with value that will select range in RCC->CR
	
	// Enable MSI oscillator
	RCC->CR |= RCC_CR_MSIPLLEN; // [TODO] - Replace with value that will enable MSI
	
	// Wait until MSI is ready
	while((RCC->CR & RCC_CR_MSIRDY /* [TODO] - Replace with value that checks whether MSI is ready */) == 0);
}
