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
#include "adc.h"
#include "spi.h"
#include "oled.h"
#include "gui.h"

int main(void)
{
	sei();
	
	
 	USART_Init(31);
 	
 	xmem_init();
 	
 	fdevopen(USART_Transmit,USART_Receive);
 	//SRAM_test();
	 
	 init_adc();
	
 	pos_t joystick_center = pos_calibrate();
	 
	spi_init();
	 
	 
	spi_select(0);
	spi_write(0x06);
	spi_write(4);
	spi_write(10);
	spi_deselect();
	
	init_oled();
	
	gui_state gui = init_gui();
	 
    /* Replace with your application code */
    while (1) 
    {
		
		_delay_ms(50);

// 		pos_t pos = pos_read(joystick_center);
// 		
// 		printf("X : %d | Y : %d\n\r", pos.x, pos.y);
		update_gui(&gui);

    }
}

