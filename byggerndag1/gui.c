/*
 * gui.c
 *
 * Created: 15.09.2026 15:23:36
 *  Author: alexanmf
 */ 

#include "oled.h"
#include "spi.h"
#include "gui.h"


gui_state init_gui(void) {
	
	gui_state gui;
	
	gui.line = 4;
	gui.col = 4;
	
	return gui;
	
}

void update_gui(gui_state *gui) {
	
		spi_select(0);
		spi_write(0x03);
		
		uint8_t x  = spi_read();
		uint8_t y  = spi_read();
		
		uint8_t z  = spi_read();
		
		printf(" x value %d \n \r", x);
		
		printf(" y value %d \n \r", y);
		
		printf(" y value %d \n \r", z);
		
		spi_deselect();
		
		if (y > 170 ) {
			gui->line += 1;
			
			oled_clear();
			printf("%d", gui->line);
			oled_go_to_line(gui->line);

				for (uint8_t i = 0; i < 128; i++) {
					
					oled_write_data(0xFF);
				
			}
		}
		
		else if (y < 60 ) {
			gui->line -= 1;
			
			oled_clear();
	
printf("%d", gui->line);
oled_go_to_line(gui->line);
				for (uint8_t i = 0; i < 128; i++) {
					
					oled_write_data(0xFF);
				}
			
		}
		if(x > 170) {
			gui->col += 1;
			oled_go_to_column(gui->col);
			oled_write_char('g');
		}
		
		printf("%d",gui->line );
		
}