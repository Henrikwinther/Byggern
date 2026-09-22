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

static const char *root_items[] = {
	"Start", "Settings", "Difficulty", "Exit"
};

static const char *settings_items[] = {
	"Luminosity", "Setting 2", "Setting 3", "Back"
};

static const char *difficulty_items[] = {
	"Easy", "Medium", "Hard", "Back"
};

static const char *luminosity_items[] = {
	"Use the joystick", "Back"
};

const GuiMenu root_menu = {
	root_items, 4, 0
};

const GuiMenu settings_menu = {
	settings_items, 4, &root_menu
};

const GuiMenu difficulty_menu = {
	difficulty_items, 4, &root_menu
};

const GuiMenu luminosity_menu = {
	luminosity_items, 2, &settings_menu
};

void oled_init(void)
{
	oled_write_command(0xAF);
	oled_write_command(0xC0);
	oled_write_command(0xA1);
	oled_clear();
}

void oled_write_command (uint8_t command) {
	
	PORTB &= ~(1 << PD2);
	spi_select(1);
	spi_write(command);
	spi_deselect();
}

void oled_write_data (uint8_t data) {
	
	PORTB |= (1 << PD2);
	spi_select(1);
	spi_write(data);
	spi_deselect();
	
}



void oled_go_to_line(uint8_t line) {
	if ( line >= 0 && line < 8){
		oled_write_command(0xB0 + line);
	}
}

void oled_go_to_column (uint8_t col) {
	
	if ( col >= 0 && col < 128) {
		oled_write_command(0x00 + (col & 0x0F));
		oled_write_command(0x10 + ((col >> 4) & 0x0F));
	}
}


void oled_clear(void) {
	for (uint8_t j = 0; j < 8; j++) {
		oled_go_to_line(j);

		for (uint8_t i = 0; i < 128; i++) {
			
			oled_write_data(0x00);
		}
	}
}

uint8_t reverse_bits(uint8_t x)
{
	x = ((x & 0x55) << 1) | ((x & 0xAA) >> 1);
	x = ((x & 0x33) << 2) | ((x & 0xCC) >> 2);
	x = (x << 4) | (x >> 4);
	return x;
}

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


void oled_printf(const char *str) {
	while (*str){
		oled_write_char(*str);
		str++;
	}
}




