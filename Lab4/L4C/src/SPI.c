#include "SPI.h"

// Note: When the data frame size is 8 bit, "SPIx->DR = byte_data;" works incorrectly. 
// It mistakenly send two bytes out because SPIx->DR has 16 bits. To solve the program,
// we should use "*((volatile uint8_t*)&SPIx->DR) = byte_data";

void SPI1_GPIO_Init(void) {
	// TODO: initialize SPI1 GPIO pins
	
	// Setting PB3 for AF5 (SPI1_SCK)
	GPIOB->MODER &= ~GPIO_MODER_MODE3;	// clearing bits for MODE3	
	GPIOB->MODER |= GPIO_MODER_MODE3_1;	// Setting MODE3 to 10 (AF)

	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL3; // clearing bits for AFSEL3
	GPIOB->AFR[0] |= (GPIO_AFRL_AFSEL3_0 + GPIO_AFRL_AFSEL3_2); //AFSEL3 = 0101 (AF5)
	
	// Output type = push-pull
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT3; // OT3 = 0 (push-pull)
	
	// Output Speed = Very High
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED3; // OSPEED3 = 11 (very high)
	
	// Pull-Up/Down = No Pull
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD3; // PUPD3 = 00 (no pull)
	
	
	// Setting PB4 for AF5 (SPI1_MISO)
	GPIOB->MODER &= ~GPIO_MODER_MODE4;	// clearing bits for MODE4	
	GPIOB->MODER |= GPIO_MODER_MODE4_1;	// Setting MODE4 to 10 (AF)

	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL4; // clearing bits for AFSEL4
	GPIOB->AFR[0] |= (GPIO_AFRL_AFSEL4_0 + GPIO_AFRL_AFSEL4_2); //AFSEL4 = 0101 (AF5)
	
	// Output type = push-pull
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT4; // OT4 = 0 (push-pull)
	
	// Output Speed = Very High
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED4; // OSPEED4 = 11 (very high)
	
	// Pull-Up/Down = No Pull
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD4; // PUPD4 = 00 (no pull)
	
	
	// Setting PB5 for AF5 (SPI1_MOSI)
	GPIOB->MODER &= ~GPIO_MODER_MODE5;	// clearing bits for MODE5	
	GPIOB->MODER |= GPIO_MODER_MODE5_1;	// Setting MODE5 to 10 (AF)

	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL5; // clearing bits for AFSEL5
	GPIOB->AFR[0] |= (GPIO_AFRL_AFSEL5_0 + GPIO_AFRL_AFSEL5_2); //AFSEL5 = 0101 (AF5)
	
	// Output type = push-pull
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT5; // OT5 = 0 (push-pull)
	
	// Output Speed = Very High
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED5; // OSPEED5 = 11 (very high)
	
	// Pull-Up/Down = No Pull
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD5; // PUPD5 = 00 (no pull)
}

