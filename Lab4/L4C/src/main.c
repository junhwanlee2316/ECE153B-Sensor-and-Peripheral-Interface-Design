/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Taeho Kim, Junhwan Lee
 * Section:	Tuesday 7pm
 * Lab: 4C
 */


#include "stm32l476xx.h"
#include "LED.h"
#include "SPI.h"
#include "SysClock.h"
#include "SysTimer.h"
#include <stdio.h>

uint8_t transmit = 0;
uint8_t receive = 0;

int main(void){
	System_Clock_Init();   // System Clock = 80 MHz
	SysTick_Init();
	
	LED_Init();
	
	SPI1_GPIO_Init();
	SPI2_GPIO_Init();
	SPI1_Init();
	SPI2_Init();
	
	while(1) {
		SPI_Send_Byte(SPI1, transmit);
		SPI_Receive_Byte(SPI2, &receive);
		
		if (transmit == receive) {
			LED_Toggle();
		}
		
		transmit = (transmit + 1) % 10;
		
		
		// delay between SPI transfers to observe LED behavior
	    delay(1000);
	}
}
