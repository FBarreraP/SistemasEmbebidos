<h1>Aula 14</h1>

Esta clase consiste en comprender la Conversión Análogo Digital (ADC) y utilizarla en la tarjeta NUCLEO STM32F767ZI

<h2>Señal análoga</h2>

Una señal análoga se caracteriza por ser continua en función del tiempo, sin embargo es variable en amplitud y periodo (frecuencia).

<div align="center">
<img src="Imagenes/image.png" alt="Señal análoga"/>
<br>
<figcaption>Fuente: https://alejandrocbba.wordpress.com/</figcaption>
</div>

<h2>Conversión análoga a digital</h2>

Para el procesamiento de información en circuitos electrónicos digitales y sistemas embebidos (Arduino, Microcontroladores, STM32, Raspberry, FPGA, entre otros), es indispensable tener la información digital debido a la lógica booleana de los procesadores digitales.

<div align="center">
<img src="Imagenes/image-1.png" alt="Señal análoga a digital"/>
<br>
<figcaption>Fuente: </figcaption>
</div>

<h2>Conversor Análogo Digital (ADC)</h2>

Un Conversor Análogo Digital (ADC) permite convertir una señal análoga en una señal digital equivalente, donde cada valor de muestra de la señal análoga está representado por una cantidad de bits. 

<div align="center">
<img src="Imagenes/image-2.png" alt="ADC"/>
<br>
<figcaption>Fuente: https://alejandrocbba.wordpress.com/</figcaption>
</div>

<h3>Resolución ADC</h3>

<div align="center">
<img src="Imagenes/image-3.png" alt="ADC"/>
<br>
<figcaption>Fuente: https://alejandrocbba.wordpress.com/</figcaption>
</div>

$$Rango análogico = V_{máx} - V_{mín} = 3V - 0V = 3V$$

$$Resolución = \frac{Rango análogico}{2^n} = \frac{3V}{2^3} = 0.375V$$

<h2>ADC en la STM32F767ZI</h2>

El microcontrolador STM32F767ZI tiene internamente tres módulos ADC con resolución configurable de 6, 8, 10 o 12 bits. Las conversiones de análogo a digital de los diferentes canales se pueden realizar de dos modos: 1) individualmente o 2) secuencialmente sobre un grupo de entradas análogas. La ubicación de los pines análogos se encuentra en la tabla 11 del datasheet del microcontrolador STM32F767ZI.

<div align="center">
<img src="Imagenes/image-14.png" alt="Diagrama de bloque ADC"/>
<br>
<figcaption>Fuente: Manual de referencia</figcaption>
</div>

Cada uno de los tres módulos tiene la posibilidad de conectar hasta 16 pines análogos externos, 2 fuentes internas y 1 canal VBAT, pero alguno de los pines externos están compartidos entre los tres módulos. Así mismo, es posible utilizar dos o tres módulos de ADC de forma paralela.

<div align="center">
<img src="Imagenes/image-16.png" alt="Diagrama de bloque ADC1"/>
<br>
<figcaption>Fuente: Manual de referencia</figcaption>
</div>

<div align="center">
<img src="Imagenes/image-17.png" alt="Diagrama de bloque ADC2"/>
<br>
<figcaption>Fuente: Manual de referencia</figcaption>
</div>

<div align="center">
<img src="Imagenes/image-18.png" alt="Diagrama de bloque ADC3"/>
<br>
<figcaption>Fuente: Manual de referencia</figcaption>
</div>

<div align="center">
<img src="Imagenes/image-19.png" alt="Selección de canal ADC"/>
<br>
<figcaption>Fuente: Manual de referencia</figcaption>
</div>

<div align="center">
<img src="Imagenes/image-20.png" alt="Modos de conversión individual y continuo"/>
<br>
<figcaption>Fuente: Manual de referencia</figcaption>
</div>

<div align="center">
<img src="Imagenes/image-21.png" alt="Alineación de datos ADC"/>
<br>
<figcaption>Fuente: Manual de referencia</figcaption>
</div>

<div align="center">
<img src="Imagenes/image-22.png" alt="Tiempo de conversión ADC"/>
<br>
<figcaption>Fuente: Manual de referencia</figcaption>
</div>

<div align="center">
<img src="Imagenes/image-23.png" alt="Diagrama de tiempo ADC"/>
<br>
<figcaption>Fuente: Manual de referencia</figcaption>
</div>

<h3>Ejemplo</h3>

```c++

```

<div align="center">
<img src="Imagenes/image-4.png" alt="RCC_APB2ENR"/>
<br>
<figcaption>Fuente: Manual de referencia</figcaption>
</div>

<div align="center">
<img src="Imagenes/image-15.png" alt="ADC_CCR"/>
<br>
<figcaption>Fuente: Manual de referencia</figcaption>
</div>

<div align="center">
<img src="Imagenes/image-5.png" alt="ADC_CR2"/>
<br>
<figcaption>Fuente: Manual de referencia</figcaption>
</div>

<div align="center">
<img src="Imagenes/image-6.png" alt="ADC_CR1"/>
<br>
<figcaption>Fuente: Manual de referencia</figcaption>
</div>

<div align="center">
<img src="Imagenes/image-7.png" alt="ADC_SMPR1"/>
<br>
<figcaption>Fuente: Manual de referencia</figcaption>
</div>

<div align="center">
<img src="Imagenes/image-8.png" alt="ADC_SMPR2"/>
<br>
<figcaption>Fuente: Manual de referencia</figcaption>
</div>

<div align="center">
<img src="Imagenes/image-12.png" alt="ADC_SQR1"/>
<br>
<figcaption>Fuente: Manual de referencia</figcaption>
</div>

<div align="center">
<img src="Imagenes/image-13.png" alt="ADC_SQR2"/>
<br>
<figcaption>Fuente: Manual de referencia</figcaption>
</div>

<div align="center">
<img src="Imagenes/image-9.png" alt="ADC_SQR3"/>
<br>
<figcaption>Fuente: Manual de referencia</figcaption>
</div>

<div align="center">
<img src="Imagenes/image-10.png" alt="ADC_SR"/>
<br>
<figcaption>Fuente: Manual de referencia</figcaption>
</div>

<div align="center">
<img src="Imagenes/image-11.png" alt="ADC_DR"/>
<br>
<figcaption>Fuente: Manual de referencia</figcaption>
</div>