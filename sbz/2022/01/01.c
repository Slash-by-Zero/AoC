#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	int res1=0;
	int res2=0;
	int elf=0;
	int top3[3];
	
	while(1){
		char line[32];
		
		if(!fgets(line, 32, stdin)) break;
		
		
		if(line[0] == '\n'){
			if(elf > res1) res1 = elf;
			for(int i = 0;i<3;i++){
				if(elf > top3[i]){
					int tmp = top3[i];
					top3[i] = elf;
					elf=tmp;
				}
			}
			elf = 0;
			continue;
		}
		
		elf += strtol(line, NULL, 10);
		
	}
	
	res2 = top3[0]+top3[1]+top3[2];
	
	printf("Part 1: %d\nPart 2: %d\n", res1, res2);
}
