#include<avr/io.h>
#include<avr/interrupt.h>

#include"LCD.h"

/* waits (pauses) for ms milliseconds (assumes clock at 16MHz) */
void delay_ms(unsigned int ms)
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
char arr1[16];
char arr2[16];
char line1[] = "    <     <    < <    <<<         <          <    <   <                    ";
char line2[] = "       <            <      <            <       <   <                      ";

void init() {
	int x;
	for(x=0;x<16;x++)       arr1[x]=' ';
	for(x=0;x<16;x++)       arr2[x]=' ';
	CLEARBIT(DDRB,BIT(4))
	CLEARBIT(DDRD,BIT(2))
	CLEARBIT(DDRD,BIT(3))
	arr1[1]='>';
	lcd_init();
	
}
void refresh() {
	lcd_cmd(0x01);
    char str[32];
	int x;
	int y=0;
    for(x=0;x<16;x++)		str[y++]=arr1[x];
	for(x=0;x<16;x++)		str[y++]=arr2[x];
	lcd_string(str);
}

void assign(int y) {
	int x,z;
	z=0;
	for(x=y;x<y+16 && x<60;x++)	arr1[z++]=line1[x];
	z=0;
	for(x=y;x<y+16 && x<60;x++)	arr2[z++]=line2[x];
}
int main() {


	char levelDisp[] = "Level 0";
	// MyCar is > and other cars are <
	init();	
	
	int level; // current level
	int gameOver=0;
	
	for(level=0;level<10 && gameOver==0;level++) {
		
		lcd_cmd(0x01);
		delay_ms(100);
		levelDisp[6]=48+level;
		lcd_string(levelDisp);
		delay_ms(500);
		
		int time = 0;
		int pos = level%2;
		while(time<60) {
			// Put the strings
			assign(time++);
			
			// Check for collision first
			if((arr1[1]=='<' && pos == 0) || (arr2[1]=='<' && pos == 1)) {
				arr1[1]='*';
				refresh();
				arr2[1]='*';
				arr1[2]='*';
				arr2[2]='*';
				refresh();
				delay_ms(500);
				time = 60;
				lcd_cmd(0x01);
				delay_ms(100);
				lcd_string("Game over");
				delay_ms(300);
				lcd_cmd(0x01);
				delay_ms(100);
				lcd_string("Game over");
				delay_ms(300);
				lcd_cmd(0x01);
				delay_ms(100);
				lcd_string("Game over");
				delay_ms(1000);
				gameOver=1;
			}	
			if(gameOver==0) {
				if(pos==0) arr1[1] = '>';
				else	arr2[1] = '>';
				refresh();
				// Wait for a sec, checking for input change
				delay_ms(100-(level*10));
				// Check for button pressed
				if(!CHECKBIT(PIND,BIT(2)))	pos=0;
				if(!CHECKBIT(PIND,BIT(3))) pos = 1;
			}
		}
		if(gameOver==0) {
			lcd_cmd(0x01);
			delay_ms(100);
			lcd_string("Level cleared.");
			delay_ms(300); 
		}
			
	
	}
	
	lcd_cmd(0x01);
	delay_ms(100);
	lcd_string("Press reset to play another game...");
	delay_ms(1000); 
	lcd_cmd(0x01);
	delay_ms(100);
	lcd_string("Anant Jain      IIT Delhi");
	delay_ms(300); 
    return 0;
		
	
}

