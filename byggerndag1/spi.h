/*
 * spi.h
 *
 * Created: 15.09.2026 10:55:05
 *  Author: alexanmf
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>

typedef enum {
	IO_BOARD_MCU = 0,
	OLED_DISPLAY,
	MCP_2515,
	SPI_NUM_SLAVES
} spi_slave_t;


void spi_init(void);
void spi_select(spi_slave_t slave);
void spi_deselect(void);



uint8_t spi_read(void);
void spi_write(uint8_t data);





#endif /* SPI_H_ */