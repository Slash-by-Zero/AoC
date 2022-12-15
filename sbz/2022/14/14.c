#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#define MAX_LINE_LENGTH 512

#define sign(x) ((x>0) - (x<0))

int main(int argc, char *argv[]){
	long res1=0, res2=1;
	char line[MAX_LINE_LENGTH];
	
	int cave[256][1024];
	
	memset(cave, 0, 256*1024*4);
	
	int minx=INT_MAX, maxx=0, miny=INT_MAX, maxy=0;
	
	for(;;){
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line) <= 1) break;
		
		int oldx, oldy;
		char *tmp, *endptr;
		tmp = strtok(line, " ->");
		oldx = strtol(tmp, &endptr, 10);
		oldy = strtol(endptr+1, NULL, 10);
		if(oldx>maxx) maxx=oldx;
		if(oldy>maxy) maxy=oldy;
		if(oldx<minx) minx=oldx;
		if(oldy<miny) miny=oldy;
		
		for(tmp=strtok(NULL, " ->"); tmp; tmp = strtok(NULL, " ->")){
			char *endptr;
			int x=strtol(tmp, &endptr, 10);
			int y=strtol(endptr+1, NULL, 10);
			
			if(x>maxx) maxx=x;
			if(y>maxy) maxy=y;
			if(x<minx) minx=x;
			if(y<miny) miny=y;
			
			for(; x != oldx || y != oldy; oldx += (oldy+=sign(y-oldy), sign(x - oldx))){
				cave[oldy][oldx] = 2;
			}
			cave[oldy][oldx] = 2;
		}
	}
	
	int part1=1;
	
	for(;;res2++){
		int x=500, y=0;
		for(;part1;y++){
			if(y+1 > maxy) part1=0;
			else if((cave[y+1][x]&3) == 0);
			else if((cave[y+1][x-1]&3) == 0) x--;
			else if((cave[y+1][x+1]&3) == 0) x++;
			else break;
		}
		if(part1) cave[y][x] |= 1;
		if(part1) res1++;
		x=500; y=0;
		for(;;y++){
			if(y+1 > maxy+1) break;
			else if((cave[y+1][x]&6) == 0);
			else if((cave[y+1][x-1]&6) == 0) x--;
			else if((cave[y+1][x+1]&6) == 0) x++;
			else break;
		}
		cave[y][x] |= 4;
		if(x==500 && y==0) break;
	}
	
	printf("Part 1: %ld\nPart 2: %ld\n", res1, res2);
}
