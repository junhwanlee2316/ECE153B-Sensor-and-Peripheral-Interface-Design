#include "ADC.h"

#include "stm32l476xx.h"

#include <stdint.h>

void ADC_Wakeup(void) {
    int wait_time;

    // To start ADC operations, the following sequence should be applied
    // DEEPPWD = 0: ADC not in deep-power down
    // DEEPPWD = 1: ADC in deep-power-down (default reset state)
    if ((ADC1->CR & ADC_CR_DEEPPWD) == ADC_CR_DEEPPWD)
        ADC1->CR &= ~ADC_CR_DEEPPWD; // Exit deep power down mode if still in that state

    // Enable the ADC internal voltage regulator
    // Before performing any operation such as launching a calibration or enabling the ADC, the ADC
    // voltage regulator must first be enabled and the software must wait for the regulator start-up
    // time.
    ADC1->CR |= ADC_CR_ADVREGEN;

    // Wait for ADC voltage regulator start-up time
    // The software must wait for the startup time of the ADC voltage regulator (T_ADCVREG_STUP)
    // before launching a calibration or enabling the ADC.
    // T_ADCVREG_STUP = 20 us
    wait_time = 20 * (80000000 / 1000000);
    while (wait_time != 0) {
        wait_time--;
    }
}

void ADC_Common_Configuration() {

    // Enable the I/O analog switch voltage booster.
    SYSCFG->CFGR1 |= SYSCFG_CFGR1_BOOSTEN;

    // Enable V_REFINT.
    ADC123_COMMON->CCR |= ADC_CCR_VREFEN;

    // Ensure that the clock is not divided.
    ADC123_COMMON->CCR &= ~(ADC_CCR_PRESC); 

    // Ensure that the ADC clock scheme is set to HCLK/1 synchronous clock mode.
	ADC123_COMMON->CCR |= ADC_CCR_CKMODE_0; 

    //Ensure that all ADCs are operating in independent mode.
    ADC123_COMMON->CCR &= ~(ADC_CCR_DUAL);
}


void ADC_Pin_Init(void) {
    
    // Enable GPIO A Clock.
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    // Set PA1 to Analog mode.
    GPIOA->MODER  &= GPIO_MODER_MODE1;
    GPIOA->MODER |= GPIO_MODER_MODE1;

    // Set PA1 to push pull output mode
    GPIOA->OTYPER &= ~(GPIO_OTYPER_OT1);
    
    // Set PA1 to no PUPD.
    GPIOA->PUPDR &= ~GPIO_MODER_MODE1;

    // Connect PA1 to the ADC input.
    GPIOA->ASCR |= GPIO_ASCR_ASC1;

}

void ADC_Init(void) {

    // Enable & Reset ADC Clock.
    RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;

    // Using one of the RCC reset registers, reset the ADC. Then, clear the reset bit.
    RCC->AHB2RSTR |= RCC_AHB2RSTR_ADCRST; 
		RCC->AHB2RSTR &= ~(RCC_AHB2RSTR_ADCRST);

    // Other ADC Initialization
    ADC_Pin_Init();
    ADC_Common_Configuration();
    ADC_Wakeup();

    // Disable before modifying.
    ADC1->CR &= ~(ADC_CR_ADEN);

    // Configure the ADC to have 12-bit resolution and rightalignment.
    ADC1->CFGR &= ~(ADC_CFGR_RES);
    ADC1->CFGR &= ~(ADC_CFGR_ALIGN);

    // Set the sequence length to 1
    ADC1->SQR1 &= ~(ADC_SQR1_L); 	
    ADC1->SQR1 &= ~(ADC_SQR1_SQ1);

    // Ensure that channel 6 is used for the first conversion.
    ADC1->SQR1 |= ADC_SQR1_SQ1_2; 					
    ADC1->SQR1 |= ADC_SQR1_SQ1_1;

    // Ensure that channel 6 is set to single-ended mode.
    ADC1->DIFSEL &= ~(ADC_DIFSEL_DIFSEL_6); 
    ADC1->SMPR1 &= ~(ADC_SMPR1_SMP6);

    // Set the sampling time for channel 6 to be 24.5 ADC clock cycles.
    ADC1->SMPR1 |= ADC_SMPR1_SMP6_1; 					
    ADC1->SMPR1 |= ADC_SMPR1_SMP6_0;

    // Ensure that the ADC is in single conversion mode and that hardware trigger detection is disabled.
    ADC1->CFGR &= ~(ADC_CFGR_CONT);  
    ADC1->CFGR &= ~(ADC_CFGR_EXTEN); 
    
    // Enable the ADC.
    ADC1->CR |= ADC_CR_ADEN;
    
    // Wait until the ADC is ready.
    while((ADC1->ISR & ADC_ISR_ADRDY) == 0); 
}
