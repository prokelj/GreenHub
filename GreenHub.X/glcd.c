#include "glcd.h"

unsigned char kursorX=0;
unsigned char kursorY=0;
unsigned char strana=0;

void SetRST(unsigned char vrednost)
{
	if (vrednost)
		RF5_1
	else
		RF5_0
}

void SetE(unsigned char vrednost)
{
	if (vrednost)
		RF4_1
	else
		RF4_0
}

void SetRW(unsigned char vrednost)
{
	if (vrednost)
		RF1_1
	else
		RF1_0
}

void SetRS(unsigned char vrednost)
{
	if (vrednost)
		RF0_1
	else
		RF0_0
}

void SetCS2(unsigned char vrednost)
{
	if (vrednost)
		RB5_1
	else
		RB5_0
}

void SetCS1(unsigned char vrednost)
{
	if (vrednost)
		RB4_1
	else
		RB4_0
}


void init_glcd_pins(void)
{
	TRISFbits.TRISF5=0;//LCD_RST izlaz
	TRISFbits.TRISF4=0;//LCD_E izlaz
	TRISFbits.TRISF1=0;//LCD_RW izlaz
	TRISFbits.TRISF0=0;//LCD_DI izlaz
	ADPCFGbits.PCFG5=1;
	TRISBbits.TRISB5=0;//LCD_CS2 izlaz				
	ADPCFGbits.PCFG4=1;
	TRISBbits.TRISB4=0;//LCD_CS1 izlaz
					
	
	//LCD_DATA	P2//RB0 RB1 RB2 RB3 RD0 RD1 RD2 RD3
	ADPCFGbits.PCFG0=1;
	TRISBbits.TRISB0=0;//RB0
	ADPCFGbits.PCFG1=1;
	TRISBbits.TRISB1=0;//RB1
	ADPCFGbits.PCFG2=1;
	TRISBbits.TRISB2=0;//RB2
	ADPCFGbits.PCFG3=1;
	TRISBbits.TRISB3=0;//RB3

	TRISDbits.TRISD0=0;//RD0
	TRISDbits.TRISD1=0;//RD1
	TRISDbits.TRISD2=0;//RD2
	TRISDbits.TRISD3=0;//RD3
	
}

void ConfigureLcdData(unsigned char direction)
{
	//LCD_DATA	P2//RB0 RB1 RB2 RB3 RD0 RD1 RD2 RD3
	if(direction==output)
	{
		//LCD_DATA OUTPUT
		TRISDbits.TRISD0=output;//RD0
		TRISDbits.TRISD1=output;//RD1
		TRISDbits.TRISD2=output;//RD2
		TRISDbits.TRISD3=output;//RD3
		TRISBbits.TRISB3=output;//RB3
		TRISBbits.TRISB2=output;//RB2
		TRISBbits.TRISB1=output;//RB1
		TRISBbits.TRISB0=output;//RB0
	}
	if (direction==input)
	{
		//LCD_DATA INPUT
		TRISDbits.TRISD0=input;//RD0
		TRISDbits.TRISD1=input;//RD1
		TRISDbits.TRISD2=input;//RD2
		TRISDbits.TRISD3=input;//RD3
		TRISBbits.TRISB3=input;//RB3
		TRISBbits.TRISB2=input;//RB2
		TRISBbits.TRISB1=input;//RB1
		TRISBbits.TRISB0=input;//RB0
	}

}

void SetLcdData(unsigned char vrednost)
{
	unsigned char pomocna,pomocna2;
	//LCD_DATA	lsb RB0 RB1 RB2 RB3 RD0 RD1 RD2 RD3 msb
	pomocna=vrednost;
	LATD&=0xfff0;
	LATB&=0xfff0;
	pomocna2=pomocna&0x0f;
	LATB|=pomocna2;
	pomocna=pomocna/16;
	LATD|=pomocna;
}

unsigned char ReadLcdData(void)
{
	//LCD_DATA	P2//RB0 RB1 RB2 RB3 RD0 RD1 RD2 RD3
	unsigned char pomocna,vrednost;
	pomocna=PORTD;
	pomocna=pomocna*16;
	vrednost=PORTB;
	vrednost&=0x0f;
	vrednost+=pomocna;
	return vrednost;
	
}

void strobe_data(void)
{
	SetE(1);
	SetE(0);
}

