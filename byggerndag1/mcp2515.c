/*
 * mcp2515.c
 *
 * Created: 28.09.2026
 *  Author: alexanmf
 */

#include "mcp2515.h"
#include "spi.h"
#include <avr/io.h>

uint8_t mcp2515_read(uint8_t address)
{
    uint8_t data;
    spi_select(MCP_2515);
    spi_write(0x03); 
    spi_write(address);
    data = spi_read();
    spi_deselect();

    return data;
}

void mcp2515_write(uint8_t address, uint8_t data)
{
    spi_select(MCP_2515);
    spi_write(0x02); 
    spi_write(address);
    spi_write(data);
    spi_deselect();
}

void mcp2515_request_to_send(void)
{
    spi_select(MCP_2515);
    spi_write(0x81); 
    spi_deselect();
}

uint8_t mcp2515_read_status(void)
{
    uint8_t status;
    spi_select(MCP_2515);
    spi_write(0xA0); 
    status = spi_read();
    spi_deselect();

    return status;
}

void mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data)
{
    spi_select(MCP_2515);
    spi_write(0x05); 
    spi_write(address);
    spi_write(mask);
    spi_write(data);
    spi_deselect();
}

void mcp2515_reset(void)
{
    spi_select(MCP_2515);
    spi_write(0xC0); 
    spi_deselect();
}

uint8_t mcp2515_rx_status(void)
{
    uint8_t rx_status;

    spi_select(MCP_2515);
    spi_write(0xB0);
    rx_status = spi_read();
    spi_deselect();

    return rx_status;
}
