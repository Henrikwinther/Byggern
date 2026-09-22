/*
 * gui.h
 *
 * Created: 15.09.2026 15:23:05
 *  Author: alexanmf
 */ 


#ifndef GUI_H_
#define GUI_H_


typedef struct {
	uint8_t line;
	uint8_t col;
} gui_state;

extern gui_state gui;


gui_state init_gui(void);
void update_gui(gui_state *gui);


#endif /* GUI_H_ */