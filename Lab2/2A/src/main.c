/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Junhwan Lee, Taeho Kim
 * Section: Tuesday 7pm
 * Lab: 2A
 */

#include "stm32l476xx.h"
#include "EXTI.h"
#include "LED.h"
#include "SysClock.h"

int main() {
	// Initialization
	System_Clock_Init();
	LED_Init();
	EXTI_Init();
	
	while(1);
	
	return 0;
}
