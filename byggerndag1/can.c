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

void can_init(void)
{
    mcp2515_reset();
	_delay_ms(100);

     mcp2515_write(0x0F, 0x40); // Set CANCTRL to 01000000 ie loopback mode
		
}

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