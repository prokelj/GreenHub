#include "serial.h"

volatile char uart_rx_buffer[BUFFER_LEN];
volatile int uart_rx_index = 0;
volatile bool uart_command_ready = false;


void configure_uart1(void)
{
    U1BRG= 0x0015;//ovim odredjujemo baudrate - na terminalu 9600

    U1MODEbits.ALTIO=0;//biramo koje pinove koristimo za komunikaciju osnovne ili alternativne(RF2 i RF3 SE MORAJU PODESITI)

    IEC0bits.U1RXIE=1;//omogucavamo rx1 interupt

    U1STA&=0xfffc;

    U1MODEbits.UARTEN=1;//ukljucujemo ovaj modul

    U1STAbits.UTXEN=1;//ukljucujemo predaju
}

void __attribute__((__interrupt__, __auto_psv__)) _U1RXInterrupt(void)
{
    IFS0bits.U1RXIF = 0;

    char received = U1RXREG;

    if (!uart_command_ready) {
        if (received == '\n' || received == '\r') {
            uart_rx_buffer[uart_rx_index] = '\0';  // Null-terminate
            uart_rx_index = 0;
            uart_command_ready = true;
        } else if (uart_rx_index < BUFFER_LEN - 1) {
            uart_rx_buffer[uart_rx_index++] = received;
        } else {
            uart_rx_index = 0; //za overflow  
        }
    }
}


void WriteUART1(unsigned int data)
{
	  while(!U1STAbits.TRMT);

    if(U1MODEbits.PDSEL == 3)
        U1TXREG = data;
    else
        U1TXREG = data & 0xFF;
}

void WriteUART1dec2string(unsigned int data)
{
    char buffer[5]; // 4 cifre + null terminator
    buffer[0] = (data / 1000) % 10 + '0';
    buffer[1] = (data / 100) % 10 + '0';
    buffer[2] = (data / 10) % 10 + '0';
    buffer[3] = (data % 10) + '0';
    buffer[4] = '\0'; 
    
    int i;
    // ispis svakog znaka
    for (i = 0; i < 4; i++) {
        WriteUART1(buffer[i]);
    }
}



void serial_print_string(register const char *str) //fja za ispis stringa
{
    while ((*str) != 0) {
        WriteUART1(*str);
        str++;
    }
}


