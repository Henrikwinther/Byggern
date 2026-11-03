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
#include "can.h"
#include "mcp2515.h"

int main(void)
{
	sei();
	
	
 	USART_Init(31);
	 
	 xmem_init();
	 
	 fdevopen(USART_Transmit, USART_Receive);
 	
	init_adc();
	spi_init();
	oled_init();
	pos_t joystick_center = pos_calibrate();
	gui_state gui;
	

	gui_init(&gui, &root_menu);
	
 	can_init();
	
	

	while (1) {
		
 		
		 can_send_joystick_position(joystick_center);

		_delay_ms(10);

		int8_t selected;

		selected = gui_update(&gui, joystick_center);
		if (selected < 0) {
			continue;
		}

		gui_handle_selection(&gui, selected);
		
 	}

}
