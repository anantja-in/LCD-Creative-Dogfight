#include<iostream>
using namespace std;

char arr1[16];
char arr2[16];
char line1[5][60];
char line2[5][60];

void init() {
	int x;
	for(x=0;x<16;x++)       arr1[x]=' ';
	for(x=0;x<16;x++)       arr2[x]=' ';
	arr1[1]='>';
	
}
void refresh() {
        int x;
        for(x=0;x<16;x++)       cout<<arr1[x];
	cout<<endl;
	for(x=0;x<16;x++)	cout<<arr2[x];
}

void assign(int y, int level) {
	int x,z;
	z=0;
	for(x=y;x<y+16 && x<60;x++)	arr1[z++]=line1[level][x];
	z=0;
	for(x=y;x<y+16 && x<60;x++)	arr2[z++]=line2[level][x];
}
int main() {

	line1[0] = "    <     <    < <    <<<         <          <    <   <    ";
	line2[0] = "       <            <      <            <       <   <      ";
	
	// MyCar is > and other cars are <
	init();	
	
	int level = 0; // current level
	int time = 0;
	int pos = 0;
	while(time<60) {
		// Put the strings
		assign(time++);
		
		// Check for collision first
		if((line1[level][1]=='<' && pos == 0) || (line2[level][1]=='<' && pos == 1)) {
			time = 60;
			// Game over procedure
		}	
	
		if(pos==0) line1[level][1] = '>';
		else	line2[level][1] = '>';
		refresh();
		// Wait for a sec, checking for input change
		delay_ms(1000-(level*100));
		// Check for button pressed
		if(button_pressed)	{ pos+=1; pos%=2;}
	}
        return 0;
}

