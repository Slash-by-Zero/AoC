#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define CUTOFF 3

int main(int argc, char *argv[]){
	int elf=0;
	int tmp=0;
	int top[CUTOFF];
	
	while(1){
		int c = fgetc(stdin);
		if(c == EOF) break;
		else if(c == '\n'){
			for(int i=0;i<CUTOFF;i++)
			{
				if(elf > top[i]){
					memcpy(top+i+1, top+i, CUTOFF-i-1);
					top[i] = elf;
					break;
				}
			}
			elf = 0;
		}
		else{
			tmp = c - '0';
			while(1){
				c = fgetc(stdin);
				if(c == '\n') break;
				tmp = tmp * 10 + c - '0';
			}
			elf += tmp;
		}
	}
		
	int res2 = top[0]+top[1]+top[2];
	
	printf("Part 1: %d\nPart 2: %d\n", top[0], res2);
}
