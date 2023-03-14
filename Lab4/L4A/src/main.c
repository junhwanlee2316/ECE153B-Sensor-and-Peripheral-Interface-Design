/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Junhwan Lee, Taeho Kim
 * Section: Tuesday 7:00 pm
 * Lab: 4A
 */


#include "stm32l476xx.h"
#include "LED.h"
#include "SysClock.h"
#include "UART.h"
#include <string.h>
#include <stdio.h>

// Initializes USARTx
// USART2: UART Communication with Termite
// USART1: Bluetooth Communication with Phone
void Init_USARTx(int x) {
	if(x == 1) {
		UART1_Init();
		UART1_GPIO_Init();
		USART_Init(USART1);
	} else if(x == 2) {
		UART2_Init();
		UART2_GPIO_Init();
		USART_Init(USART2);
	} else {
		// Do nothing...
	}
}

int main(void) {
	System_Clock_Init(); // Switch System Clock = 80 MHz
	LED_Init();
	
	// Initialize UART -- change the argument depending on the part you are working on
	Init_USARTx(1);
	
	char rxByte;
	while(1) {
		int reset = 1;
		printf("Y: turn on LED");
		printf("N: turn off LED");
		scanf(rxByte)
		while(reset == 1){
			scanf("%c", &rxByte);
			if(rxByte == 'Y' || rxByte == 'y'){
				Green_LED_On()
				printf("Led Turned On")
				reset = 0;
			}
			if(rxByte == 'N' || rxByte == 'n'){
				Green_LED_Off()
				printf("Led Turned Off")
				reset = 0;
			}
			else{
				printf("Unrecognized command is received. Try again")
			}
		}
	}
}
