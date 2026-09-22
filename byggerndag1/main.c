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

static void handle_selection(gui_state *gui, int8_t selected)
{
	if (gui->menu == &root_menu) {
		switch (selected) {
			case 0:
			printf("Start selected\n\r");
			break;
			case 1:
			gui_enter_menu(gui, &settings_menu);
			break;
			case 2:
			gui_enter_menu(gui, &difficulty_menu);
			break;
			case 3:
			printf("Exit selected\n\r");
			break;
		}
		} else if (gui->menu == &settings_menu && selected == 0) {
		gui_enter_menu(gui, &luminosity_menu);
		} else if (gui->menu->parent != 0 && selected == gui->menu->item_count - 1) {
		gui_enter_menu(gui, gui->menu->parent);
	}
}

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

	while (1) {
		int8_t selected;

		_delay_ms(50);

		selected = gui_update(&gui, joystick_center);
		if (selected < 0) {
			continue;
		}

		handle_selection(&gui, selected);
	}

}