void SPI1_Init(void){
	// TODO: initialize SPI1 peripheral as master
	
	// Enable the SPI clock
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // SPI1EN = 1 (enable)
	
	// Set the RCC SPI reset bit, then clear it to reset the SPI1 or SPI2 peripheral
	RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST; // SPI1RST = 1 (reset)
	RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1RST; // SPI1RST = 0 (no effect)
	
	// Disable the SPI enable bit. The peripheral must be configured while it is disabled
	SPI1->CR1 &= ~SPI_CR1_SPE; // SPE = 0 (disable)
	
	// Configure the peripheral for full-duplex communication
	SPI1->CR1 &= ~SPI_CR1_RXONLY; // RXONLY = 0 (full duplex)
	
	// Configure the peripheral for 2-line unidirectional data mode
	SPI1->CR1 &= ~SPI_CR1_BIDIMODE;	// BIDIMODE = 0 (2-line undirectional data mode)
	
	// Disable output in bidirectional mode
	SPI1->CR1 &= ~SPI_CR1_BIDIOE; // BIDIOE = 0 (output disabled)
	
	// Configure the frame format as MSB first
	SPI1->CR1 &= ~SPI_CR1_LSBFIRST; // LSBFIRST = 0 (MSB first)
	
	// Configure the frame format to 8-bit mode
	SPI1->CR2 |= SPI_CR2_DS; // DS = 1111
	SPI1->CR2 &= ~SPI_CR2_DS_3; // DS = 0111 (8-bit mode)
	
	// Use Motorola SPI mode
	SPI1->CR2 |= SPI_CR2_DS; // DS = 0 (motorola mode)
	
	// Configure the clock to low polarity
	SPI1->CR1 &= ~SPI_CR1_CPOL; // CPOL = 0 (low polarity)
	
	// Configure the clock to first clock transition
	SPI1->CR1 &= ~SPI_CR1_CPHA;	// CPHA = 0 (first clock transition)
	
	// Set the baud rate prescaler to 16
	SPI1->CR1 |= SPI_CR1_BR; // BR = 111
	SPI1->CR1 &= ~SPI_CR1_BR_2; // BR = 011 (baud rate = 16)
	
	// Disable hardware CRC calculation
	SPI1->CR1 &= ~SPI_CR1_CRCEN; // CRCEN = 0 (disable)
	
	// Set SPI1 to master mode and SPI2 to slave mode
	SPI1->CR1 |= SPI_CR1_MSTR; // MSTR = 1 (master mode)
	// SPI2->CR1 &= ~SPI_CR1_MSTR; //MSTR = 0 (SPI2 to slave mode)
	
	// Enable software SSM
	SPI1->CR1 |= SPI_CR1_SSM; // SSM = 1 (software slave enable)
	
	// Enable NSS pulse generation
	SPI1->CR2 |= SPI_CR2_NSSP; // NSSP = 1 (NSS pulse generated)
	
	// Configure the internal slave select bit, 1 for master and 0 for slave
	SPI1->CR1 |= SPI_CR1_SSI; // SSI = 1 (master)
	
	// Set the FIFO threshold to 1/4 (required for 8-bit mode)
	SPI1->CR2 |= SPI_CR2_FRXTH; // FRXTH = 1 (threshold = 1/4)
	
	// Enable the SPI peripheral
	SPI1->CR1 |= SPI_CR1_SPE; // SPE = 1 (enable)
}

void SPI2_GPIO_Init(void) {
	// TODO: initialize SPI2 GPIO pins
	
	// Setting PB13 for AF5 (SPI2_SCK)
	GPIOB->MODER &= ~GPIO_MODER_MODE13;	// clearing bits for MODE13	
	GPIOB->MODER |= GPIO_MODER_MODE13_1;	// Setting MODE13 to 10 (AF)

	GPIOB->AFR[1] &= ~GPIO_AFRH_AFSEL13; // clearing bits for AFSEL13
	GPIOB->AFR[1] |= (GPIO_AFRH_AFSEL13_0 + GPIO_AFRH_AFSEL13_2); //AFSEL13 = 0101 (AF5)
	
	// Output type = push-pull
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT13; // OT13 = 0 (push-pull)
	
	// Output Speed = Very High
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED13; // OSPEED13 = 11 (very high)
	
	// Pull-Up/Down = No Pull
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD13; // PUPD13 = 00 (no pull)
	
	
	// Setting PB14 for AF5 (SPI2_MISO)
	GPIOB->MODER &= ~GPIO_MODER_MODE14;	// clearing bits for MODE14	
	GPIOB->MODER |= GPIO_MODER_MODE14_1;	// Setting MODE14 to 10 (AF)

	GPIOB->AFR[1] &= ~GPIO_AFRH_AFSEL14; // clearing bits for AFSEL14
	GPIOB->AFR[1] |= (GPIO_AFRH_AFSEL14_0 + GPIO_AFRH_AFSEL14_2); //AFSEL14 = 0101 (AF5)
	
	// Output type = push-pull
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT14; // OT14 = 0 (push-pull)
	
	// Output Speed = Very High
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED14; // OSPEED14 = 11 (very high)
	
	// Pull-Up/Down = No Pull
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD14; // PUPD14 = 00 (no pull)
	
	
	// Setting PB15 for AF5 (SPI2_MOSI)
	GPIOB->MODER &= ~GPIO_MODER_MODE15;	// clearing bits for MODE15	
	GPIOB->MODER |= GPIO_MODER_MODE15_1;	// Setting MODE15 to 10 (AF)

	GPIOB->AFR[1] &= ~GPIO_AFRH_AFSEL15; // clearing bits for AFSEL15
	GPIOB->AFR[1] |= (GPIO_AFRH_AFSEL15_0 + GPIO_AFRH_AFSEL15_2); //AFSEL15 = 0101 (AF5)
	
	// Output type = push-pull
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT15; // OT15 = 0 (push-pull)
	
	// Output Speed = Very High
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED15; // OSPEED15 = 11 (very high)
	
	// Pull-Up/Down = No Pull
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD15; // PUPD15 = 00 (no pull)
}

