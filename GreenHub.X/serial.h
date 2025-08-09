#ifndef SERIAL_H
#define	SERIAL_H

#include<p30fxxxx.h>
#include <stdbool.h>

#define BUFFER_LEN 16
extern volatile char uart_rx_buffer[BUFFER_LEN];
extern volatile int uart_rx_index;
extern volatile bool uart_command_ready;


void configure_uart1(void);
void __attribute__((__interrupt__, __auto_psv__)) _U1RXInterrupt(void);
void WriteUART1(unsigned int data);
void WriteUART1dec2string(unsigned int data);
void serial_print_string(register const char *str);

#endif	/* SERIAL_H */

