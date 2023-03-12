/*
 * HEX_DECODING.c
 *
 * Created: 2022-12-28 11:55:34
 *  Author: Mikae
 */ 
#include "UART.h"
#include <inttypes.h>
#include "Variable_defs.h"

void HEX_rec_mark_check(void)
{
	
}

void HEX_ASCII_TO_VALUE(uint8_t* HEX_ascii)
{
	switch(*HEX_ascii)
	{
		case '0': 
			*HEX_ascii = 0x0;
			break;
		case '1':
			*HEX_ascii = 0x1;
			break;
		case '2':
			*HEX_ascii = 0x2;
			break;
		case '3':
			*HEX_ascii = 0x3;
			break;	
		case '4':
			*HEX_ascii = 0x4;
			break;	
		case '5':
			*HEX_ascii = 0x5;
			break;	
		case '6':
			*HEX_ascii = 0x6;
			break;	
		case '7':
			*HEX_ascii = 0x7;
			break;	
		case '8':
			*HEX_ascii = 0x8;
			break;	
		case '9':
			*HEX_ascii = 0x9;
			break;	
		case 'A':
			*HEX_ascii = 0xA;
			break;			
		case 'B':
			*HEX_ascii = 0xB;
			break;			
		case 'C':
			*HEX_ascii = 0xC;
			break;
		case 'D':
			*HEX_ascii = 0xD;
			break;						
		case 'E':
			*HEX_ascii = 0xE;
			break;			
		case 'F':
			*HEX_ascii = 0xF;
			break;			
		//Inga matchningar
		default:
			*HEX_ascii = 0x0;	
			
	}
}