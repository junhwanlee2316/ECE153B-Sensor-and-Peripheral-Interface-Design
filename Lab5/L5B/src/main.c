/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Taeho Kim, Junhwan Lee
 * Section: Tuesday 7pm
 * Lab: 5B
 */


#include "ADC.h"
#include "DAC.h"
#include "EXTI.h"
#include "PWM.h"
#include "SysClock.h"
#include "stm32l476xx.h"
#include <stdio.h>

int main(void) {
    // Initialization
    System_Clock_Init(); // Switch System Clock = 16 MHz

    ADC_Init();
    DAC_Init();
    EXTI_Init();

    // [TODO] Initialize PWM
		//reuse from part a

    while (1) {
        // [TODO] Trigger ADC and get result

        // [TODO] LED behavior based on ADC result
    }
}
