//Ejemplo Hola mundo con led de usuario de la tarjeta
//Fabián Barrera Prieto
//Universidad ECCI
//STM32F411RET6U
//operation 'or' (|) for set bit and operation 'and' (&) for clear bit

#include <stdio.h>
#include "stm32f4xx.h"

void Delay (uint32_t time)
{
	//while (time--);  
	for (int t=0;t<time;t++);

}

int main(){
	
	/************** STEPS TO FOLLOW *****************
	1. Enable GPIOA clock
	2. Set the PIN D13 as output
	3. Configure the output mode i.e state, speed, and pull
	************************************************/
	//Reference manual chip
	//Step 1
	RCC->AHB1ENR |= (1<<0);  // Enable the GPIOA clock (user led LD2 is connected to PA_5)
	//Step 2
	GPIOA->MODER &= ~(0b11<<10); //clear (00) pin PA_5(bits 7:6)  
	GPIOA->MODER |= (1<<10); //pin PA_5(bits 11:10) as Output (01)
	//Step 3
	GPIOA->OTYPER &= ~(1<<5);  // bit 5=0 --> Output push pull (HIGH or LOW)
	GPIOA->OSPEEDR |= ((1<<11)|(1<<10));//(0b11<<10)  // Pin PA_5 (bits 11:10) as High Speed (11)
	GPIOA->PUPDR &= ~(0b11<<10); //~((1<<11)|(1<<10)) // Pin PA_5 (bits 11:10) are 0:0 --> no pull up or pull down
	
	while(1){
		//GPIOA->BSRR |= (1<<5); // Set the Pin PA_5
		GPIOA->ODR |= 1<<5; // Set the Pin PA_5
		Delay(1000000);
		//GPIOA->BSRR |= (1<<21); // Reset the Pin PA_5
		GPIOA->ODR &= ~(1<<5); // Reset the Pin PA_5
		Delay(1000000);
	}
}
