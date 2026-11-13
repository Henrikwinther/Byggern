/*
 * oled.c
 *
 * Created: 15.09.2026 13:30:23
 *  Author: alexanmf
 */ 

#define F_CPU 4915200UL
#include <avr/io.h>
#include <util/delay.h>
#include "spi.h"
#include "oled.h"
#include "fonts.h"

/* Configure the OLED display and clear its screen. */
void oled_init(void)
{
	oled_write_command(0xAF);
	oled_write_command(0xC0);
	oled_write_command(0xA1);
	oled_clear();
}

/* Send a command byte with the display's data/command line cleared. */
void oled_write_command (uint8_t command) {
	
	PORTB &= ~(1 << PD2);
	spi_select(1);
	spi_write(command);
	spi_deselect();
}

/* Send a data byte with the display's data/command line set. */
void oled_write_data (uint8_t data) {
	
	PORTB |= (1 << PD2);
	spi_select(1);
	spi_write(data);
	spi_deselect();
	
}



/* Select one of the display's eight text pages. */
void oled_go_to_line(uint8_t line) {
	if ( line >= 0 && line < 8){
		oled_write_command(0xB0 + line);
	}
}

/* Set the display column using its low and high address commands. */
void oled_go_to_column (uint8_t col) {
	
	if ( col >= 0 && col < 128) {
		oled_write_command(0x00 + (col & 0x0F));
		oled_write_command(0x10 + ((col >> 4) & 0x0F));
	}
}


/* Fill every display page with zero-valued pixels. */
void oled_clear(void) {
	for (uint8_t j = 0; j < 8; j++) {
		oled_go_to_line(j);

		for (uint8_t i = 0; i < 128; i++) {
			
			oled_write_data(0x00);
		}
	}
}

/* Reverse bit order so glyph columns match the display orientation. */
uint8_t reverse_bits(uint8_t x)
{
	x = ((x & 0x55) << 1) | ((x & 0xAA) >> 1);
	x = ((x & 0x33) << 2) | ((x & 0xCC) >> 2);
	x = (x << 4) | (x >> 4);
	return x;
}

/* Look up and write one printable character from the small font table. */
void oled_write_char(char c)
{
	if (c < 32 || c > 126) {
		c = ' ';
	}

	uint8_t index = c - 32;

	for (uint8_t i = 0; i < 4; i++) {
		uint8_t data = pgm_read_byte(&font4[index][i]);
		oled_write_data(reverse_bits(data));
	}
}


/* Write a null-terminated string one character at a time. */
void oled_printf(const char *str) {
	while (*str){
		oled_write_char(*str);
		str++;
	}
}




