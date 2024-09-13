<h1>Aula 16</h1>

Esta clase consiste en comprender los TIMERS (temporizadores) y utilizarlos en la tarjeta NUCLEO STM32F767ZI

<h2>Temporizadores</h2>

<h3>Ejemplo</h3>

```c++
//Ejemplo ADC
//Fabián Barrera Prieto
//Universidad ECCI
//STM32F767ZIT6U
//operation 'or' (|) for set bit and operation 'and' (&) for clear bit

#include <stdio.h>
#include "stm32f7xx.h"
#include <string.h>

```

```c++
//Ejemplo Hola mundo con led de usuario de la tarjeta
//Fabián Barrera Prieto
//Universidad ECCI
//STM32F767ZIT6U
//operation 'or' (|) for set bit and operation 'and' (&) for clear bit

#include <stdio.h>
#include "stm32f7xx.h"

int main(){

	RCC->AHB1ENR |= (1<<1); //Enable the GPIOB clock (user led LD1 is connected to PB0)
	//RCC->AHB1ENR |= (1<<2); //Enable the GPIOC clock (user push button B1 is connected to PC13)

	GPIOB->MODER &= ~(0b11<<0); //clear (00) pin PB0(bits 1:0) and set as Input (00) for default 
	GPIOB->MODER |= (1<<0); //pin PB0(bits 1:0) as Output (01)
	//GPIOC->MODER &= ~(0b11<<26); //clear (00) pin PC13(bits 27:26) and set as Input (00) for default 

	GPIOB->OTYPER &= ~(1<<0);  // clear (0) pin PB0 (bit 0) --> Output push pull (HIGH or LOW)
	GPIOB->OSPEEDR |= ((1<<1)|(1<<0));//(0b11<<0)  // Pin PB0 (bits 1:0) as Very High Speed (11)
	//GPIOC->OSPEEDR |= ((1<<27)|(1<<26));//(0b11<<26)  // Pin PC13 (bits 27:26) as Very High Speed (11)
	GPIOB->PUPDR &= ~(0b11<<0); //~((1<<1)|(1<<0)) // Pin PB0 (bits 1:0) are 0:0 --> no pull up or pull down
	//GPIOC->PUPDR &= ~(0b11<<26); //~((1<<27)|(1<<26)) // Pin PC13 (bits 27:26) are 0:0 --> no pull up or pull down
	//GPIOC->PUPDR |= (1<<27); // Pin PC_13 (bits 27:26) are 1:0 --> pull down for default

	while(1){
	//if(((GPIOC->IDR & (1<<13)) >> 13) == 1){//Read PC13 pin
		for (int t=0;t<1000000;t++){
			//GPIOB->BSRR |= (1<<0); // Set the Pin PB0
			GPIOB->ODR |= 1<<0; // Set the Pin PB0
		}
		for (int t=0;t<1000000;t++){
			//GPIOB->BSRR |= (1<<16); // Reset the Pin PB0
			GPIOB->ODR &= ~(1<<0); // Reset the Pin PB0
		}
	//}
	}
}
```