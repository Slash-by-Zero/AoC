#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 128

static int cube[64][64][64];

static long countSurface(int, int, int);

int main(int argc, char *argv[]){
	long res1=0, res2=0;
	char line[MAX_LINE_LENGTH];
	
	memset(cube, 0, 64*64*64*sizeof(int));
	
	for(;;){
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line)<=1) break;
		
		int x,y,z;
		char *endptr;
		x = strtol(line, &endptr, 10)+1;
		y = strtol(endptr+1, &endptr, 10)+1;
		z = strtol(endptr+1, NULL, 10)+1;
		
		res1 += 6;
		if(cube[x-1][y][z])	res1-=2;
		if(cube[x+1][y][z])	res1-=2;
		if(cube[x][y-1][z])	res1-=2;
		if(cube[x][y+1][z])	res1-=2;
		if(cube[x][y][z-1])	res1-=2;
		if(cube[x][y][z+1])	res1-=2;
		
		cube[x][y][z] = 1;
	}
	
	res2 = countSurface(0,0,0);
	
	printf("Part 1: %ld\nPart 2: %ld\n", res1, res2);
}

static long countSurface(int x, int y, int z){
	long res=0;
	if(x>0 && cube[x-1][y][z]==1)	res++;
	if(x<49 &&cube[x+1][y][z]==1)	res++;
	if(y>0 && cube[x][y-1][z]==1)	res++;
	if(y<49 && cube[x][y+1][z]==1)	res++;
	if(z>0 && cube[x][y][z-1]==1)	res++;
	if(z<49 && cube[x][y][z+1]==1)	res++;
	cube[x][y][z]=2;
	if(x>0 && cube[x-1][y][z]==0) res+=countSurface(x-1, y, z);
	if(x<49 && cube[x+1][y][z]==0) res+=countSurface(x+1, y, z);
	if(y>0 && cube[x][y-1][z]==0) res+=countSurface(x, y-1, z);
	if(y<49 && cube[x][y+1][z]==0) res+=countSurface(x, y+1, z);
	if(z>0 && cube[x][y][z-1]==0) res+=countSurface(x, y, z-1);
	if(z<49 && cube[x][y][z+1]==0) res+=countSurface(x, y, z+1);
	return res;
}
