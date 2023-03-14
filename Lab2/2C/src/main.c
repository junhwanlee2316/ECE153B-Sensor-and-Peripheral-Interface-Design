/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Junhwan Lee, Taeho Kim
 * Section: Tuesday 7pm
 * Lab: 2C
 */
 
#include "stm32l476xx.h"

#include "LED.h"
#include "RTC.h"
#include "SysClock.h"

char strTime[12] = {0};
char strDate[12] = {0};

void RTC_Set_Alarm(void) {
	// Disable both alarms in RTC CR
	RTC->CR &= ~RTC_CR_ALRAE;
	RTC->CR &= ~RTC_CR_ALRBE;

	// Remove RTC write protection so that we can write to the RTC registers
	RTC_Disable_Write_Protection();

	// Clear the alarm enable bit and the interrupt enable bit for both alarms 
	RTC->CR &= ~RTC_CR_ALRAE;
	RTC->CR &= ~RTC_CR_ALRBE;
	RTC->CR &= ~RTC_CR_ALRAIE;
	RTC->CR &= ~RTC_CR_ALRBIE;

	// Wait until access to both alarm registers is allowed
	while(!((RTC->ISR) & RTC_ISR_ALRAWF) || !((RTC->ISR) & RTC_ISR_ALRBWF)) {
	}

	// Alarm A to set off an alarm when the seconds field of the RTC is 30 seconds
	RTC->ALRMAR &= ~RTC_ALRMAR_MSK1;
	RTC->ALRMAR |= RTC_ALRMAR_MSK2;
	RTC->ALRMAR |= RTC_ALRMAR_MSK3;
	RTC->ALRMAR |= RTC_ALRMAR_MSK4;
	RTC->ALRMAR |= RTC_ALRMAR_ST_0;		// Tens : 011 ; Ones: 0
	RTC->ALRMAR |= RTC_ALRMAR_ST_1;

	// Alarm B to set off an alarm every second
	RTC->ALRMBR |= RTC_ALRMAR_MSK1;
	RTC->ALRMBR |= RTC_ALRMAR_MSK2;
	RTC->ALRMBR |= RTC_ALRMAR_MSK3;
	RTC->ALRMBR |= RTC_ALRMAR_MSK4;

	// Enable both alarms A and B and their interrupts
	RTC->CR |= RTC_CR_ALRAE;
	//RTC->CR |= RTC_CR_ALRBE;
	RTC->CR |= RTC_CR_ALRAIE;
	//RTC->CR |= RTC_CR_ALRBIE;

	// Re-enable write protection for the RTC registers
	RTC_Enable_Write_Protection();

}

void RTC_Alarm_Enable(void) {
	// Configure the interrupt to trigger on the rising edge in EXTI RTSRx
	EXTI->RTSR1 |= EXTI_RTSR1_RT18;

	// Set the interrupt mask in EXTI IMRx and the event mask in EXTI EMRx
	EXTI->IMR1 |= EXTI_IMR1_IM18;
	EXTI->EMR1 |= EXTI_EMR1_EM18;

	// Clear the pending interrupt in EXTI PRx by writing a 1 to the bit that corresponds
	// to the target EXTI line
	EXTI->PR1 |= EXTI_PR1_PIF18;

	// Enable the interrupt in the NVIC and set it to have the highest priority
	NVIC_EnableIRQ(RTC_Alarm_IRQn);
	NVIC_SetPriority(RTC_Alarm_IRQn, 0);
}

void RTC_Alarm_IRQHandler(void) {
	// Enable Alarm A and disable Alarm B. When Alarm A is triggered, toggle the green LED
	if (RTC->ISR & RTC_ISR_ALRAF) {
		Green_LED_Toggle();					// Toggle Green LED
		RTC->ISR &= ~RTC_ISR_ALRAF;			// Clear alarm A event flag
		EXTI->PR1 |= EXTI_PR1_PIF18;		// Clear interrupt pending bit
	}

	// Enable Alarm B and disable Alarm A. When Alarm B is triggered, toggle the green LED
	if (RTC->ISR & RTC_ISR_ALRBF) {
		Green_LED_Toggle();					// Toggle the green LED
		RTC->ISR &= ~RTC_ISR_ALRBF;			// Clear alarm B event flag
		EXTI->PR1 |= EXTI_PR1_PIF18;		// Clear interrupt pending bit
	}
}

int main(void) {	
	System_Clock_Init(); // Switch System Clock = 80 MHz
	
	LED_Init();
	
	RTC_Init();
	RTC_Alarm_Enable();
	RTC_Set_Alarm();
	
	while(1) {
		Get_RTC_Calendar(strTime, strDate);
	}
}
