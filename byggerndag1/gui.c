#include "oled.h"
#include "spi.h"
#include "gui.h"
#define F_CPU 4915200UL
#include <avr/io.h>
#include <util/delay.h>

static void draw_menu(const gui_state *gui)
{
	oled_clear();

	for (uint8_t i = 0; i < gui->menu->item_count; i++) {
		oled_go_to_line(7 - i);
		oled_go_to_column(42);
		if (i == gui->position) {
			oled_write_char('>');
		} else {
			oled_write_char(' ');
		}
		oled_go_to_column(50);
		oled_printf(gui->menu->items[i]);
	}
	
}

void gui_init(gui_state *gui, const GuiMenu *menu)
{
	gui->menu = menu;
	gui->position = 0;
	gui->last_direction = NEUTRAL;
	gui->button_was_pressed = 0;
	draw_menu(gui);
}

void gui_enter_menu(gui_state *gui, const GuiMenu *menu)
{
	gui->menu = menu;
	gui->position = 0;
	gui->last_direction = NEUTRAL;
	draw_menu(gui);
}


int8_t gui_update(gui_state *gui, pos_t joystick_center)
{
	JoystickDirection direction = joystick_direction(joystick_center);
	int8_t selected = -1;
	uint8_t pressed = joystick_pressed() != 0;

	if (direction != gui->last_direction) {
		if (direction == UP &&gui->position > 0) {
			gui->position--;
			draw_menu(gui);
		}
		if (direction == DOWN &&  gui->position + 1 < gui->menu->item_count ) {
			gui->position++;
			
			draw_menu(gui);
		}
	}

	gui->last_direction = direction;

	if (pressed && !gui->button_was_pressed) {
		selected = (int8_t)gui->position;
	}
	gui->button_was_pressed = pressed;
	
	Buttons buttons = buttons_read();
	
	if(buttons.right !=0){
		printf("Right buttons : %d\n\r", buttons.right);
		
	}
	if(buttons.left !=0){
		printf("Left buttons : %d\n\r", buttons.left);
		
	}
	
	

	return selected;
}

uint8_t joystick_pressed(void)
{
	uint8_t button;

	spi_select(0);
	
	spi_write(0x03);
	_delay_us(50);
	spi_read();
	
	spi_read();
	
	button = spi_read();
	spi_deselect();

	return button;
}

Buttons buttons_read(void)
{
	Buttons buttons;

	spi_select(0);
			
	spi_write(0x04);
	_delay_us(50); // minimum delay to acquire right value
	buttons.right = spi_read();
			
 	buttons.left = spi_read();
 	buttons.nav = spi_read();
	spi_deselect();

	if (buttons.left == 0xFF) {
		buttons.left = 0x00;
	}

	return buttons;
}


