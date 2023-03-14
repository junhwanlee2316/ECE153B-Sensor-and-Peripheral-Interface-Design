#include "ADC.h"
#include "PWM.h"
#include "SysClock.h"
#include "stm32l476xx.h"
#include <stdio.h>

volatile uint32_t ADC_measurement;
volatile double voltage_measured = 0;
uint32_t MAX = 4096;

int main(void) {
  // Initialization
  System_Clock_Init(); // Switch System Clock = 16 MHz

  ADC_Init();
  PWM_Init();

  while (1) {
    // Through the ADC control register, start a regular conversion.
    ADC1->CR |= ADC_CR_ADSTART;
  
    // Wait until the ADC conversion
    while((ADC123_COMMON->CSR & ADC_CSR_EOC_MST)==0);
  
    // Read the ADC data register to retrieve the measurement.
    ADC_measurement = ADC1->DR;    
    voltage_measured = ADC_measurement/(double)MAX; // Vin/Vref
    TIM2->CCR1 = 301*voltage_measured;
  }
}