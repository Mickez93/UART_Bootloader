/*
 * Variable_defs.c
 *
 * Created: 2022-12-27 15:26:50
 *  Author: Mikae
 */ 
#include <inttypes.h>
#include <avr/io.h>

uint8_t Instr_buffer[SPM_PAGESIZE] = {0};
uint16_t Byte_count = 0;
