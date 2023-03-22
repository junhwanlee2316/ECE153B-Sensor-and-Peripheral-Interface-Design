/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Junhwan Lee, Taeho Kim
 * Section: Tuesday
 * Project
*/

#include "UART.h"

void UART1_Init(void) {
	// part a 2.3 (step 1)
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;			// enable USART1 clock in peripheral clk reg
	
	RCC->CCIPR &= ~RCC_CCIPR_USART1SEL;				// 1.b select the sys clk as USART1 clk src 
	RCC->CCIPR |= RCC_CCIPR_USART1SEL_0;
}

void UART2_Init(void) {
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;			// 1.a enable USART2 clock in peripheral clk reg
	
	RCC->CCIPR &= ~RCC_CCIPR_USART2SEL;
	RCC->CCIPR |= RCC_CCIPR_USART2SEL_0;		// 1.b select the sys clk as USART2 clk src 
}

void UART3_Init(void) {
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;			// enable USART1 clock in peripheral clk reg
	
	RCC->CCIPR &= ~RCC_CCIPR_USART1SEL;				// 1.b select the sys clk as USART1 clk src 
	RCC->CCIPR |= RCC_CCIPR_USART1SEL_0;
}

void UART1_GPIO_Init(void) {
	// part a 2.3 (step 2)
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; //clk enabled

	//pins set to High speed 
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6;	//6
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR7;	//7

	GPIOB->OTYPER &= ~GPIO_OTYPER_OT6;
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT7;			//pins set to push-pull
	
	GPIOB->PUPDR |= (GPIO_PUPDR_PUPD6_0) | (GPIO_PUPDR_PUPD7_0);			//pins set to pull-up resistor
	
	//MODER//
	GPIOB->MODER &= ~GPIO_MODER_MODE6;	//pin 6
	GPIOB->MODER |= GPIO_MODER_MODE6_1; 
	
	GPIOB->MODER &= ~GPIO_MODER_MODE7;	//pin 7
	GPIOB->MODER |= GPIO_MODER_MODE7_1;							
	
	//set to AF//
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL6;		//pin 6
	GPIOB->AFR[0] |= (GPIO_AFRL_AFSEL6_2 | GPIO_AFRL_AFSEL6_1 | GPIO_AFRL_AFSEL6_0);

	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL7;		//pin 7
	GPIOB->AFR[0] |= (GPIO_AFRL_AFSEL7_2 | GPIO_AFRL_AFSEL7_1 | GPIO_AFRL_AFSEL7_0);
}

void UART2_GPIO_Init(void) {
	//part a 
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; //clk enabled

	// 2.a pins set to High speed//
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2;	//pin2
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3;	//pin3
	
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT2;
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT3;			// 2.b pins set to push-pull
	
	GPIOA->PUPDR |= (GPIO_PUPDR_PUPD2_0) | (GPIO_PUPDR_PUPD3_0);			// 2.c pins set to pull-up resistor
	
	//MODER//
	GPIOA->MODER &= ~GPIO_MODER_MODE2;	//pin 2
	GPIOA->MODER |= GPIO_MODER_MODE2_1; 
	
	GPIOA->MODER &= ~GPIO_MODER_MODE3;	//pin 3
	GPIOA->MODER |= GPIO_MODER_MODE3_1;							
	
	//set to AF//
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL2;		//pin 2
	GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL2_2 | GPIO_AFRL_AFSEL2_1 | GPIO_AFRL_AFSEL2_0);

	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL3;		//pin 3
	GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL3_2 | GPIO_AFRL_AFSEL3_1 | GPIO_AFRL_AFSEL3_0);

}

void UART3_GPIO_Init(void) {
	// part a 2.3 (step 2)
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; //clk enabled

	//pins set to High speed 
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR10;	//6
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR11;	//7

	GPIOB->OTYPER &= ~GPIO_OTYPER_OT10;
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT11;			//pins set to push-pull
	
	GPIOB->PUPDR |= (GPIO_PUPDR_PUPD10_0) | (GPIO_PUPDR_PUPD11_0);			//pins set to pull-up resistor
	
	//MODER//
	GPIOB->MODER &= ~GPIO_MODER_MODE10;	//pin 6
	GPIOB->MODER |= GPIO_MODER_MODE10_1; 
	
	GPIOB->MODER &= ~GPIO_MODER_MODE11;	//pin 7
	GPIOB->MODER |= GPIO_MODER_MODE11_1;							
	
	//set to AF//
	GPIOB->AFR[1] &= ~GPIO_AFRH_AFSEL10;		//pin 6
	GPIOB->AFR[1] |= (GPIO_AFRH_AFSEL10_2 | GPIO_AFRH_AFSEL10_1 | GPIO_AFRH_AFSEL10_0);

	GPIOB->AFR[1] &= ~GPIO_AFRH_AFSEL11;		//pin 7
	GPIOB->AFR[1] |= (GPIO_AFRH_AFSEL11_2 | GPIO_AFRH_AFSEL11_1 | GPIO_AFRH_AFSEL11_0);
	
}

void USART_Init(USART_TypeDef* USARTx) {
	USARTx->CR1 &= ~USART_CR1_UE; 					// disable USART before modifying regs

	USARTx->CR1 &= ~(USART_CR1_M1 | USART_CR1_M0); 	// 3.a M1M0 = 00 = 1 start, 8 data bits, n stop bits
	USARTx->CR1 &= ~USART_CR1_OVER8;				// 0 = oversampling by 16
	USARTx->CR2 &= ~USART_CR2_STOP;					// 00 = 1 stop bit

	//3.b set USARTDIV in BRR[3:0] (*note: BRR[3:0] == USARTDIV[3:0] when USARTx->CR1 bit 16 (line 50) is 0)
	USARTx->BRR &= ~0xFFFF; //clear [15:0] 
	USARTx->BRR = 8333;	//BaudRate = f_clk/(USARTDIV) = 8333.333 ~ 8333 = 0x208D

	//3.c enable transmitter and receiver 
	USARTx->CR1 |= USART_CR1_TE;					//enable transmitter
	USARTx->CR1 |= USART_CR1_RE;					//enable receiver

	USARTx->CR1 |= USART_CR1_UE; 					// EnablUSART diabled 
}

uint8_t USART_Read (USART_TypeDef * USARTx) {
	// SR_RXNE (Read data register not empty) bit is set by hardware
	while (!(USARTx->ISR & USART_ISR_RXNE));  // Wait until RXNE (RX not empty) bit is set
	// USART resets the RXNE flag automatically after reading DR
	return ((uint8_t)(USARTx->RDR & 0xFF));
	// Reading USART_DR automatically clears the RXNE flag 
}

void USART_Write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes) {
	int i;
	// TXE is cleared by a write to the USART_DR register.
	// TXE is set by hardware when the content of the TDR 
	// register has been transferred into the shift register.
	for (i = 0; i < nBytes; i++) {
		while (!(USARTx->ISR & USART_ISR_TXE));   	// wait until TXE (TX empty) bit is set
		// Writing USART_DR automatically clears the TXE flag 	
		USARTx->TDR = buffer[i] & 0xFF;
		USART_Delay(300);
	}
	while (!(USARTx->ISR & USART_ISR_TC));   		  // wait until TC bit is set
	USARTx->ISR &= ~USART_ISR_TC;
}   

void USART_Delay(uint32_t us) {
	uint32_t time = 100*us/7;    
	while(--time);   
}