void LcdInstructionWrite (unsigned char u8Instruction)
{
   	SetRS(0);       	/* Instruction mode */
  	SetRW(0);       	/* Write mode */
  	SetLcdData(u8Instruction);/* outbyte */
	strobe_data();
}


/*-------------------------------------------------------------------------------
Tempo for the LCD timing
	pause()
-------------------------------------------------------------------------------*/
void LcdDelay(unsigned int u32Duration)
{
	unsigned int u32Delay;
	for (u32Delay=0; u32Delay<(u32Duration); u32Delay++);
}

/*-------------------------------------------------------------------------------
Wait as long as the LCD is busy
	LcdWaitBusy();
-------------------------------------------------------------------------------*/
void LcdWaitBusy (void)
{
	unsigned char temp;
	SetRS(0);       	/* Instruction mode */
  	SetRW(1);       	/* Read mode */
  	
	ConfigureLcdData(input);		/* set LCD_DATA port in input mode */
	SetE(1); 
	temp=ReadLcdData();
	SetE(0); 
	
	while (temp & 0x80) 
	{
			temp=ReadLcdData();
	}

}
/*--------------------------------------------------------------------------------
GOTO X
---------------------------------------------------------------------------------*/

void GoToX(unsigned char x)
{
	if (x<64)
	{
		LcdSelectSide(LEFT);
		LcdInstructionWrite (Y_ADRESS | x);
		strana=LEFT;
	}
	else if(x<128)
	{
		LcdSelectSide(RIGHT);
		LcdInstructionWrite (Y_ADRESS | (x-64));
		strana=RIGHT;
	}
	kursorX=x;
}

/*--------------------------------------------------------------------------------
GOTO Y
---------------------------------------------------------------------------------*/

void GoToY(unsigned char y)
{
	if(y<8)
	{
		LcdInstructionWrite (PAGE_ADRESS | y);
		kursorY=y;
	}
}

/*--------------------------------------------------------------------------------
GOTO XY
---------------------------------------------------------------------------------*/

void GoToXY(unsigned char x,unsigned char y)
{
	GoToX(x);
	GoToY(y);
}

/*---------------------------------------------------
koristimo za skroolovanje ekrana tj tu polovinu koju postavimo startline
ona celu polovinu translira gore dole da joj je vrh na liniji koju postavimo
prosledjujemo vrednost 0-64
------------------------------------------------*/

void LcdSelectStartline(unsigned char startna_linija)
{
	LcdInstructionWrite (START_LINE | startna_linija);
}

/*-------------------------------------------------------------------------------
Select the side of the LCD
	LcdSelectSide(U8 u8LcdSide)
		u8LcdSide = LEFT or RIGHT
-------------------------------------------------------------------------------*/

void LcdSelectSide(unsigned char u8LcdSide)
{
	SetRS(0);  	
	if(u8LcdSide == RIGHT)
	{
    		SetRW(0);
   		SetCS1(1); 	
        SetCS2(0);
	  	LcdInstructionWrite(DISPLAY_ON);
		strana=RIGHT;
   	}
	else
	{
		SetRW(0);
   		SetCS1(0); 	
   		SetCS2(1);
	 	LcdInstructionWrite(DISPLAY_ON);     
    		strana=LEFT;
 	}
}


/*-------------------------------------------------------------------------------
Read data from LCD
	U8 = LcdDataRead();
-------------------------------------------------------------------------------*/

unsigned char LcdDataRead (void)
{
	unsigned char temp;
	//LcdWaitBusy ();		// wait until LCD not busy --------------------------------------------------------------------
 	 ConfigureLcdData(input);
	SetRS(1);      	/* Data mode */
	SetRW(1);      	/* read mode */
	//LcdDelay(5);
	strobe_data();//ne znma zasto ali trazi ovo sto je u sustino dupli strobe da bi dobro iscitao bajt..
	//LcdDelay(5);
	SetE(1);
	//LcdDelay(5);
	temp=ReadLcdData();
	SetE(0);
	ConfigureLcdData(output);
	//LcdDelay(5);
	SetRS(0);      	/* Data mode */
	SetRW(0);      	/* read mode */
	return temp;/* return the data read */
}
/*-------------------------------------------------------------------------------
LCD Initialization
	GLCD_LcdINIT()
-------------------------------------------------------------------------------*/
void configure_glcd(void)	
{
	SetLcdData(0);
	SetRS(0);
  	SetRW(0);
  	SetE(0);
  	SetCS1(0);
  	SetCS2(0);
  	
  	SetRST(1);
  	LcdDelay(10);
  	SetRST(0);
  	LcdDelay(10);
  	SetRST(1);
  	  	
  	LcdSelectSide(LEFT);
  	LcdInstructionWrite(DISPLAY_OFF); /* Display OFF */
  	LcdInstructionWrite(START_LINE);
  	LcdInstructionWrite(PAGE_ADRESS);
  	LcdInstructionWrite(Y_ADRESS);
  	LcdInstructionWrite(DISPLAY_ON); /* Display ON */
  	
  	LcdSelectSide(RIGHT);
  	LcdInstructionWrite(DISPLAY_OFF); /* Display OFF */
  	LcdInstructionWrite(START_LINE);
  	LcdInstructionWrite(PAGE_ADRESS);
  	LcdInstructionWrite(Y_ADRESS);
  	LcdInstructionWrite(DISPLAY_ON); /* Display ON */
  	
}  	



