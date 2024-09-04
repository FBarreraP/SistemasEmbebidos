//Ejemplo Interrupciones con led de usuario de la tarjeta
//Fabián Barrera Prieto
//Universidad ECCI
//STM32F767ZIT6U
//operation 'or' (|) for set bit and operation 'and' (&) for clear bit

#include <stdio.h>
#include "stm32f7xx.h"

uint8_t cont = 0;
uint32_t a, b, c;

extern "C"{
	void EXTI15_10_IRQHandler(void){
		EXTI->PR |= 1; //Down flag
		if(((GPIOC->IDR & (1<<13)) >> 13) == 1){//Read PC13 pin
			cont += 1;
		}
	}
}

int main(){
	//GPIOs
	RCC->AHB1ENR |= ((1<<1)|(1<<2)); 
	
	GPIOB->MODER &= ~(0b11<<0);
	GPIOB->MODER |= (1<<0); 
	GPIOB->MODER &= ~(0b11<<14);
	GPIOB->MODER |= (1<<14);
	GPIOC->MODER &= ~(0b11<<26);
	
	GPIOB->OTYPER &= ~(1<<0);  
	GPIOB->OTYPER &= ~(1<<7);
	GPIOB->OSPEEDR |= ((1<<1)|(1<<0));
	GPIOB->OSPEEDR |= ((1<<15)|(1<<14));
	GPIOC->OSPEEDR |= ((1<<27)|(1<<26));
	GPIOB->PUPDR &= ~(0b11<<0);
	GPIOB->PUPDR &= ~(0b11<<14);
	GPIOC->PUPDR &= ~(0b11<<26);
	GPIOC->PUPDR |= (1<<27);
	
	//Interrupt
	RCC->APB2ENR |= (1<<14); //Enable the SYSCFG clock (EXTI)
	SYSCFG->EXTICR[3] &= ~(0b1111<<4); //Clear bits [7:0] (Px13)
	SYSCFG->EXTICR[3] |= (1<<5); //Select the pin 13 on port C like interrupt [7:0]=0010 (PC13)
	EXTI->IMR |= (1<<13); //Enable the interrupt on pin 13
	EXTI->RTSR |= (1<<13); //Select rising edge for interrupt
	NVIC_EnableIRQ(EXTI15_10_IRQn); //Enable the NVIC module
		
	while(1){
      for (int t=0;t<1000000;t++){
				GPIOB->ODR |= 1<<0;
			}
			for (int t=0;t<1000000;t++){
				GPIOB->ODR &= ~(1<<0);
			}
			if(cont == 5){
				cont = 0;
				a = (~(GPIOB->ODR&(1<<7)));
				b = (GPIOB->ODR|(1<<7));
				c = a&b;
				GPIOB->ODR = c;
			}
	}
}