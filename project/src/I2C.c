#include "I2C.h"

extern void Error_Handler(void);

// Inter-integrated Circuit Interface (I2C)
// up to 100 Kbit/s in the standard mode, 
// up to 400 Kbit/s in the fast mode, and 
// up to 3.4 Mbit/s in the high-speed mode.

// Recommended external pull-up resistance is 
// 4.7 kOmh for low speed, 
// 3.0 kOmh for the standard mode, and 
// 1.0 kOmh for the fast mode
	
//===============================================================================
//                        I2C GPIO Initialization
//===============================================================================
void I2C_GPIO_Init(void) {

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; //clk enabled
	// configure PB6 for I2C1_SCL, PB7 for I2C1_SDA

	// set both pins to alternate function mode (10)
	// explanation: default is 11, so use mask &~(01) to set second bit to 0
	GPIOB->MODER &= ~(GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0);

	// set pin 6,7 to AF4
	int afWidth = 4;
	GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL6 | GPIO_AFRL_AFSEL7); // reset to AF0
	GPIOB->AFR[0] |= (4 << (6*afWidth)); // pin 6 to AF4
	GPIOB->AFR[0] |= (4 << (7*afWidth)); // pin 7 to AF4

	// set both pins to open-drain (1)
	GPIOB->OTYPER |= (GPIO_OTYPER_OT6 | GPIO_OTYPER_OT7);

	// set both pins to very high (11)
	GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7);
	
	// set both pins to pull-up (01)
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD6 | GPIO_PUPDR_PUPD7);
	GPIOB->PUPDR |= (GPIO_PUPDR_PUPD6_0 | GPIO_PUPDR_PUPD7_0);
}
	
#define I2C_TIMINGR_PRESC_POS	28
#define I2C_TIMINGR_SCLDEL_POS	20
#define I2C_TIMINGR_SDADEL_POS	16
#define I2C_TIMINGR_SCLH_POS	8
#define I2C_TIMINGR_SCLL_POS	0

//===============================================================================
//                          I2C Initialization
//===============================================================================
void I2C_Initialization(void){
	uint32_t OwnAddr = 0x52;
	//part B
	
	RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN; // 1.a enable peripheral clk

	//set sys clk as src for I2C1 in the peripherals independent clock configuration register
	RCC->CCIPR &= ~RCC_CCIPR_I2C1SEL; 	//1.b 
	RCC->CCIPR |=  RCC_CCIPR_I2C1SEL_0; 

	//Reset I2C1 by setting bits in the peripheral reset register
	RCC->APB1RSTR1 |= RCC_APB1RSTR1_I2C1RST; //1.c
	RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_I2C1RST; //clear bits 

	//disable I2C
	I2C1->CR1 &= ~I2C_CR1_PE;

	//2.a
	//enable error interrupts
	I2C1->CR1 |= I2C_CR1_ERRIE;
	//enable analog noise fileter, disable digital noise filter, enable clk stretching
	I2C1->CR1 &= ~(I2C_CR1_ANFOFF | I2C_CR1_DNF | I2C_CR1_NOSTRETCH); 
	
	//set master to operate in 7-bit addressing
	I2C1->CR2 &= ~I2C_CR2_ADD10;
	//enable auto-end mode, enable NACK generation
	I2C1->CR2 |= (I2C_CR2_AUTOEND | I2C_CR2_NACK);

	//2.b
	/*
	Min Low clk per. = 4.7us
	Min Hi clk per. = 4.0us
	Min data setup time = 1000ns = 1us
	Min data hold time = 1250ns = 1.25us
	
	PRESC = 7
	f_PRESC = 80MHz/(1+7) = 10MHz (t_PRESC = 0.1us)
	SCLDEL = 0
	t_SCLDEL = (SCLDEL + 1) * t_PRESC = (10 + 1)*0.1us = 1.1us Condition: [t_SCLDEL > 1us] 
	SDADEL = 0
	t_SDADEL = (SDADEL + 1) * t_PRESC = (12 + 1)*0.1us = 1.3us	     	 [t_SDADEL > 1.25us]
	SCLL = 	0
	t_SCLL = (SCLL + 1) * t_PRESC = (47 + 1)*0.1us = 4.8us		         [t_SCLL > 4.7us]
	SCLH = 	0
	t_SCLH = (SCLH + 1) * t_PRESC = (40 + 1)*0.1us = 4.1us		         [t_SCLH > 4.0us]
	
	*/

	I2C1->TIMINGR |= ((7<<I2C_TIMINGR_PRESC_POS) 
					 |(10<<I2C_TIMINGR_SCLDEL_POS)
					 |(12<<I2C_TIMINGR_SDADEL_POS)
					 |(47<<I2C_TIMINGR_SCLH_POS)
					 |(40<<I2C_TIMINGR_SCLL_POS));
	
	// part 3 disable own address
	I2C1->OAR1 &= ~I2C_OAR1_OA1EN; 
	I2C1->OAR2 &= ~I2C_OAR2_OA2EN;
	
	//address set to 7 bit mode
	I2C1->OAR1 &= ~I2C_OAR1_OA1MODE; //3.a
	
	// Set Address to OwnAddr
	I2C1->OAR1 |= OwnAddr; //3.b
	
	// Enable Own1 Address
	I2C1->OAR1 |= I2C_OAR1_OA1EN; //3.c
	
	// Enable I2C1
	I2C1->CR1 |= I2C_CR1_PE; //4
}

