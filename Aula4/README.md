<h1>Aula 3</h1>

Esta clase consiste en entender las diferentes configuraciones de las entradas y salidas de los GPIOs de la tarjeta NUCLEO F767ZI.

<h2>GPIO</h2>

Los GPIOs pueden ser configurado en diferentes modos:

- Entrada flotante
- Entrada Pull Up
- Entrada Pull Down
- Análogo
- Salida Push Pull
- Salida Open Drain
- Función alternante (Timers, PWM, SPI, I2C, USART, etc.)

A través de los siguientes registros: 

- GPIOx_MODER -> Registro de configuración del pin (análogos/in/out/AF)
- GPIOx_OTYPER -> Registro de selección del tipo de salida (Push-Pull o Open-Drain)
- GPIOx_PUPDR -> Registro de selección de Pull Up o Pull Down
- GPIOx_OSPEEDER -> Registro de configuración de velocidad del pin
- GPIOx_IDR -> Registro de salida de datos en el pin (Leer)
- GPIOx_ODR	-> Registro de salida de datos en el pin (Escribir)

<div align="center">
<img src="image.png" alt="Estructura pin I/O"/>
<br>
<figcaption>Fuente: Manual de usuario</figcaption>
</div>

<h3>Entradas digitales</h3>

- El búfer de salida está desactivado
- La entrada de disparo Schmitt- Trigger está activada
- Las resistencias pull-up y pull-down se activan en función del valor en el registro GPIOx_PUPDR
- El dato presente en el pin de I/O se muestrea en el registro de entrada de datos cada ciclo de reloj AHB
- Un acceso de lectura al registro de entrada de dator proporciona el estado de I/O

<div align="center">
<img src="image-1.png" alt="Entradas digitales"/>
<br>
<figcaption>Fuente: Manual de usuario</figcaption>
</div>

<h3>Salidas digitales</h3>

- El búfer de salida está habilitado:
	– Modo Open Drain: un "0" en el registro de salida activa el N-MOS, mientras que un "1" en el registro de salida deja el puerto en alta impedancia (el P-MOS nunca se activa)
	– Modo Push-Pull: un "0" en el registro de salida activa el N-MOS, mientras que un "1" en el registro de salida activa el P-MOS
• La entrada de disparo Schmitt-Trigger está activada
• Las resistencias Pull-Up y Pull-Down se activan en función del valor en el registro GPIOx_PUPDR
• Los datos presentes en el pin de I/O se muestrean en el registro de datos de entrada cada ciclo de reloj AHB
• Un acceso de lectura al registro de entrada de datos obtiene el estado de I/O
• Un acceso de lectura al registro de salida de datos obtiene el último valor escrito

<div align="center">
<img src="image-2.png" alt="Salidas digitales"/>
<br>
<figcaption>Fuente: Manual de usuario</figcaption>
</div>

<h3>Función alternante</h3>

• El búfer de salida se puede configurar en modo de Open-Drain o Push-Pull
• El búfer de salida es accionado por las señales procedentes del periférico (habilitación del transmisor y datos)
• La entrada de disparo Schmitt-Trigger está activada
• Las resistencias Pull-Up y Pull-Down se activan o no dependiendo del valor en el registro GPIOx_PUPDR
• Los datos presentes en el pin de E/S se muestrean en el registro de datos de entrada cada ciclo de reloj AHB
• Un acceso de lectura al registro de entrada de datos obtiene el estado de I/O

<div align="center">
<img src="image-3.png" alt="Función alternante"/>
<br>
<figcaption>Fuente: Manual de usuario</figcaption>
</div>

<h3>Análogo</h3>

• El búfer de salida está desactivado
• La entrada de disparo Schmitt-Trigger está desactivada, lo que proporciona un consumo cero para cada valor analógico del pin de I/O. La salida del disparador Schmitt se fuerza a un valor constante (0).
• Las resistencias Pull-Up y Pull-Down están desactivadas por hardware
• El acceso de lectura al registro de entrada de datos obtiene el valor "0"

<div align="center">
<img src="image-4.png" alt="Análogo"/>
<br>
<figcaption>Fuente: Manual de usuario</figcaption>
</div>



<h2>GPIO</h2>










```cpp
//Ejemplo Hola mundo con led de usuario y botón de usuario de la tarjeta
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
	RCC->AHB1ENR |= (1<<0); //Enable the GPIOA clock (user led LD2 is connected to PA_5)
	RCC->AHB1ENR |= (1<<2); //Enable the GPIOC clock (user push button B1 is connected to PC_13)
	//Step 2
	GPIOA->MODER &= ~(0b11<<10); //clear (00) pin PA_5(bits 11:10) and set as Input (00) for default 
	GPIOC->MODER &= ~(0b11<<26); //clear (00) pin PC_13(bits 27:26) and set as Input (00) for default 
	GPIOA->MODER |= (1<<10); //pin PA_5(bits 11:10) as Output (01)
	//Step 3
	GPIOA->OTYPER &= ~(1<<5);  // bit 5=0 --> Output push pull (HIGH or LOW)
	GPIOA->OSPEEDR |= ((1<<11)|(1<<10));//(0b11<<10)  // Pin PA_5 (bits 11:10) as High Speed (11)
	GPIOC->OSPEEDR |= ((1<<27)|(1<<26));//(0b11<<26)  // Pin PC_13 (bits 27:26) as High Speed (11)
	GPIOA->PUPDR &= ~(0b11<<10); //~((1<<11)|(1<<10)) // Pin PA_5 (bits 11:10) are 0:0 --> no pull up or pull down
	GPIOC->PUPDR &= ~(0b11<<26); //~((1<<27)|(1<<26)) // Pin PC_13 (bits 27:26) are 0:0 --> no pull up or pull down
	GPIOC->PUPDR |= (1<<26); // Pin PC_13 (bits 27:26) are 0:1 --> pull up
	
	while(1){
		if(((GPIOC->IDR & (1<<13)) >> 13) == 0){//Read PC13 pin
			//GPIOA->BSRR |= (1<<5); // Set the Pin PA_5
			GPIOA->ODR |= 1<<5; // Set the Pin PA_5
			Delay(1000000);
			//GPIOA->BSRR |= (1<<21); // Reset the Pin PA_5
			GPIOA->ODR &= ~(1<<5); // Reset the Pin PA_5
			Delay(1000000);
		}
	}
}

```