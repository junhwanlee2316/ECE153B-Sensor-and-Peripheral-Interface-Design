/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Junhwan Lee, Taeho Kim
 * Section: Tuesday 7:00 pm
 * Lab: 6A
 */

#include "Stepper.h"

void GPIO_Init(void){   
    // [TODO]
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
#define DELAY2 500

#define A GPIO_ODR_OD5
#define A_NOT GPIO_ODR_OD6
#define B GPIO_ODR_OD8
#define B_NOT GPIO_ODR_OD9


void Full_Stepping_Clockwise(void){
    // Step 1
    GPIOC->ODR &= ~(A|A_NOT|B|B_NOT);       // Reset
    GPIOC->ODR |= A;     
    for (int i = 0; i < Delay2; ++i) {}

    // Step 2 
    GPIOC->ODR &= ~(A|A_NOT|B|B_NOT);       // Reset
    GPIOC->ODR |= B;     
    for (int i = 0; i < Delay2; ++i) {}

    // Step 3
    GPIOC->ODR &= ~(A|A_NOT|B|B_NOT);       // Reset
    GPIOC->ODR |= A_NOT;     // Set pin 6
    for (int i = 0; i < Delay2; ++i) {}

    // Step 4
    GPIOC->ODR &= ~(A|A_NOT|B|B_NOT);       // Reset
    GPIOC->ODR |= B_NOT;     // Set pin 9
    for (int i = 0; i < Delay2; ++i) {}
}

void Full_Stepping_CounterClockwise(void){
    // Step 4
    GPIOC->ODR &= ~(A|A_NOT|B|B_NOT);       // Reset
    GPIOC->ODR |= B_NOT;     // Set pin 9
    for (int i = 0; i < Delay2; ++i) {}
    // Step 3
    GPIOC->ODR &= ~(A|A_NOT|B|B_NOT);       // Reset
    GPIOC->ODR |= A_NOT;     // Set pin 6
    for (int i = 0; i < Delay2; ++i) {}
    // Step 2 
    GPIOC->ODR &= ~(A|A_NOT|B|B_NOT);       // Reset
    GPIOC->ODR |= B;     // Set pin 8
    for (int i = 0; i < Delay2; ++i) {}
    // Step 1
    GPIOC->ODR &= ~(A|A_NOT|B|B_NOT);       // Reset
    GPIOC->ODR |= A;     // Set pin 5
    for (int i = 0; i < Delay2; ++i) {}
}

void Half_Stepping_Clockwise(void){

    for(int i = 0; i < 8; i++){
        GPIOC->ODR &= ~(A|A_NOT|B|B_NOT);       // Reset
        if(i == 0) {GPIOC->ODR |= (A | B_NOT); }    
        if(i == 1) {GPIOC->ODR |= A; }
        if(i == 2){GPIOC->ODR |= (A|B);}
        if(i==3){GPIOC->ODR |= B; }
        if(i==4){GPIOC->ODR |= (A_NOT|B); }
        if(i==5){GPIOC->ODR |= A_NOT; }
        if(i==6){GPIOC->ODR |= (A_NOT|B_NOT); }
        if(i==7){GPIOC->ODR |= B_NOT; }
				for (int i = 0; i < Delay2; ++i) {}
    }
}

void Half_Stepping_CounterClockwise(void){

    for(int i = 0; i < 8; i++){
        GPIOC->ODR &= ~(A|A_NOT|B|B_NOT);       // Reset
        if(i == 0) {GPIOC->ODR |= (B_NOT); }    
        if(i == 1) {GPIOC->ODR |= (A_NOT|B_NOT); }
        if(i == 2){GPIOC->ODR |= (A_NOT);}
        if(i==3){GPIOC->ODR |= (A_NOT|B); }
        if(i==4){GPIOC->ODR |= B; }
        if(i==5){GPIOC->ODR |= A|B; }
        if(i==6){GPIOC->ODR |= (A); }
        if(i==7){GPIOC->ODR |= (A|B_NOT); }
        for (int i = 0; i < Delay2; ++i) {}
    }
}

void stopMotor(){
    GPIOC->ODR ~= &(A|B|B_NOT|A_NOT);
}

void initStepperMotor(){
    GPIO_Init();
}