/*-------------------------------------------------------------------------------
Send datas to the LCD
	LcdDataWrite (U8 u8Data)
		u8Data = data to send to the LCD
-------------------------------------------------------------------------------*/
void LcdDataWrite (unsigned char u8Data)
{
	//LcdWaitBusy ();		// wait until LCD not busy --------------------------------------------------------------------
	SetRS(1);      	/* Data mode */
	SetRW(0);      	/* write mode */
	SetLcdData(u8Data);	/* outbyte */
	strobe_data();

}


/*brisanje ekrana---------------*/

void GLCD_ClrScr (void)
{
	unsigned char q,vert;

	for (vert = 0; vert< 8; vert++) 
    	{
		GoToXY(0,vert);
		for (q = 0; q < 64; q++)
		{
			LcdDataWrite(0);
		}
		GoToXY(64,vert);
		for (q = 0; q < 64; q++)
		{
			LcdDataWrite(0);
		}
	}
}

/*ispuni ceo ekran ekrana---------------*/

void GLCD_FillScr (void)
{
unsigned char q,vert;

	for (vert = 0; vert< 8; vert++) 
    	{
		GoToXY(0,vert);
		for (q = 0; q < 64; q++)
		{
			LcdDataWrite(0xff);
		}
		GoToXY(64,vert);
		for (q = 0; q < 64; q++)
		{
			LcdDataWrite(0xff);
		}
	}
}


/*-------------------------------------------------------------------------------
Send an image to the LCD
	GLCD_DisplayPicture (U8 *au8PictureData)
		au8PictureData = contains datas for picture
-------------------------------------------------------------------------------*/
//void GLCD_DisplayPicture (unsigned char  *au8PictureData)
void GLCD_DisplayPictureConst (unsigned char const *slika)
{

unsigned char q,vert;

	for (vert = 0; vert< 8; vert++) /* loop on the 8 pages */
    	{
      		GoToXY(0,vert);
      		for (q = 0; q < 64; q++)
		{
			LcdDataWrite(slika[vert*128+q]);
		}
		GoToXY(64,vert);
      		for (q = 0; q < 64; q++)
		{
			LcdDataWrite(slika[vert*128+q+64]);
		}
	}
}

void GLCD_DisplayPicture (unsigned char *slika)
{
unsigned char q,vert;

	for (vert = 0; vert< 8; vert++) /* loop on the 8 pages */
    	{
      		GoToXY(0,vert);
      		for (q = 0; q < 64; q++)
		{
			LcdDataWrite(slika[vert*128+q]);
		}
		GoToXY(64,vert);
      		for (q = 0; q < 64; q++)
		{
			LcdDataWrite(slika[vert*128+q+64]);
		}
	}
}

void update_percentage_display(unsigned char* niz, int percentage) {
    //ne bi se nikad trebalo desiti
    if (percentage < 0) percentage = 0;
    if (percentage > 100) percentage = 100;

    int startIndex = 782; // pocetni pixel
    int total_length = 100; //0-100% -> 100 pixela

    int i;
    for (i = 0; i < total_length; ++i) //prolazimo kroz 100 horizontalnih pixela
    {
        if (i < percentage) //crtamo pixele dok ne dodjemo do zeljenog procenta
        {
            niz[startIndex + i] = 255; // crtamo 0-7 vertikalnih pixela
        } else //ostatak pixela brisemo
        {
            niz[startIndex + i] = 0; // brisemo 0-7 vertikalnih pixela
        }
    }
    GLCD_DisplayPicture(perc_bar); //update-ujemo bar
}


