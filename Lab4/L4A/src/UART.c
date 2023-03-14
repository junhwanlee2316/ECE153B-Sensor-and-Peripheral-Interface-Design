#include "UART.h"

void UART1_Init(void) {
	// Enable the USART1 clock in the peripheral clock register
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	// Select the system clock as the USART1 clock source in the peripheral independent clock configuration register.
	// System clock enable is SEL[1:0] = 01
	RCC -> CCIPR &= ~RCC_CCIPR_USART1SEL_1;
	RCC -> CCIPR |= RCC_CCIPR_USART1SEL_0;
}

void UART2_Init(void) {
	// Enable the USART2 clock in the peripheral clock register
	RCC -> APB1ENR1 |= RCC_APB1ENR1_USART2EN;
	// Select the system clock as the USART2 clock source in the peripheral independent clock configuration register.
	// System clock enable is SEL[1:0] = 01
	RCC -> CCIPR &= ~RCC_CCIPR_USART2SEL_1;
	RCC -> CCIPR |= RCC_CCIPR_USART2SEL_0;
}

void UART1_GPIO_Init(void) {

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	GPIOB->MODER &= ~(GPIO_MODER_MODE6_0 + GPIO_MODER_MODE7_0);	
	GPIOB->MODER |= GPIO_MODER_MODE6_1 + GPIO_MODER_MODE7_1;		
	GPIOB->AFR[0] |= (GPIO_AFRL_AFSEL6 + GPIO_AFRL_AFSEL7);
	GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL6_3 + GPIO_AFRL_AFSEL7_3);
	// Configure PB6 and PB7 to operate as UART transmitters and receivers.
	// Both GPIO pins should operate at very high speed.
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6 + GPIO_OSPEEDER_OSPEEDR7;

	// Both GPIO pins should have a push-pull output type.
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT6 + GPIO_OTYPER_OT7);

	// Configure both GPIO pins to use pull-up resistors for I/O.
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD6_1 + GPIO_PUPDR_PUPD7_1);
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD6_0 + GPIO_PUPDR_PUPD7_0;
}


void UART2_GPIO_Init(void) {

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	GPIOA->MODER &= ~(GPIO_MODER_MODE2_0 + GPIO_MODER_MODE3_0);	
	GPIOA->MODER |= GPIO_MODER_MODE2_1 + GPIO_MODER_MODE3_1;		
	GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL2 + GPIO_AFRL_AFSEL3);
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL2_3 + GPIO_AFRL_AFSEL3_3);
	// Configure PA2 and PA3 to operate as UART transmitters and receivers.
	// Both GPIO pins should operate at very high speed.
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2 + GPIO_OSPEEDER_OSPEEDR3;

	// Both GPIO pins should have a push-pull output type.
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT2 + GPIO_OTYPER_OT3);

	// Configure both GPIO pins to use pull-up resistors for I/O.
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD2_1 + GPIO_PUPDR_PUPD3_1);
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD2_0 + GPIO_PUPDR_PUPD3_0;
}

void USART_Init(USART_TypeDef* USARTx) {
	// [TODO]
	// Disable USART first
	USARTx -> CR1 &= ~USART_CR1_UE;		// 0: USART prescaler and outputs disabled, low-power mode

	// Modify control registers
	USARTx -> CR1 &= ~USART_CR1_M; 		// M[1:0] = 00: 1 Start bit, 8 data bits, n stop bits
	USARTx -> CR1 &= ~USART_CR1_OVER8;	// 0: Oversampling by 16
	USARTx -> CR2 &= ~USART_CR2_STOP;	// 00: 1 stop bit

	// Set the baud rate to 9600. To generate the baud rate, you will have to write a value into USARTx BRR.
	uint32_t USARTDIV;
	uint32_t f_CLK = 80000000;			// 80 MHZ from SysClock.c
	uint32_t baud_rate = 9600;

	USARTDIV = f_CLK / baud_rate;
	USARTx -> BRR = USARTDIV;

	// Enable both the transmitter and receiver.
	USARTx->CR1 |= USART_CR1_TE;
	USARTx->CR1 |= USART_CR1_RE;

	// Enable USART in the control registers.
	USARTx -> CR1 |= USART_CR1_UE;
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
