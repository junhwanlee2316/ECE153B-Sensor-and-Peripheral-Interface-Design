/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Junhwan Lee, Taeho Kim
 * Section: Tuesday
 * Project
*/

#include "Stepper.h"

void GPIO_Init(void){   
    // Enable the clock for GPIO port C.
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

    // Configure pins 5, 6, 8, and 9 as output.
    GPIOC->MODER &= ~(GPIO_MODER_MODE5 + GPIO_MODER_MODE6 + GPIO_MODER_MODE8 + GPIO_MODER_MODE9);
    GPIOC->MODER |= GPIO_MODER_MODE5_0 + GPIO_MODER_MODE6_0 + GPIO_MODER_MODE8_0 + GPIO_MODER_MODE9_0;

    // Set the output speed of the pins to fast.
    GPIOC->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED5 + GPIO_OSPEEDR_OSPEED6 + GPIO_OSPEEDR_OSPEED8 + GPIO_OSPEEDR_OSPEED9);
    GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED5_1 + GPIO_OSPEEDR_OSPEED6_1 + GPIO_OSPEEDR_OSPEED8_1 + GPIO_OSPEEDR_OSPEED9_1;

    // Set the output type of the pins to push-pull.
    GPIOC->OTYPER &= ~(GPIO_OTYPER_OT5 + GPIO_OTYPER_OT6 + GPIO_OTYPER_OT8 + GPIO_OTYPER_OT9);

    // Set the pins to no pull-up, no pull-down.
    GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD5 + GPIO_PUPDR_PUPD6 + GPIO_PUPDR_PUPD8 + GPIO_PUPDR_PUPD9);
}

#define DELAY 60000 // delay between steps of the sequences

#define A GPIO_ODR_OD5
#define A_NOT GPIO_ODR_OD6
#define B GPIO_ODR_OD8
#define B_NOT GPIO_ODR_OD9


void Full_Stepping_Clockwise(int delay){
    // Step 1
    GPIOC->ODR &= ~(A|A_NOT|B|B_NOT);       // Reset
    GPIOC->ODR |= A;     
    for (int i = 0; i < delay; ++i) {}

    // Step 2 
    GPIOC->ODR &= ~(A|A_NOT|B|B_NOT);       // Reset
    GPIOC->ODR |= B;     
    for (int i = 0; i < delay; ++i) {}

    // Step 3
    GPIOC->ODR &= ~(A|A_NOT|B|B_NOT);       // Reset
    GPIOC->ODR |= A_NOT;     // Set pin 6
    for (int i = 0; i < delay; ++i) {}

    // Step 4
    GPIOC->ODR &= ~(A|A_NOT|B|B_NOT);       // Reset
    GPIOC->ODR |= B_NOT;     // Set pin 9
    for (int i = 0; i < delay; ++i) {}
}

void Full_Stepping_CounterClockwise(int delay){
    // Step 4
    GPIOC->ODR &= ~(A|A_NOT|B|B_NOT);       // Reset
    GPIOC->ODR |= B_NOT;     // Set pin 9
    for (int i = 0; i < delay; ++i) {}
    // Step 3
    GPIOC->ODR &= ~(A|A_NOT|B|B_NOT);       // Reset
    GPIOC->ODR |= A_NOT;     // Set pin 6
    for (int i = 0; i < delay; ++i) {}
    // Step 2 
    GPIOC->ODR &= ~(A|A_NOT|B|B_NOT);       // Reset
    GPIOC->ODR |= B;     // Set pin 8
    for (int i = 0; i < delay; ++i) {}
    // Step 1
    GPIOC->ODR &= ~(A|A_NOT|B|B_NOT);       // Reset
    GPIOC->ODR |= A;     // Set pin 5
    for (int i = 0; i < delay; ++i) {}
}

// Stop the motor
void stopMotor(void){
    GPIOC->ODR &= ~(A|B|B_NOT|A_NOT);
}

// Initalize the motor
void initStepperMotor(void){
    GPIO_Init();
}