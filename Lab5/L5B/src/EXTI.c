#include "EXTI.h"

#include "DAC.h"

void EXTI_Init(void) {
    // [TODO] Configure EXTI for button
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
}

#define DAC_MIN 0
#define DAC_MAX 4095
#define DAC_INCREMENT 256

static uint32_t dac_value = 0;
static enum {
    DOWN,
    UP,
} direction = UP;

// [TODO] Interrupt handler
int LEDON = 0;

void EXTI15_10_IRQHandler(void) {
	if ((EXTI->PR1 & EXTI_PR1_PIF13) != 0) {
		// Clear interrupt pending bit
		EXTI->PR1 |= EXTI_PR1_PIF13; // clear pending bit of EXTI pending interrupt register	
		// Define behavior that occurs when interrupt occurs
		if (LEDON == 0) {
			LEDON = 1;
			
			for(int i = DAC_MIN; i <= DAC_MAX; i += DAC_INCREMENT)
			{
					uint32_t msTicks = 0;
					DAC_Write_Value((uint32_t)i);
				
					while (msTicks < 1000)
					{
						++msTicks;
					}
			}
		}
		
		else	{
			LEDON = 0;
			
			for(int i = DAC_MIN; i <= DAC_MAX; i += DAC_INCREMENT)
			{
					uint32_t msTicks = 0;
					DAC_Write_Value((uint32_t)i);
				
					while (msTicks < 1000)
					{
						++msTicks;
					}
			}
		}
	}
}