/*
 * byggerndag1.c
 *
 * Created: 01.09.2026 11:46:06
 * Author : henrikwp
 */ 
#define F_CPU 4915200UL
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include <avr/interrupt.h>
#include <stdio.h>

int main(void)
{
	sei();
	
	USART_Init(31);
	
	int x = 45;
    /* Replace with your application code */
    while (1) 
    {
  		_delay_ms(500);

 		fdevopen(USART_Transmit,USART_Receive);
		 
 		printf("x = %d \r \n",x);
		
    }
}

