//Ejemplo Systick delay
//Fabián Barrera Prieto
//Universidad ECCI
//STM32F767ZIT6U
//operation 'or' (|) for set bit and operation 'and' (&) for clear bit

#include <stdio.h>
#include "stm32f7xx.h"

void SysTick_Wait(uint32_t n){
	SysTick->LOAD = n - 1; //15999 is loaded into the SysTick->VAL register when the counter is enabled
	SysTick->VAL = 0; //Clear the count flag
	while (((SysTick->CTRL & 0x00010000) >> 16) == 0); //Check the count flag until it's 1 
}

void SysTick_ms(uint32_t x){
	for (uint32_t i = 0; i < x; i++){//x ms
		SysTick_Wait(16000); //1ms
	}
}

int main(){

	RCC->AHB1ENR |= (1<<1); 
	GPIOB->MODER &= ~(0b11<<0);
	GPIOB->MODER |= (1<<0); 	
	GPIOB->OTYPER &= ~(1<<0);  
	GPIOB->OSPEEDR |= ((1<<1)|(1<<0));
	GPIOB->PUPDR &= ~(0b11<<0);
	
	//Systick
	SysTick->LOAD = 0x00FFFFFF; //Initializing with the maximum value to 24 bits and load the SysTick->VAL register when the counter is enabled
	SysTick->CTRL |= (0b101); //Clock source is processor clock (AHB) and counter enable
	
	while(1){
		GPIOB->ODR |= 1<<0; 
		SysTick_ms(1000);
		GPIOB->ODR &= ~(1<<0);
		SysTick_ms(1000);
	}
}