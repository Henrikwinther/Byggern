/*
 * uart.h
 *
 * Created: 01.09.2026 14:00:59
 *  Author: henrikwp
 */ 


#ifndef UART_H_
#define UART_H_

void USART_Init(unsigned int ubrr);
int USART_Transmit(unsigned char data);
int USART_Receive(void);



#endif /* UART_H_ */