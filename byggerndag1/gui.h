/*
 * gui.h
 *
 * Created: 15.09.2026 15:23:05
 *  Author: alexanmf
 */ 


#ifndef GUI_H_
#define GUI_H_

#include <stdint.h>
#include "adc.h"

typedef struct GuiMenu GuiMenu;

struct GuiMenu {
	const char **items;
	uint8_t item_count;
	const GuiMenu *parent;
};

typedef struct {
	const GuiMenu *menu;
	uint8_t position;
	JoystickDirection last_direction;
	uint8_t button_was_pressed;
} gui_state;

void gui_init(gui_state *gui, const GuiMenu *menu);
void gui_enter_menu(gui_state *gui, const GuiMenu *menu);
int8_t gui_update(gui_state *gui, pos_t joystick_center);

typedef struct {
	uint8_t right;
	uint8_t left;
	uint8_t nav;
}Buttons;

uint8_t joystick_pressed(void);
Buttons buttons_read(void);


#endif /* GUI_H_ */