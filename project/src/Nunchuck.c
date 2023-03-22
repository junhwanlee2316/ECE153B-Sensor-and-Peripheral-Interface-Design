/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Junhwan Lee, Taeho Kim
 * Section: Tuesday
 * Project
*/

#include "I2C.h"
#include "Nunchuck.h"

// Address for Wii Nunchuck
static uint8_t SLAVE_ADDRESS = 0x52 << 1;
static uint8_t Data_Receive[6];
static uint8_t zeroByte = 0x00;

// Wii Nunchuk handshake function
void initChuk(void) {

	// Init bytes and start byte
	uint8_t initBytes[2] = {0x40, 0x00};
	uint8_t startBytes = 0x00;
	
	// initalize
	for(int i = 0; i< 5000; i++){}

	// Send data
	I2C_SendData(I2C1, SLAVE_ADDRESS, initBytes, 2);
	for(int i = 0; i< 5000; i++){}
	I2C_SendData(I2C1, SLAVE_ADDRESS, &startBytes, 1);
}

// Read Chuk value function
int readChuk(void) {

	// Conversion data
	I2C_SendData(I2C1, SLAVE_ADDRESS, &zeroByte, 1);

	// Receive data
	for(int i = 0; i< 5000; i++){}
	I2C_ReceiveData(I2C1, 0xA5, Data_Receive, 6);
	
	// X-axis value
	uint8_t x = Data_Receive[0];
	
	// Return 1 if left and -1 if right
	if(x < 0x70){
		return 1;
	}
	if(x > 0x90){
		return -1;
	}
	
	else return 0;
}




