#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#define MAX_LINE_LENGTH 512

#define sign(x) ((x>0) - (x<0))

int main(int argc, char *argv[]){
	long res1=0, res2=0;
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
	
	int stack[256];
	stack[0] = 500;
	
	for(int y=0;;y++){
		if(y > maxy);
		else if((cave[y+1][stack[y]]) == 0) {stack[y+1]=stack[y]; continue;}
		else if((cave[y+1][stack[y]-1]) == 0) {stack[y+1]=stack[y]-1; continue;}
		else if((cave[y+1][stack[y]+1]) == 0) {stack[y+1]=stack[y]+1; continue;}
		
		if(part1 && y > maxy){
			res1 = res2;
			part1 = 0;
		}
		
		cave[y][stack[y]] = 1;
		
		res2++;
		if(y==0) break;
		y-=2;
	}
	
	printf("Part 1: %ld\nPart 2: %ld\n", res1, res2);
}
