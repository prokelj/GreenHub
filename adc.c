#include "adc.h"

unsigned int x, y, fotootp, mq3;

void __attribute__((__interrupt__, __auto_psv__)) _ADCInterrupt(void) {
    
    x = ADCBUF0; //0
    y = ADCBUF1; //1
    fotootp = ADCBUF2; //fotootp
    mq3 = ADCBUF3; //mq3 

    IFS0bits.ADIF = 0;
}

void init_adc_pins(void)
{
	ADPCFGbits.PCFG8=0; //x
	ADPCFGbits.PCFG9=0; //y
    ADPCFGbits.PCFG11=0; //fotootp rb11
	ADPCFGbits.PCFG12=0; //mq3 rb12
    
	TRISBbits.TRISB8=1;
	TRISBbits.TRISB9=1;
    TRISBbits.TRISB11=1;
	TRISBbits.TRISB12=1;
}

void configure_adc(void)
{  
ADCON1bits.ADSIDL=0;
ADCON1bits.FORM=0;
ADCON1bits.SSRC=7;
ADCON1bits.SAMP=1;

ADCON2bits.VCFG=7;
ADCON2bits.CSCNA=1;
ADCON2bits.SMPI=3;
ADCON2bits.BUFM=0;
ADCON2bits.ALTS=0;

ADCON3bits.SAMC=31;
ADCON3bits.ADRC=1;
ADCON3bits.ADCS=31;

ADCHSbits.CH0NB=0;
ADCHSbits.CH0NA=0;

ADCHSbits.CH0SA=0;
ADCHSbits.CH0SB=0;


ADCSSL=0b0001101100000000; //rb8 rb9 rb11 rb12
ADCON1bits.ASAM=1;

IFS0bits.ADIF=1;
IEC0bits.ADIE=1;

ADCON1bits.ADON = 1;
}