void SPI2_Init(void){
	// TODO: initialize SPI2 peripheral as slave
	
	// Enable the SPI clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN; // SPI2EN = 1 (enable)
	
	// Set the RCC SPI reset bit, then clear it to reset the SPI1 or SPI2 peripheral
	RCC->APB1RSTR1 |= RCC_APB1RSTR1_SPI2RST; // SPI1RST = 1 (reset)
	RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_SPI2RST; // SPI1RST = 0 (no effect)
	
	// Disable the SPI enable bit. The peripheral must be configured while it is disabled
	SPI2->CR1 &= ~SPI_CR1_SPE; // SPE = 0 (disable)
	
	// Configure the peripheral for full-duplex communication
	SPI2->CR1 &= ~SPI_CR1_RXONLY; // RXONLY = 0 (full duplex)
	
	// Configure the peripheral for 2-line unidirectional data mode
	SPI2->CR1 &= ~SPI_CR1_BIDIMODE;	// BIDIMODE = 0 (2-line undirectional data mode)
	
	// Disable output in bidirectional mode
	SPI2->CR1 &= ~SPI_CR1_BIDIOE; // BIDIOE = 0 (output disabled)
	
	// Configure the frame format as MSB first
	SPI2->CR1 &= ~SPI_CR1_LSBFIRST; // LSBFIRST = 0 (MSB first)
	
	// Configure the frame format to 8-bit mode
	SPI2->CR2 |= SPI_CR2_DS; // DS = 1111
	SPI2->CR2 &= ~SPI_CR2_DS_3; // DS = 0111 (8-bit mode)
	
	// Use Motorola SPI mode
	SPI2->CR2 |= SPI_CR2_DS; // DS = 0 (motorola mode)
	
	// Configure the clock to low polarity
	SPI2->CR1 &= ~SPI_CR1_CPOL; // CPOL = 0 (low polarity)
	
	// Configure the clock to first clock transition
	SPI2->CR1 &= ~SPI_CR1_CPHA;	// CPHA = 0 (first clock transition)
	
	// Set the baud rate prescaler to 16
	SPI2->CR1 |= SPI_CR1_BR; // BR = 111
	SPI2->CR1 &= ~SPI_CR1_BR_2; // BR = 011 (baud rate = 16)
	
	// Disable hardware CRC calculation
	SPI1->CR1 &= ~SPI_CR1_CRCEN; // CRCEN = 0 (disable)
	
	// Set SPI1 to master mode and SPI2 to slave mode
	SPI2->CR1 &= ~SPI_CR1_MSTR; //MSTR = 0 (SPI2 to slave mode)
	
	// Enable software SSM
	SPI2->CR1 |= SPI_CR1_SSM; // SSM = 1 (software slave enable)
	
	// Enable NSS pulse generation
	SPI2->CR2 |= SPI_CR2_NSSP; // NSSP = 1 (NSS pulse generated)
	
	// Configure the internal slave select bit, 1 for master and 0 for slave
	SPI2->CR1 &= ~SPI_CR1_SSI; // SSI = 1 (master)
	
	// Set the FIFO threshold to 1/4 (required for 8-bit mode)
	SPI2->CR2 |= SPI_CR2_FRXTH; // FRXTH = 1 (threshold = 1/4)
	
	// Enable the SPI peripheral
	SPI2->CR1 |= SPI_CR1_SPE; // SPE = 1 (enable)
}
 
void SPI_Send_Byte(SPI_TypeDef* SPIx, uint8_t write_data) {
	// TODO: send data from SPI1
	while((SPIx->SR & SPI_SR_TXE) == 0); // Wait until Transmit Buffer Empty flag to become set
	*((uint8_t volatile *) &SPIx->DR) =  write_data;
	while((SPIx->SR & SPI_SR_BSY) == 1); // Wait until busy flag to be unset
}

void SPI_Receive_Byte(SPI_TypeDef* SPIx, uint8_t* read_data) {
	// TODO: receive data from SPI2
	while((SPIx->SR & SPI_SR_RXNE) == 0); // Wait until not empty flag
	*read_data = *((uint8_t volatile *) &SPIx->DR);
}

