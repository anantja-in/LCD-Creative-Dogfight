#define BIT(x)	(1 << (x))
#define CHECKBIT(x,b) (x&b)		//Checks bit status
#define SETBIT(x,b) x|=b;		//Sets the particular bit
#define CLEARBIT(x,b) x&=~b;		//Sets the particular bit
#define TOGGLEBIT(x,b) x^=b;		//Toggles the particular bit


/*	LCD.h
*
*	This header files have implementation of some common finctions of the LCD.
*	Include this header file in your C code. Call the lcd_init() function before
	calling any other functions to initialize the lcd.


	lcd_init() ----- Call the lcd_init() function before calling any other functions
			  to initialize the lcd.
	lcd_cmd() ------ This function is used to give any command instructions to the
			  LCD. For e.g lcd_cmd(0x01) will give the clear command.
	lcd_char() ----- This function will display a single character on the LCD display.
			  For example lcd_char(0x61) will display A. Again lcd_char('b')
			  will display b.
	lcd_string()---- This function will display a string. An example of this would be
			  like lcd_string("This is AVR")
	lcd_showvalue()- This will show a 3-digit decimal value on the LCD. For example if
			  we give lcd_showvalue(0xFF) then 255 will be displayed.
	lcd_gotoxy1()--- Will set the cursor at a particular position on LINE1 of the LCD.
			  So lcd_gotoxy1(3) will set the cursor at the 3+1=4th column in
			  the 1st line of the LCD.
	lcd_gotoxy2()--- Will set the cursor at a particular position on LINE1 of the LCD.
			  So lcd_gotoxy2(0) will set the cursor at the 0+1=1st column in
			  the 2nd line of the LCD.

	lcd_exit() ----- You may call this function after you are over with your LCD.While
			  calling the lcd functions, there would be some changes to the
			  PORTC. This restores the original PORTC configuration before
			  calling the lcd_init() function. This is not absolutely necessary.
			  You may or may not use this function.
*
*
*
*/



#define LINE1	0x80
#define LINE2	0xC0
unsigned char k=0,j=0;
unsigned char save=0;

void lcd_init(void);
void lcd_cmd(unsigned char cmd);
void toggleE(void);
void dely_ns(unsigned char tim);
void lcd_char(unsigned char single);
void lcd_string(unsigned char str[32]);
void lcd_showvalue(unsigned char num);
void lcd_gotoxy1(unsigned char pos);
void lcd_gotoxy2(unsigned char pos);
void lcd_exit(void);
void WaitMs(unsigned int ms);

void lcd_init(void)	//init the lcd
{	
	save=PORTC;
	DDRC=0xFF;
	DDRB=0xFF;
	WaitMs(15);
	CLEARBIT(PORTC,BIT(0)+BIT(1)+BIT(2));
	PORTC=0x20;
	lcd_cmd(0x01);
	lcd_cmd(0x0F);
	lcd_cmd(0x02);
	lcd_cmd(LINE1);
	
}

void toggleE(void)
{
SETBIT(PORTC,BIT(2));
dely_ns(230);
CLEARBIT(PORTC,BIT(2));
}

void dely_ns(unsigned char tim)
{
	for(j=0;j<tim;j++)
	{
		for(k=0;k<10;k++)
		{};
	}
}

void lcd_cmd(unsigned char cmd)
{
	CLEARBIT(PORTC,BIT(0)+BIT(1)+BIT(2));
	PORTC=((cmd&0xF0));			
	toggleE();
	PORTC=(((cmd&(0x0F))*16));	
	toggleE();
}


void lcd_char(unsigned char single)
{
	PORTC=0x01;
	CLEARBIT(PORTC,BIT(0)+BIT(2));
	
	PORTC=(single&0xF0)|0x01;						
	toggleE();
	PORTC=((single&(0x0F))*16)|0x01;
	toggleE();
}

void lcd_string(unsigned char str[32])
{
	unsigned char k=0;
	PORTC=0x01;
	CLEARBIT(PORTC,BIT(0)+BIT(2));
	while(str[k]!='\0')	//Till null character is encountered
		{
			if(k==16)
				lcd_cmd(LINE2);
			PORTC=((str[k])&0xF0)|0x01;
			toggleE();
			PORTC=((str[k]&(0x0F))*16)|0x01;
			toggleE();
			k++;
		}
}


void lcd_showvalue(unsigned char num)	//prints the decimal 3digit value of num
{
	unsigned char H=0,T=0,O=0;
	H=num/100;
	T=(num - (H*100))/10;
	O=(num - (H*100) - (T*10));
	
	lcd_char(H+48);
	lcd_char(T+48);
	lcd_char(O+48);
}

void lcd_gotoxy1(unsigned char pos)
{
	lcd_cmd(LINE1+pos);
}

void lcd_gotoxy2(unsigned char pos)
{
	lcd_cmd(LINE2+pos);
}
		

void lcd_exit(void)
{
	PORTC=save;
}

/* waits (pauses) for ms milliseconds (assumes clock at 16MHz) */
void WaitMs(unsigned int ms)
{
	int i;

	while (ms-- > 0)
	{
		/* 16380 (16k) clock cycles for 1ms; each time through loop
		   is 5 cycles (for loop control + nop) */
		for (i = 0; i < 3276; ++i)
			asm("nop");
	}
}
