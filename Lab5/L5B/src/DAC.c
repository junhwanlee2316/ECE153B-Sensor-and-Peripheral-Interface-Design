#include "stm32l476xx.h"

static void DAC_Pin_Init(void) {
    // [TODO]
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;	// port A clock enable
		GPIOA->MODER |= GPIO_MODER_MODE4;	// setting pin4 to analog mode (11)
		GPIOA->PUPDR &= GPIO_PUPDR_PUPD4;	// setting pin4 to no pull up, down mode (00)
}

void DAC_Write_Value(uint32_t value) {
    // [TODO] Write DAC1Ch1 right-aligned 12-bit value
		DAC->DHR12R1 = value;
}

void DAC_Init(void) {
    DAC_Pin_Init();

    // [TODO] Enable DAC clock
		RCC->APB1ENR1 |= RCC_APB1ENR1_DAC1EN;
	
		// Disable DAC channel 1 before modyfing DAC setting
		DAC->CR &= ~(DAC_CR_EN1);
		
    // [TODO] Enable software trigger mode
		DAC->SWTRIGR |= DAC_SWTRIGR_SWTRIG1;

    // [TODO] Disable trigger
		DAC->SWTRIGR &= ~(DAC_SWTRIGR_SWTRIG1);

    // [TODO] DAC1 connected in normal mode to external pin only with buffer enabled
		DAC->MCR &= ~(DAC_MCR_MODE1);

    // [TODO] Enable DAC channel 1
		DAC->CR |= DAC_CR_EN1;

    DAC_Write_Value(0);
}
