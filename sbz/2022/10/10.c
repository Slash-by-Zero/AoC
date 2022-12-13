#include <stdio.h>

int main(int argc, char *argv[]){
	int res1=0, c=0, cycle=1, x = 1;
	int crt[6*40];
	for(;;cycle++){
		
		c = fgetc(stdin);
		if(c == EOF || c == '\n') break;
		
		fgetc(stdin); fgetc(stdin); fgetc(stdin); fgetc(stdin);
		
		if((cycle+20)%40==0) res1+=x*cycle;
		crt[cycle-1] = ((cycle-1)%40 >= x-1 && (cycle-1)%40 <= x+1) ? '#' : '.';
		
		if(c != 'a') continue;
		
		cycle++;
		if((cycle+20)%40==0) res1+=x*cycle;
		crt[cycle-1] = ((cycle-1)%40 >= x-1 && (cycle-1)%40 <= x+1) ? '#' : '.';
		
		c = fgetc(stdin);
		
		int tmp = 0;
		if(c == '-'){
			for(c = fgetc(stdin); c != '\n'; c = fgetc(stdin)) tmp=tmp*10-c+'0';
		}
		else{
			tmp = c - '0';
			for(c = fgetc(stdin); c != '\n'; c = fgetc(stdin)) tmp=tmp*10+c-'0';
		}
		
		x+=tmp;
	}
	
	printf("Part 1: %d\nPart 2:", res1);
	for(int i=0;i<cycle;i++){
		if(i%40==0) printf("\n");
		printf("%c", (char) crt[i]);
	}
	printf("\n");
}
