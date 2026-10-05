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
	
	
	
	can_message_t can_msg;
	can_msg.id = 123;
	can_msg.length = 4;
	can_msg.data[0] = 0xAA;
	can_msg.data[1] = 0xBB;
	can_msg.data[2] = 0x77;
	can_msg.data[3] = 0x22;
	

	while (1) {
		
		
		can_send_message(&can_msg);
		
		/*_delay_ms(10);*/
		
		can_message_t msg_received = {0};
		if (mcp2515_rx_status() & 0x40)
		{
			msg_received = can_receive_message();
			printf(" message data2 received: %x \n \r", (msg_received.data[2]));
			printf(" message data3 received: %x \n \r", (msg_received.data[3]));
		
		}
		else {
			printf(" no can message \n \r");
		}


		int8_t selected;

		selected = gui_update(&gui, joystick_center);
		if (selected < 0) {
			continue;
		}

		gui_handle_selection(&gui, selected);
		
 	}

}

