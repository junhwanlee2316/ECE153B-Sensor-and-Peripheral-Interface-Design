#include "I2C.h"
#include "Nunchuck.h"

// Address for Wii Nunchuck
static uint32_t SLAVE_ADDRESS = 0x52;

void initChuk(void) {
	
	// the init bytes are 0x40 0x00
	// the start bytes arre 0x00 
	uint8_t initBytes[2] = {0x40, 0x00};
	uint8_t startByte = 0x00;
	
	// initalize
	I2C_SendData(I2C1, SLAVE_ADDRESS<<1, initBytes, 2);
	I2C_SendData(I2C1, SLAVE_ADDRESS<<1, &startByte, 1);
	
}

uint8_t* readChuk(void) {

	uint8_t Data_Receive[6];
	uint8_t zeroByte = 0x00;
	
	I2C_SendData(I2C1, SLAVE_ADDRESS<<1, &zeroByte, 1);
	I2C_ReceiveData(I2C1, 0xA5, Data_Receive, 6);

    return Data_Receive;
}

int getChukDir(uint8_t* chukBuf) {
	uint8_t x = chukBuf[0];
	
	if (x < 0x70) {
		return 1;   //left
	}
	if (x > 0x90) {
		return 2;   //right
	}
	return 0;
}
