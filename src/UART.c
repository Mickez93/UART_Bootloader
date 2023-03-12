#define F_CPU 8000000UL
#define USART_BAUDRATE 2400
#define BAUD_PRESCALE (((( F_CPU / 16) + (USART_BAUDRATE / 2)) / (USART_BAUDRATE)) - 1)

#include <inttypes.h>
#include <avr/io.h>
#include "Variable_defs.h"

void UART_init(void)
{
	UBRR0 = BAUD_PRESCALE; //Baud 250000
	UCSR0B = (1<<TXEN0)|(1<<RXEN0);
	UCSR0C = (1<<UCSZ00)|(1<<UCSZ01);
	
}

void UART_SEND(uint8_t Range)
{

	while(!(UCSR0A & (1<<UDRE0)) )
	{
	}
	UDR0 = Range;
	
}

uint8_t UART_GET()
{
	uint16_t timeout_counter = 0;
	
	while (!(UCSR0A & (1 << RXC0)))
	{
		timeout_counter++;
		
		// Do nothing until data have been received and is ready to be read from UDR
	}
	
		return(UDR0);

}
/*
uint8_t UART_GET()
{
	uint16_t timeout_counter = 0;
	
	while (!(UCSR0A & (1 << RXC0)) && (timeout_counter <= 1000))
	{
		timeout_counter++;
		
		// Do nothing until data have been received and is ready to be read from UDR
	}
	if(timeout_counter>1000)
	{
		return 0;
	}
	else
	{
		return(UDR0);
	}
}

*/