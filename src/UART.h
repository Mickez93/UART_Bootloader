/*
 * UART.h
 *
 * Created: 2022-12-23 10:43:18
 *  Author: Mikae
 */ 
#include <inttypes.h>

#ifndef UART_H_
#define UART_H_
#define F_CPU 8000000UL
#define USART_BAUDRATE 2400
#define BAUD_PRESCALE (((( F_CPU / 16) + (USART_BAUDRATE / 2)) / (USART_BAUDRATE)) - 1)

void UART_init(void);
void UART_SEND(uint8_t Range);
uint8_t UART_GET(void);



#endif /* UART_H_ */