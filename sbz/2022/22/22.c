#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#define MAX_LINE_LENGTH 256

int main(int argc, char *argv[]){
	long res1=0, res2=1;
	char line[MAX_LINE_LENGTH];
	
	int map[256][256];
	memset(map, 0xff, 256*256*sizeof(int));
	
	int row=1;
	
	int cubeSize = INT_MAX;
	
	for(;;){
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line)<=1) break;
		
		int lower=0, upper=strlen(line);
		for(int i=0;i<strlen(line);i++){
			if(line[i] == '.') map[row][i+1] = 0;
			else if(line[i] == '#') map[row][i+1] = 1;
			else lower++;
		}
		
		if(upper-lower<cubeSize) cubeSize=upper-lower;
		
		row++;
	}
	
	int c;
	int x=1, y=1;
	int dir=0;
	
	int move[4][2] = {{0,1},{1,0},{0,-1},{-1,0}};
	
	for(x=1; map[y][x] != 0;x++);
	
	for(c = fgetc(stdin); c != EOF && c != '\n'; c = fgetc(stdin)){
		int n = c - '0';
		for(c = fgetc(stdin); c >= '0' && c <= '9'; c = fgetc(stdin)){
			n = n*10 + c - '0';
		}
		
		int prevx=x, prevy=y;
		for(;n>0; n--){
			x += move[dir][1];
			y += move[dir][0];
			if(map[y][x] == -1){
				for(;map[(y+256)%256][(x+256)%256] == -1; x += (y += move[dir][0], move[dir][1]));
				x=(x+256)%256;
				y=(y+256)%256;
			}
			if(map[y][x] == 1){
				x=prevx;
				y=prevy;
				break;
			}
			prevx=x;
			prevy=y;
		}
		
		if(c == EOF || c == '\n') break;
		
		if(c == 'R') dir++;
		if(c == 'L') dir--;
		dir = (dir+4)%4;
	}
part1f:
	
	res1 = 1000*y+4*x+dir;
	
	printf("Part 1: %ld\nPart 2: %ld\n", res1, res2);
}
