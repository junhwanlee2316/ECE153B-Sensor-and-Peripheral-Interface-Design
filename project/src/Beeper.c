#include "Beeper.h"

void Beeper_Init(){
		// enable clock for port B
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
		
		// configure PB5 as output
		GPIOB->MODER &= ~(GPIO_MODER_MODE5);
		GPIOB->MODER |= GPIO_MODER_MODE5_0;
	
		// configure PB5 as push-pull output type
		GPIOB->OTYPER &= ~(GPIO_OTYPER_OT5);
	
		// configure PB5 as no pupd
		GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD5);
}                  
