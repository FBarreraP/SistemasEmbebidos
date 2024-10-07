<h1>Aula 22</h1>

Esta clase consiste en comprender el PWM (Modulación por Ancho de Pulso) y utilizarlo en la tarjeta NUCLEO STM32F767ZI

<h2>PWM</h2>

Una modulación por ancho de pulso (PWM) se caracteriza por una señal que tiene una frecuencia determinada, la cual se compone por un tiempo en alto (duty cycle) y/o un tiempo en bajo. Son utilizados para variar la intensidad de luz de leds, variar velocidad de motores DC, variar la posición angular de servomotores, etc.

<div align="center">
<img src="PWMLed.gif" alt="PWM led"/>
<br>
<figcaption>Fuente: https://exploreembedded.com/wiki/LPC1768:_PWM</figcaption>
</div>

<div align="center">
<img src="PWMServo.gif" alt="PWM servo"/>
<br>
<figcaption>Fuente: https://www.gie.com.my/shop.php?action=robotics/motors/ldx335</figcaption>
</div>

La variación de una señal PWM consiste la variación desde 0% hasta 100% en el ciclo útil de dicha señal, por tanto, es posible obtener diferentes valores de salida de voltaje de manera proporcional al valor máximo de voltaje.

<div align="center">
<img src="image.png" alt="PWMs"/>
<br>
<figcaption>Fuente: https://arduinokitproject.com/l298n-dc-stepper-driver-arduino-tutorial/</figcaption>
</div>

<h2>PWM en el STM32F767ZI</h2>

Los TIMERs 1, 8, 2, 3, 4, 5, 9, 10, 11, 12, 13 y 14
Cada uno de estos puede generar hasta 4 PWMs a través de los registros CCR1, CCR2, CCR3 y CCR4

<div align="center">
<img src="image-1.png" alt="modo PWM STM32F767ZI"/>
<br>
<figcaption>Fuente: Manual de referencia</figcaption>
</div>



<h3>Ejemplo</h3>

```c
//Ejemplo PWM
//Fabián Barrera Prieto
//Universidad ECCI
//STM32F767ZIT6U
//operation 'or' (|) for set bit and operation 'and' (&) for clear bit

    RCC->APB1ENR |= (1<<0); //TIM1 clock enable 
    TIM1->PSC = 4; //Prescale factor 25 for 20ms of time
    TIM1->ARR = 63999; // Maximum count value for 20ms of time
    TIM1->CR1 |= (1<<0); // Enable Counting
    TIM1->CCMR1 |= (0b110<<12)|(0b110<<4); //Set PWM mode on CH2 and CH1
    TIM1->CCMR2 |= (0b110<<12)|(0b110<<4); //Set PWM mode on CH4 and CH3
    TIM1->CCER |= (1<<12)|(1<<8)|(1<<4)|(1<<0);//Enable channels CH4, CH3, CH2, CH1 as outputs 
    TIM1->EGR |= (1<<0); //Enable Event
```