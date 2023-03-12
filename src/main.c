/*
 * Loadboot.c
 *
 * Created: 2022-12-27 14:20:06
 * Author : Mikae
 */ 

#define F_CPU 8000000UL
#define USART_BAUDRATE 2400
#define BAUD_PRESCALE (((( F_CPU / 16) + (USART_BAUDRATE / 2)) / (USART_BAUDRATE)) - 1)

#include <avr/io.h>
#include <avr/boot.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include "Variable_defs.h"
#include "UART.h"
#include "HEX_DECODING.h"


static int UART_putchar(char tecken, FILE *stream);
static FILE mystdout = FDEV_SETUP_STREAM(UART_putchar,NULL,_FDEV_SETUP_WRITE);
static int UART_putchar(char tecken, FILE *stream)
{

	while(!(UCSR0A & (1<<UDRE0)) )
	{
	}
	UDR0 = tecken;
	return 0;
}




void boot_program_page (uint32_t page, uint8_t *buf)
{	
	//Page size = 128 words/256 bytes
	uint16_t i;
	uint8_t sreg;
	

	// Disable interrupts.

	sreg = SREG;
	cli();

	
	boot_page_erase (page);
	boot_spm_busy_wait ();      // Wait until the memory is erased.

	for (i=0; i<SPM_PAGESIZE; i+=2)
	{
		// Set up little-endian word.

		uint16_t w = *buf++;
		w += (*buf++) << 8;

		boot_page_fill (page + i, w);
	}

	boot_page_write (page);     // Store buffer in flash page.
	boot_spm_busy_wait();       // Wait until the memory is written.

	// Reenable RWW-section again. We need this if we want to jump back
	// to the application after bootloading.

	boot_rww_enable ();

	// Re-enable interrupts (if they were ever enabled).

	SREG = sreg;
}

int main(void)
{	
	uint8_t Test = 0;
	uint16_t  Page_number = 0;
	uint8_t Data_loop = 0;
	uint8_t Data_buffer[SPM_PAGESIZE] = {0xFF}; //SPM PAGESIZE = 256
	// ASCII Buffertar
	uint8_t Rec_len_ascii[2] = {0};
	uint8_t Data_ascii[2] = {0};
	uint8_t Load_offset_ascii[4] = {0};
	uint8_t Rec_type_ascii[2] = {0};
	//Binära värden
	uint8_t Rec_len_bin = 0;
	uint16_t Load_offset_bin = 0;
	uint16_t Byte_count = 0;
	uint8_t Rec_type_bin = 0;
	
	DDRA = 0x00;
	UART_init();
	stdout = &mystdout;
	sei();
	
	
	while(PINA != 1 && PINA != 2)
	{
		//Ligger och väntar på input från knappar
	}
	
	if(PINA == 1)
	{	
		//Hoppar till applikationskod
		printf("PINA 1");
		asm("jmp 0");
		
	}
	else if(PINA == 2)
	{	
		
		printf("PINA2");
		//KOLLA EFTER : FÖR START AV RAD
		do 
		{
			Data_loop = 0;
			
			while(UART_GET() != 58)
			{
				
			}
			//RECLEN
			
			for(int i = 0 ; i < 2 ; i++)
			{
				Rec_len_ascii[i] = UART_GET();
				HEX_ASCII_TO_VALUE(&Rec_len_ascii[i]);
			}
			Rec_len_bin = (Rec_len_ascii[0]<<4) + Rec_len_ascii[1];
			
			//LOAD OFFSET
			for(int i = 0 ; i < 4 ; i++)
			{
				Load_offset_ascii[i] = UART_GET();
				HEX_ASCII_TO_VALUE(&Load_offset_ascii[i]);
			}
			Load_offset_bin = (Load_offset_ascii[0]<<12)+(Load_offset_ascii[1]<<8)+(Load_offset_ascii[2]<<4) + Load_offset_ascii[3];
			//rectype
			for(int i = 0; i < 2 ; i++)
			{
				Rec_type_ascii[i] = UART_GET();
				HEX_ASCII_TO_VALUE(&Rec_type_ascii[i]);
			}
			Rec_type_bin = (Rec_type_ascii[0]<<4) + Rec_type_ascii[1];
			//data
			while(Data_loop < Rec_len_bin && Rec_type_bin != 1)
			{	
				for(int i = 0; i < 2 ; i++)
				{
					Data_ascii[i] = UART_GET();
					HEX_ASCII_TO_VALUE(&Data_ascii[i]);
				}
				
				Data_buffer[Byte_count] = (Data_ascii[0]<<4) + Data_ascii[1];
				Data_loop++;
				Byte_count++;
			}
				
			
			
			//Kasta bort checksum
			for(int i = 0; i < 2 ; i++)
			{
				Test = UART_GET();
			}
			
			if(Byte_count+16 > SPM_PAGESIZE)
			{
				
				for(int i = Byte_count; i < SPM_PAGESIZE ; i++)
				{
					Data_buffer[i] = 0xFF;
				}
				
				
				boot_program_page(Page_number, Data_buffer);
				
				Page_number = Page_number+256;
				Byte_count = 0;
				
				
			}
			
		} while ((Rec_type_bin != 1));
		
		/*
		
		//PRINTAR ENDAST FÖR TEST/////////////
		for(int i = 0 ; i < 2 ; i++)
		{
			printf("Rec_bin[%d] %d\r",i,Rec_len_ascii[i]);
		}
			printf("Rec len %d\r", Rec_len_bin);
		for(int i = 0 ; i < 4 ; i++)
		{
			printf("Load_bin[%d] %d\r",i,Load_offset_ascii[i]);
		}
			printf("Load_bin_ihoplagd %d\r", Load_offset_bin);
		for(int i = 0 ; i < 2 ; i++)
		{
			printf("Rec_bin[%d] %d\r",i,Rec_type_ascii[i]);
		}
		printf("Rec type %d\r", Rec_type_bin);
		
		for(int i = 0; i < Byte_count ; i++)
		{
			printf("Instr_buff[%d] = %d\r",i,Data_buffer[i]);
			printf("Byte count %d ", Byte_count);
		}
		*/
			
		for(int i = Byte_count; i < SPM_PAGESIZE ; i++)
		{
			Data_buffer[i] = 0xFF;
		}
		printf("Page number %d", Page_number);
		boot_program_page(Page_number, Data_buffer);
		printf("Byte count %d ", Byte_count);
		asm("jmp 0");
	}
		
}	

/*
for(int j = 0; j < 2 ; j++)
{
	
	Data_ascii[j] = UART_GET();
	HEX_ASCII_TO_VALUE(&Data_ascii[j]);
	
	if(j == 1)
	{
		Instr_buffer[Byte_count] = (Data_ascii[0]>>4)+Data_ascii[1];
		Byte_count++;
		
	}
}
*/
/*
for(int i = 0; i < (Rec_len_bin*2) ; i++)
{
	
	Data_ascii_buffer[Byte_count] = UART_GET();
	HEX_ASCII_TO_VALUE(&Data_ascii_buffer[Byte_count]);
	Byte_count++;
	
}
*/