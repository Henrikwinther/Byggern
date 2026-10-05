/*
 * mcp2515.h
 *
 * Created: 28.09.2026
 *  Author: alexanmf
 */

#ifndef MCP2515_H_
#define MCP2515_H_

#include <stdint.h>

uint8_t mcp2515_read(uint8_t address);
void mcp2515_write(uint8_t address, uint8_t data);
void mcp2515_request_to_send(void);
uint8_t mcp2515_read_status(void);
void mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data);
void mcp2515_reset(void);
uint8_t mcp2515_rx_status(void);

#endif /* MCP2515_H_ */
