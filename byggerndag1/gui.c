#include "oled.h"
#include "spi.h"
#include "gui.h"
#define F_CPU 4915200UL
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

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

static const GuiMenu score_menu = {
	0, 0, 0
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

static void draw_score(const gui_state *gui)
{
	char digits[5];
	uint8_t digit_count = 0;
	uint16_t score = gui->score;

	oled_clear();
	oled_go_to_line(4);
	oled_go_to_column(42);
	oled_printf("Score: ");

	do {
		digits[digit_count++] = '0' + score % 10;
		score /= 10;
	} while (score > 0);

	while (digit_count > 0) {
		oled_write_char(digits[--digit_count]);
	}
}

static void draw_menu(const gui_state *gui)
{
	oled_clear();

	if (gui->menu == &score_menu) {
		draw_score(gui);
		return;
	}

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
	gui->score = 0;
	draw_menu(gui);
}

void gui_enter_menu(gui_state *gui, const GuiMenu *menu)
{
	gui->menu = menu;
	gui->position = 0;
	gui->last_direction = NEUTRAL;
	draw_menu(gui);
}

void gui_set_score(gui_state *gui, uint16_t score)
{
	gui->score = score;
	if (gui->menu == &score_menu) {
		draw_score(gui);
	}
}

void gui_handle_selection(gui_state *gui, int8_t selected)
{
	if (gui->menu == &root_menu) {
		switch (selected) {
			case 0:
			gui_enter_menu(gui, &score_menu);
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

