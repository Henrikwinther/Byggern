/*
 * can.c
 *
 * Created: 28.09.2026
 *  Author: alexanmf
 */

#define F_CPU 4915200UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "can.h"
#include "mcp2515.h"
#include "spi.h"
#include "adc.h"
#include "gui.h"


/* Reset the controller and configure its bit timing and normal mode. */
void can_init(void)
{
    mcp2515_reset();
	_delay_ms(10);
	
	mcp2515_write(0x28, 0x03);  // set cnf3 to 00000011 (ps2 = 4tq)
	
	mcp2515_write(0x29, 0xAC); // set cnf2 to 10101100 (prseg = 5tq, phseg1 = 6tq, blt = 1)
	
	mcp2515_write(0x2A, 0x01);  // brp = 1 and sjw = 1
	
    mcp2515_write(0x0F, 0x00); // Set CANCTRL to normal mode
		
}

/* Load a standard identifier and payload into the transmit buffer. */
void can_send_message(can_message_t *msg) {

    mcp2515_write(0x31, ((msg->id) >> 3) & 0xFF );
    mcp2515_write(0x32, ((msg->id) << 5) & 0xE0 );
    mcp2515_write(0x35, msg->length);

    for (uint8_t i = 0; i < msg->length; i++)
    {
        mcp2515_write(0x36 + i, msg->data[i]);
    }

    mcp2515_request_to_send();
}

/* Report whether the first receive buffer contains a message. */
uint8_t can_check_rx(void)
{
	return (mcp2515_read(0x2C) & 0x01) != 0;
}

/* Decode the identifier, length, and payload from the receive buffer. */
can_message_t can_receive_message(void)
{

        can_message_t received_message;

        received_message.id = 0;

        // Read standard CAN ID
        received_message.id |= ((uint16_t)mcp2515_read(0x61)) << 3;
        received_message.id |= (mcp2515_read(0x62) >> 5) & 0x07;

        // Read DLC
        received_message.length = mcp2515_read(0x65) & 0x0F;

        // Read data
        for (uint8_t i = 0; i < received_message.length; i++)
        {
            received_message.data[i] = mcp2515_read(0x66 + i);
			

        }
		
		mcp2515_bit_modify(0x2C, 0x01, 0x00); // Clear RXoIF such that RXB0 can take the next msg
		
        return received_message;
    
}

/* Read, normalize, and transmit joystick coordinates and button states. */
void can_send_joystick_position(pos_t joystick_center) {
	
	can_message_t msg;

	msg.id = 100;
	msg.length = 4;

	pos_t joystick_pos = pos_read(joystick_center);
	
	// protect noisy values
	if (joystick_pos.x >= -3 && joystick_pos.x <= 3) {
		joystick_pos.x = 0;
	}

	if (joystick_pos.y >= -3 && joystick_pos.y <= 3) {
		joystick_pos.y = 0;
	}
	
	if (joystick_pos.x >= 97) {
		joystick_pos.x = 100;
	}

	if (joystick_pos.x <= -97 ) {
		joystick_pos.x = -100;
	}
	

	Buttons buttons = buttons_read();

	
	
	msg.data[0] = (uint8_t)joystick_pos.x;
	msg.data[1] = (uint8_t)joystick_pos.y;
	msg.data[2] = buttons.left;
	msg.data[3] = buttons.right;
	
// 	printf("x : %d \r \n",(int8_t) msg.data[0]);
// 	printf("y : %d \r \n",(int8_t) msg.data[1]);

	can_send_message(&msg);
}