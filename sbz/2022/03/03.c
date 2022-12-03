#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	int res1 = 0, res2 = 0;
	char first[128];
	char all[128], candidates[128];
	char badge;
	int length, i;
	char line[128];
	memset(candidates, 1, 128);
	
	for(int g = 0;;g = (g+1)%3){
		if(!fgets(line, 128, stdin)) break;
		line[strlen(line)-1] = '\0';
		length = strlen(line)/2;
		
		memset(first, 0, 128);
		memset(all, 0, 128);
		for(i=0;i<length; i++){
			first[(int) line[i]] = 1;
			all[(int) line[i]] = 1;
		}
		for(;i < length * 2;i++){
			all[(int) line[i]] = 1;
			if(first[(int) line[i]]) {
				res1 += line[i];
				if(line[i] >= 'a') res1-='a'-1;
				else res1-='A'-27;
				break;
			}
		}
		for(;i < length * 2;i++){
			all[(int) line[i]] = 1;
		}
		
		for(i='A'; i<='z'; i++){
			candidates[i] &= all[i];
			if(candidates[i]) badge = i;
		}
		
		if(g == 2){
			res2 += badge;
			if(badge >= 'a') res2-='a'-1;
			else res2-='A'-27;
			memset(candidates, 1, 128);
		}
	}
	
	printf("Part 1: %d\nPart 2: %d\n", res1, res2);
}
