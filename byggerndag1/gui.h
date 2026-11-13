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

extern uint8_t game_started;

/* A menu stores its labels, item count, and optional parent menu. */
typedef struct GuiMenu GuiMenu;

struct GuiMenu {
	const char **items;
	uint8_t item_count;
	const GuiMenu *parent;
};

extern const GuiMenu root_menu;
extern const GuiMenu settings_menu;
extern const GuiMenu difficulty_menu;
extern const GuiMenu luminosity_menu;

/* Runtime state used to draw menus and track joystick input changes. */
typedef struct {
	const GuiMenu *menu;
	uint8_t position;
	JoystickDirection last_direction;
	uint8_t button_was_pressed;
	uint16_t score;
} gui_state;

/* Initialize the interface, update its state, and handle menu actions. */
void gui_init(gui_state *gui, const GuiMenu *menu);
void gui_enter_menu(gui_state *gui, const GuiMenu *menu);
void gui_set_score(gui_state *gui, uint16_t score);
void gui_handle_selection(gui_state *gui, int8_t selected);
int8_t gui_update(gui_state *gui, pos_t joystick_center);

/* Digital button values read from the I/O board. */
typedef struct {
	uint8_t right;
	uint8_t left;
	uint8_t nav;
}Buttons;

/* Read joystick button states from the I/O board. */
uint8_t joystick_pressed(void);
Buttons buttons_read(void);


#endif /* GUI_H_ */