<h1>Aula 25</h1>

Esta clase consiste en comprender el I2C () y utilizarlo en la tarjeta NUCLEO STM32F767ZI

<h2>I2C</h2>



<h3>Ejemplo</h3>

```c
//Ejemplo I2C
//Fabián Barrera Prieto
//Universidad ECCI
//STM32F767ZIT6U
//operation 'or' (|) for set bit and operation 'and' (&) for clear bit

#include <stdio.h>
#include "stm32f7xx.h"
#include <string.h>

//MPU6050
#define MPU6500_address 0x68 // Endereço da MPU6500 (giroscópio e acelerômetro)

// Escalas do girôscopio
#define    GYRO_FULL_SCALE_250_DPS    0x00 // SCALE_250 (°/s) = 0 (0x00 = 000|00|000)
#define    GYRO_FULL_SCALE_500_DPS    0x08 // SCALE_500 (°/s) = 1 (0x08 = 000|01|000)
#define    GYRO_FULL_SCALE_1000_DPS   0x10 // SCALE_1000 (°/s) = 2 (0x10 = 000|10|000)
#define    GYRO_FULL_SCALE_2000_DPS   0x18 // SCALE_2000 (°/s) = 3 (0x18 = 000|11|000)

// Escalas do acelerômetro
#define    ACC_FULL_SCALE_2_G        0x00 // SCALE_2_G (g) = 0 (0x00 = 000|00|000)
#define    ACC_FULL_SCALE_4_G        0x08 // SCALE_4_G (g) = 1 (0x08 = 000|01|000)
#define    ACC_FULL_SCALE_8_G        0x10 // SCALE_8_G (g) = 2 (0x10 = 000|10|000)
#define    ACC_FULL_SCALE_16_G       0x18 // SCALE_16_G (g) = 3 (0x18 = 000|11|000)

// Escalas de conversao (As taxas de conversão são especificadas na documentação)
#define SENSITIVITY_ACCEL     2.0/32768.0             // Valor de conversão do Acelerômetro (g/LSB) para 2g e 16 bits de comprimento da palavra
#define SENSITIVITY_GYRO      250.0/32768.0           // Valor de conversão do Girôscopio ((°/s)/LSB) para 250 °/s e 16 bits de comprimento da palavra
#define SENSITIVITY_TEMP      333.87                  // Valor de sensitividade do Termometro (Datasheet: MPU-9250 Product Specification, pag. 12)
#define TEMP_OFFSET           21                      // Valor de offset do Termometro (Datasheet: MPU-6050 Product Specification, pag. 12)

// Offsets de calibração (AQUI DEVEM IR OS VALORES DETERMINADOS EN LA CALIBRACAO PREVIA COM O CÓDIGO "calibracao.ino")
double offset_accelx = 334.0, offset_accely = -948.0, offset_accelz = 16252.0;
double offset_gyrox = 111.0, offset_gyroy = 25.0, offset_gyroz = -49.0;

// Valores "RAW" de tipo inteiro
int16_t raw_accelx, raw_accely, raw_accelz;
int16_t raw_gyrox, raw_gyroy, raw_gyroz;
int16_t raw_temp;

// Saídas calibradas
float accelx, accely, accelz;
float gyrox, gyroy, gyroz;
float temp;

uint8_t data[1];
uint8_t GirAcel[14];

uint8_t flag = 0, j, cont = 0;
int i;
unsigned char d;
char text[50], text1[60]={"TESTE DE CONEXAO PARA O GIROSCOPIO E O ACELEROMETRO \n\r"}; 
char text2[35]={"Erro de conexao com a MPU6050 \n\r"};
char text3[55]={"Opaaa. Eu nao sou a MPU6050, Quem sou eu? :S. I am:"};
char text4[40]={"Conexao bem sucedida com a MPU6050 \n\r"};
char text5[45]={"Oi, tudo joia?... Eu sou a MPU6050 XD \n\r"};
unsigned char tx_data[1];

//I2C
void ReadI2C1(uint8_t Address, uint8_t Register, uint8_t *Data, uint8_t bytes);
void WriteI2C1(uint8_t Address, uint8_t Register, uint8_t *Data, uint8_t bytes);

void Print(char *data, int n);

void SysTick_Wait(uint32_t n){
    SysTick->LOAD = n - 1; //15999
    SysTick->VAL = 0; //Clean the value of Systick counter
    while (((SysTick->CTRL & 0x00010000) >> 16) == 0); //Check the count flag until it's 1 
}

void SysTick_ms(uint32_t x){
    for (uint32_t i = 0; i < x; i++){//x ms
        SysTick_Wait(16000); //1ms
    }
}

extern "C"{
    void EXTI15_10_IRQHandler(void){
        EXTI->PR |= 1; //Down flag
        if(((GPIOC->IDR & (1<<13)) >> 13) == 1){
            flag = 1;
        }
    }

    void USART3_IRQHandler(void){ //Receive interrupt
        if(((USART3->ISR & 0x20) >> 5) == 1){//Received data is ready to be read (flag RXNE = 1)
            d = USART3->RDR;//Read the USART receive Data 
            if(d == 'H'){
                flag = 1;
            }
        }
    }
}

int main(){
    //----------------------------------------------------------------------------
    //                        					GPIOs
    //----------------------------------------------------------------------------
    RCC->AHB1ENR |= ((1<<1)|(1<<2)); 

    GPIOB->MODER &= ~((0b11<<0)|(0b11<<14));
    GPIOB->MODER |= ((1<<0)|(1<<14)); 
    GPIOC->MODER &= ~(0b11<<26);

    GPIOB->OTYPER &= ~((1<<0)|(1<<7));
    GPIOB->OSPEEDR |= (((1<<1)|(1<<0)|(1<<15)|(1<<14)));
    GPIOC->OSPEEDR |= ((1<<27)|(1<<26));
    GPIOB->PUPDR &= ~((0b11<<0)|(0b11<<14));
    GPIOC->PUPDR &= ~(0b11<<26);
    GPIOC->PUPDR |= (1<<27);

    //----------------------------------------------------------------------------
    //                        				Systick
    //----------------------------------------------------------------------------
    SysTick->LOAD = 0x00FFFFFF; 
    SysTick->CTRL |= (0b101);

    //----------------------------------------------------------------------------
    //                        				Interrupt
    //----------------------------------------------------------------------------
    RCC->APB2ENR |= (1<<14); 
    SYSCFG->EXTICR[3] &= ~(0b1111<<4); 
    SYSCFG->EXTICR[3] |= (1<<5); 
    EXTI->IMR |= (1<<13); 
    EXTI->RTSR |= (1<<13);
    NVIC_EnableIRQ(EXTI15_10_IRQn); 
        
    //----------------------------------------------------------------------------
    //                        					UART
    //----------------------------------------------------------------------------
    RCC->AHB1ENR |= (1<<3); 
    GPIOD->MODER |= (1<<19)|(1<<17); 
    GPIOD->AFR[1] |= (0b111<<4)|(0b111<<0); 
    RCC->APB1ENR |= (1<<18); 
    USART3->BRR = 0x683; 
    USART3->CR1 |= ((1<<5)|(0b11<<2)); 
    NVIC_EnableIRQ(USART3_IRQn);

    //----------------------------------------------------------------------------
    //                        					I2C
    //----------------------------------------------------------------------------
    RCC->AHB1ENR |= (1<<1); //Enable GPIOB clock (PB9=I2C1_SDA and PB8=I2C1_SCL)
    GPIOB->MODER |= (1<<19)|(1<<17); //Set (10) pins PB9 (bits 19:18) and PB8 (bits 17:16) as alternant function
    GPIOB->OTYPER |= (1<<9)|(1<<8); //Set (1) pin PB9 (bit 9) and pin PB8 (bit 8) as output open drain (HIGH or LOW)
    GPIOB->OSPEEDR |= (0b11<<18)|(0b11<<16); //Set (11) pin PB9 (bits 19:18) and pin PB8 (bits 17:16) as Very High Speed
    GPIOB->PUPDR|= (1<<18)|(1<<16); //Set (01) pin PB9 (bits 19:18) and pin PB8 (bits 17:16) as pull up
    GPIOB->AFR[1] |= (1<<6)|(1<<2); //Set the I2C1 (AF4) alternant function for pins PB9=I2C1_SDA (bits 7:4) and PB8=I2C1_SCL (bits 3:0)
    RCC->APB1ENR |= (1<<21); //Enable I2C1 clock
    RCC->DCKCFGR2 |= (1<<17); //Set (10) bits 17:16 as HSI clock is selected as source I2C1 clock
    //I2C1->CR1 &= ~I2C_CR1_PE; //
    I2C1->TIMINGR |= (1<<29)|(0b101<<20)|(0b1010<<16)|(0b11<<11)|(0b11001<<0);//0x205A1819; //
    I2C1->CR1 |= (1<<0);// Enable I2C1

    USART3->CR1 |= (1<<0);
    
    SysTick_ms(1000);

    //----------------------------------------------------------------------------
    //                        				MPU6050
    //----------------------------------------------------------------------------
    tx_data[0] = 0x00;	
    WriteI2C1(MPU6500_address, 0x6B, tx_data, 1); // Desativa modo de hibernação do MPU6050
    Print(text1, strlen(text1));
    //.....................................................................
    //        Quem sou eu para a MPU6050 (giroscópio e acelerômetro)
    //.....................................................................
    ReadI2C1(MPU6500_address, 0x75, data, 14);
    if (data[0] != 0x68) { // DEFAULT_REGISTER_WHO_AM_I_MPU6050 0x68
    Print(text2, strlen(text2));
    sprintf(text3,"%s %#x \n\r",data[0]);
    Print(text3, strlen(text3));
    while (1);
    }else{
        Print(text4, strlen(text4));
        Print(text5, strlen(text5));
    }
    SysTick_ms(100);
    //.....................................................................
    //        Configuracao dos sensores giroscópio e acelerômetro
    //.....................................................................
    tx_data[0] = 0x00;
    WriteI2C1(MPU6500_address, 0x1B, tx_data, 1);	
    WriteI2C1(MPU6500_address, 0x1C, tx_data, 1);	
    SysTick_ms(10);
    
    while(1){
        GPIOB->ODR |= 1<<0; 
        SysTick_ms(500);
        GPIOB->ODR &= ~(1<<0);
        SysTick_ms(500);
        if(flag == 1){
            flag = 0;
            for(i=0; i<299; i++){
                ReadI2C1(MPU6500_address, 0x3B, GirAcel, 14);
                raw_accelx = GirAcel[0]<<8 | GirAcel[1];    
                raw_accely = GirAcel[2]<<8 | GirAcel[3];
                raw_accelz = GirAcel[4]<<8 | GirAcel[5];
                raw_temp = GirAcel[6]<<8 | GirAcel[7];
                raw_gyrox = GirAcel[8]<<8 | GirAcel[9];
                raw_gyroy = GirAcel[10]<<8 | GirAcel[11];
                raw_gyroz = GirAcel[12]<<8 | GirAcel[13];
                SysTick_ms(10);	
                //Dados escalados
                accelx = raw_accelx*SENSITIVITY_ACCEL;
                accely = raw_accely*SENSITIVITY_ACCEL;
                accelz = raw_accelz*SENSITIVITY_ACCEL;
                gyrox = raw_gyrox*SENSITIVITY_GYRO;
                gyroy = raw_gyroy*SENSITIVITY_GYRO;
                gyroz = raw_gyroz*SENSITIVITY_GYRO;
                temp = (raw_temp/SENSITIVITY_TEMP)+21;
                //sprintf(text,"%d \t %d \t %d \t %d \t %d \t %d \t %d \t %d \n",i+1,raw_accelx, raw_accely, raw_accelz, raw_gyrox, raw_gyroy, raw_gyroz, raw_temp);
                sprintf(text,"%d \t %.2f \t %.2f \t %.2f \t %.2f \t %.2f \t %.2f \t %.2f \n\r",i+1,accelx, accely, accelz, gyrox, gyroy, gyroz, temp);
                for(j=0; j<strlen(text); j++){
                        USART3->TDR = text[j]; 
                        while(((USART3->ISR & 0x80) >> 7) == 0){} 
                }
                //USART3->TDR = 0x0A; 
                //while((USART3->ISR & 0x80)==0){};
                USART3->TDR = 0x0D; 
                while(((USART3->ISR & 0x80) >> 7) == 0){}
            }
        }
    }
}

void WriteI2C1(uint8_t Address, uint8_t Register, uint8_t *Data, uint8_t bytes){
    uint8_t n; // Count for data read
    
    I2C1->CR2 &= ~(0x3FF<<0);// Clear the slave address
    I2C1->CR2 |= (Address<<1);// Set the 7-bit slave address to be sent

    // i2c modo escritura
    I2C1->CR2 &= ~(1<<10);// Master requests a write transfer
    I2C1->CR2 &= ~(0xFF<<16);// Clear the number of bytes to be transmitted
    I2C1->CR2 |= ((bytes+1)<<16);// Set the number of bytes to be transmitted
    I2C1->CR2 |= (1<<25);// Set automatic end mode

    I2C1->CR2 |= (1<<13);// Generate START

    while (((I2C1->ISR) & (1<<1)) != (0b10)){}// Wait the (TXIS) Transmit interrupt status

    I2C1->TXDR = Register;// Transmit the register

    n = bytes;
    while(n>0){
        while (((I2C1->ISR) & (1<<1)) != (0b10)){}// Wait the (TXIS) Transmit interrupt status
        I2C1->TXDR = *Data;// Data to be sent
        Data++;
        n--;
    }
    while (((I2C1->ISR) & (1<<5)) != (0b100000)){}// Wait the STOPF detection flag
}

void ReadI2C1(uint8_t Address, uint8_t Register, uint8_t *Data, uint8_t bytes ) {
    
    uint8_t n; // Contador para la lectura de los bytes

    // Dirección del dispositivo
    I2C1->CR2 &= ~(0x3FF<<0);// Clear the slave address
    I2C1->CR2 |= (Address<<1);// Set the 7-bit slave address to be sent

// i2c modo escritura
    I2C1->CR2 &= ~(1<<10);// Master requests a write transfer
    I2C1->CR2 &= ~(0xFF<<16);// Clear the number of bytes to be transmitted
    I2C1->CR2 |= (1<<16);// Set the number of bytes to be transmitted
    I2C1->CR2 &= ~(1<<25);// Set software end mode

    I2C1->CR2 |= (1<<13);// Generate START
    
    while (((I2C1->ISR) & (1<<1)) != (0b10)){}// Wait the (TXIS) Transmit interrupt status

    I2C1->TXDR = Register;// Transmit the register

    while (((I2C1->ISR) & (1<<6)) != (0b1000000)){}// Wait a (TC) Transfer complete

    // i2c en modo lectura
    I2C1->CR2 |= (1<<10);// Master requests a read transfer
    I2C1->CR2 &= ~(0xFF<<16);// Clear the number of bytes to be transmitted
    I2C1->CR2 |= (bytes<<16);// Set the number of bytes to be received
    I2C1->CR2 &= ~(1<<25);// Set software end mode

    I2C1->CR2 |= (1<<13);// Generate RE-START

    n = bytes;
    while (n>0){
        while (((I2C1->ISR) & (1<<2)) != (0b100)){}// Wait (RXNE) that the received data is copied into the I2C_RXDR register
        *Data = I2C1->RXDR;// Receive the register
        Data++;
        n--;
    }

    I2C1->CR2 |= (1<<14);// I2C stop

    while (((I2C1->ISR) & (1<<5)) != (0b100000)){}// Wait the STOPF detection flag
}

void Print(char *data, int n){
    for(j=0; j<n; j++){
        USART3->TDR = *data; 
        data++;
        while(((USART3->ISR & 0x80) >> 7) == 0){} 
    }
    //USART3->TDR = 0x0A; 
    //while((USART3->ISR & 0x80)==0){};
    USART3->TDR = 0x0D; 
    while(((USART3->ISR & 0x80) >> 7) == 0){}
}
```

