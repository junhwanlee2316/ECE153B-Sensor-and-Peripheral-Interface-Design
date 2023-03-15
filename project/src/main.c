/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Junhwan Lee, Taeho Kim
 * Section: Tuesday
 * Project
 */
 

#include "stm32l476xx.h"
#include "UltSonic.h"
#include "I2C.h"
#include "Nunchuck.h"
#include "SysClock.h"
#include "Stepper.h"


uint32_t volatile timeInterval = 0;
uint32_t volatile dist_cm = 0;
static uint32_t Data_Receive;
static uint8_t command = 0x40;

int main(void) {	

	System_Clock_Init(); // System Clock = 80 MHz

	// Enable High Speed Internal Clock (HSI = 16 MHz)
	RCC->CR |= RCC_CR_HSION;
	while ((RCC->CR & RCC_CR_HSIRDY) == 0); // Wait until HSI is ready
	
	// Select HSI as system clock source 
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_HSI;
	while ((RCC->CFGR & RCC_CFGR_SWS) == 0); // Wait until HSI is system clock source
	
  	I2C_GPIO_Init();
	I2C_Initialization();

	Setup();
	initChuk();

	uint8_t *Data_Receive;
	int direction = 0;
	while(1) {
		
		timeInterval = getTimeInterval();
		// Timer clock = 1MHz = 1us
		// Distance in cm = pulse width(us) / 58
		int t = timeInterval;
		if (t/1000 < 38) {
			dist_cm = t/58;
		}
		
		Data_Receive = readChuk();
		direction = getChukDir(Data_Receive);

		while(direction == 1){
			
			// move motor to clockwise
			for(int i = 0; i < 100; i++){
				Full_Stepping_Clockwise();

			}
			Data_Receive = readChuk();
			direction = getChukDir(Data_Receive);
		}
		stopMotor();
		while(direction == 2){
			// move motor to counter-clockwise
			for(int i = 0; i < 100; i++){
				Full_Stepping_CounterClockwise();

			}
			Data_Receive = readChuk();
			direction = getChukDir(Data_Receive);
		}
		stopMotor();
	}
}
