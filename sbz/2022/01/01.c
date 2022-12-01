#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int main(int argc, char *argv[]){
	int elf=0;
	int top[3];
	int i, tmp, res2;
	
	for(;;){
		int c = fgetc(stdin);
		if(c == EOF) break;
		else if(c == '\n'){
			for(i=0;i<3;i++){
				if(elf > top[i]){
					memcpy(top+i+1, top+i, (2-i) << 2);
					top[i] = elf;
					break;
				}
			}
			elf = 0;
		}
		else{
			tmp = c - '0';
			for(;;){
				c = fgetc(stdin);
				if(c == '\n' ) break;
				tmp <<= 1;
				tmp += tmp<<2;
				tmp += c - '0';
			}
			elf += tmp;
		}
	}
	
	res2 = top[0]+top[1]+top[2];
	
	printf("Part 1: %d\nPart 2: %d\n", top[0], res2);
}
