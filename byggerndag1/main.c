/*
 * byggerndag1.c
 *
 * Created: 01.09.2026 11:46:06
 * Author : henrikwp
 */ 
#define F_CPU 4915200UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "uart.h"
#include "sram.h"

int main(void)
{
	sei();
	
	
 	USART_Init(31);
 	
 	xmem_init();
 	
 	fdevopen(USART_Transmit,USART_Receive);
 	//SRAM_test();
 	

	
	
	volatile uint8_t *adc = (volatile uint8_t *)0x1800; 
	
    /* Replace with your application code */
    while (1) 
    {
		 adc[0]= 1;

    }
}

