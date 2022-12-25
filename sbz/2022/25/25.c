#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[]){
	long curr=0;
	long res1;
	
	for(int c = fgetc(stdin); c != EOF; c = fgetc(stdin)){
		if(c == '\n'){
			res1+=curr;
			curr=0;
			continue;
		}
		
		curr = curr * 5 + c - '0';
		if( c == '='){
			curr-=15;
		}
		else if(c == '-'){
			curr+=2;
		}
	}
	
	printf("Part 1: ");
	
	char output[32];
	output[0]=0;
	
	for(;res1>0;res1 = res1/5){
		res1+=2;
		memcpy(output+1, output, 31);
		output[0] = res1%5 == 1 ? '-' : (res1%5 == 0 ? '=' : (res1%5+'.'));
	}
	
	printf("%s\n", output);
}
