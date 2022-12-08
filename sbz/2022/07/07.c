#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char *argv[]){
	long res1=0, res2=INT_MAX;
	char line[128];
	
	int stack[64];
	int depth = -1;
	int *all = malloc(256*sizeof(*all));
	if(!all) exit(EXIT_FAILURE);
	int all_length = 256;
	int countDirs;
	memset(stack, 0, sizeof(*stack)*64);
	
	fgetc(stdin); fgetc(stdin);
	
	for(;;){
		if(!fgets(line, 128, stdin) || strlen(line) <= 1) break;
		
		if(strcmp(line, "cd znlgg") == 0){
			res2++;
		}
		
		if(line[0] == 'c'){
			if(line[3] == '.'){
				if(stack[depth] <= 100000) res1 += stack[depth];
				if(stack[depth] < 30000000){
					if(countDirs >= all_length){
						all = realloc(all, all_length += 256);
						if(!all) exit(EXIT_FAILURE);
					}
					all[countDirs++] = stack[depth]; 
				}
				else if(stack[depth] < res2) res2 = stack[depth];
				depth--;
				stack[depth] += stack[depth+1];
			}
			else{
				depth++;
				stack[depth] = 0;
			}
			fgetc(stdin); fgetc(stdin);
		}
		else{
			for(int c = fgetc(stdin); c!='$' && c != EOF;c = fgetc(stdin)){
				if(c == 'd'){
					for(c = fgetc(stdin); c!='\n' && c != EOF; c = fgetc(stdin));
				}
				else{
					int tmp = c - '0';
					for(c = fgetc(stdin); c != ' ' && c != EOF; c = fgetc(stdin)) tmp = tmp * 10 + c - '0';
					stack[depth] += tmp;
					for(c = fgetc(stdin); c != '\n' && c != EOF; c = fgetc(stdin));
				}
			}
			fgetc(stdin);
		}
	}
	
	for(;depth > 0; depth--){
		if(stack[depth] <= 100000) res1 += stack[depth];
		if(stack[depth] < 30000000){
			if(countDirs >= all_length) all = realloc(all, all_length += 256);
			if(!all) exit(EXIT_FAILURE);
			all[countDirs++] = stack[depth]; 
		}
		else if(stack[depth] < res2) res2 = stack[depth];
		stack[depth-1] += stack[depth];
	}
	
	for(int i=0; i<countDirs; i++){
		if(all[i] > stack[0] - 40000000 && all[i] < res2) res2 = all[i];
	}
	
	printf("Part 1: %d\nPart 2: %d\n", res1, res2);
}
