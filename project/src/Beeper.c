#include "Beeper.h"
/*Steps
1. initialize beeper
2. change main so that when the distance becomes too close, beeper makes the sound
3. input: distance, output: high
	 if (distance = something) then output = 1 in main
*/

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