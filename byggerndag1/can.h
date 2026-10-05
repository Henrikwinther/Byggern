/*
 * can.h
 *
 * Created: 28.09.2026
 *  Author: alexanmf
 */

#ifndef CAN_H_
#define CAN_H_

#include <stdint.h>

typedef struct {
    uint16_t id;
    uint8_t data[8];
    uint8_t length;
} can_message_t;

void can_init(void);
void can_send_message(can_message_t *msg);
can_message_t can_receive_message(void);

#endif /* CAN_H_ */