//===============================================================================
//                           I2C Start
// Master generates START condition:
//    -- Slave address: 7 bits
//    -- Automatically generate a STOP condition after all bytes have been transmitted 
// Direction = 0: Master requests a write transfer
// Direction = 1: Master requests a read transfer
//=============================================================================== 
int8_t I2C_Start(I2C_TypeDef * I2Cx, uint32_t DevAddress, uint8_t Size, uint8_t Direction) {	
	
	// Direction = 0: Master requests a write transfer
	// Direction = 1: Master requests a read transfer
	
	uint32_t x_axis = 0;
	
	// This bit is set by software, and cleared by hardware after the Start followed by the address
	// sequence is sent, by an arbitration loss, by a timeout error detection, or when PE = 0.
	x_axis = I2Cx->CR2;
	
	tmpreg &= (uint32_t)~((uint32_t)(I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | I2C_CR2_RD_WRN | I2C_CR2_START | I2C_CR2_STOP));
	
	if (Direction == READ_FROM_SLAVE)
		tmpreg |= I2C_CR2_RD_WRN;  // Read from Slave
	else
		tmpreg &= ~I2C_CR2_RD_WRN; // Write to Slave
		
	tmpreg |= (uint32_t)(((uint32_t)DevAddress & I2C_CR2_SADD) | (((uint32_t)Size << 16 ) & I2C_CR2_NBYTES));
	
	tmpreg |= I2C_CR2_START;
	
	I2Cx->CR2 = tmpreg; 
	
   	return 0;  // Success
}

//===============================================================================
//                           I2C Stop
//=============================================================================== 
void I2C_Stop(I2C_TypeDef * I2Cx){
	// Master: Generate STOP bit after the current byte has been transferred 
	I2Cx->CR2 |= I2C_CR2_STOP;								
	// Wait until STOPF flag is reset
	while( (I2Cx->ISR & I2C_ISR_STOPF) == 0 ); 
}

//===============================================================================
//                           Wait for the bus is ready
//=============================================================================== 
void I2C_WaitLineIdle(I2C_TypeDef * I2Cx){
	// Wait until I2C bus is ready
	while( (I2Cx->ISR & I2C_ISR_BUSY) == I2C_ISR_BUSY );	// If busy, wait
}

//===============================================================================
//                           I2C Send Data
//=============================================================================== 
int8_t I2C_SendData(I2C_TypeDef * I2Cx, uint8_t DeviceAddress, uint8_t *pData, uint8_t Size) {
	int i;
	
	if (Size <= 0 || pData == NULL) return -1;
	
	I2C_WaitLineIdle(I2Cx);
	
	if (I2C_Start(I2Cx, DeviceAddress, Size, WRITE_TO_SLAVE) < 0 ) return -1;

	// Send Data
	// Write the first data in DR register
	// while((I2Cx->ISR & I2C_ISR_TXE) == 0);
	// I2Cx->TXDR = pData[0] & I2C_TXDR_TXDATA;  

	for (i = 0; i < Size; i++) {
		// TXE is set by hardware when the I2C_TXDR register is empty. It is cleared when the next
		// data to be sent is written in the I2C_TXDR register.
		// while( (I2Cx->ISR & I2C_ISR_TXE) == 0 ); 

		// TXIS bit is set by hardware when the I2C_TXDR register is empty and the data to be
		// transmitted must be written in the I2C_TXDR register. It is cleared when the next data to be
		// sent is written in the I2C_TXDR register.
		// The TXIS flag is not set when a NACK is received.
		while((I2Cx->ISR & I2C_ISR_TXIS) == 0);
		I2Cx->TXDR = pData[i] & I2C_TXDR_TXDATA;  // TXE is cleared by writing to the TXDR register.
	}
	
	// Wait until TC flag is set 
	while((I2Cx->ISR & I2C_ISR_TC) == 0 && (I2Cx->ISR & I2C_ISR_NACKF) == 0);
	
	if( (I2Cx->ISR & I2C_ISR_NACKF) != 0 ) return -1;

	I2C_Stop(I2Cx);
	return 0;
}


//===============================================================================
//                           I2C Receive Data
//=============================================================================== 
int8_t I2C_ReceiveData(I2C_TypeDef * I2Cx, uint8_t DeviceAddress, uint8_t *pData, uint8_t Size) {
	int i;
	
	if(Size <= 0 || pData == NULL) return -1;

	I2C_WaitLineIdle(I2Cx);

	I2C_Start(I2Cx, DeviceAddress, Size, READ_FROM_SLAVE); // 0 = sending data to the slave, 1 = receiving data from the slave
						  	
	for (i = 0; i < Size; i++) {
		// Wait until RXNE flag is set 	
		while( (I2Cx->ISR & I2C_ISR_RXNE) == 0 );
		pData[i] = I2Cx->RXDR & I2C_RXDR_RXDATA;
	}
	
	// Wait until TCR flag is set 
	while((I2Cx->ISR & I2C_ISR_TC) == 0);
	
	I2C_Stop(I2Cx);
	
	return 0;
}