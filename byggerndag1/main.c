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
	/* Enable interrupts and initialize serial, memory, input, display, and CAN. */
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
	
	can_message_t score_msg = {0};

	/* Keep the display responsive while exchanging joystick and score messages. */
	while (1) {
		
		/* Send joystick updates after the user starts the game. */
 		if (game_started){
		 can_send_joystick_position(joystick_center);
		 }

		_delay_ms(1);

		/* Refresh the score when a CAN message arrives. */
		if (can_check_rx()) {
			score_msg = can_receive_message();
			gui_set_score(&gui, score_msg.data[0]);
			printf("%d \n \r",score_msg.data[0]);
		}

		int8_t selected;

		/* Handle menu actions only when a new selection is reported. */
		selected = gui_update(&gui, joystick_center);
		if (selected < 0) {
			continue;
		}

		gui_handle_selection(&gui, selected);
		
 	}

}
