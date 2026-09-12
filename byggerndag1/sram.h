/*
 * sram.h
 *
 * Created: 07.09.2026 13:45:19
 *  Author: alexanmf
 */ 


#ifndef SRAM_H_
#define SRAM_H_

#include <stdint.h>

void xmem_init(void);
void xmem_write ( uint8_t data , uint16_t addr );
uint8_t xmem_read ( uint16_t addr );
void SRAM_test(void);





#endif /* SRAM_H_ */