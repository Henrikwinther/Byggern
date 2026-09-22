/*
 * oled.h
 *
 * Created: 15.09.2026 13:30:35
 *  Author: alexanmf
 */ 


#ifndef OLED_H_
#define OLED_H_

#include <avr/io.h>
#include "gui.h"


void oled_init(void);
extern const GuiMenu root_menu;
extern const GuiMenu settings_menu;
extern const GuiMenu difficulty_menu;
extern const GuiMenu luminosity_menu;
void oled_write_data (uint8_t data);
void oled_write_command (uint8_t command);

void oled_clear(void);
void oled_go_to_column (uint8_t col);
void oled_go_to_line(uint8_t line);
void oled_printf(const char *str);
void oled_write_char(char c);


#endif /* OLED_H_ */