/*
 * uart.c
 *
 * Created: 01.09.2026 14:00:44
 *  Author: henrikwp
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>



#define F_CPU 4915200UL
#define BAUD 9600UL
#define MYUBRR F_CPU/16/BAUD-1

void USART_Init(unsigned int ubrr)
{
	/* Set baud rate */
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;

	/* Enable receiver and transmitter */
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);

	/* 8 data bits, 2 stop bits */
	UCSR0C = (1 << URSEL0) | (1 << USBS0) | (1 << UCSZ01) | (1 << UCSZ00);
	
	UCSR0B |= (1 << RXCIE0);

}

int USART_Transmit(unsigned char data)
{
	/* Wait for empty transmit buffer */
	while (!(UCSR0A & (1 << UDRE0)))
	;

	/* Put data into buffer, sends the data */
	UDR0 = data;
	
	return 0;
}

int USART_Receive(void)
{
	/* Wait for data to be received */
	while (!(UCSR0A & (1 << RXC0)))
	;

	/* Get and return received data from buffer */
	return UDR0;
}

ISR(USART0_RXC_vect)
{
	unsigned char data;

	data = UDR0;
	USART_Transmit(data);
}