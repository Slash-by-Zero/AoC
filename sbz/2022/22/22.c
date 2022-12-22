#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#define MAX_LINE_LENGTH 256

static int map[256][256];
static int cubeSize = INT_MAX;
static int move[4][2] = {{0,1},{1,0},{0,-1},{-1,0}};

static void calcEdges(){
	int x=0, y=1, dir=0;
	
	int pos;
	
	int edges[3*4];
	memset(edges, 0, 3*4*4);
	
	int coords[3]={0,0,0};
	int dir3d={0,0,1};
	
	for(int i=0;i<14;i++){
		x += move[dir][1] * (cubeSize-1);
		y += move[dir][0] * (cubeSize-1);
		
		
		int i;
		for(i=-1;i<=1;i++) if(map[y+move[(dir+i+4)%4][0]][x+move[(dir+i+4)%4][1]] & ~1 == 0) dir=(dir+i+4)%4;
		
		if(i == -1){
			for(int;
		}
		else if(i==0){
			
		}
	}
}

int main(int argc, char *argv[]){
	long res1=0, res2=1;
	char line[MAX_LINE_LENGTH];
	
	memset(map, 0xff, 256*256*sizeof(int));
	
	int row=1;
	
	
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
	
	calcEdges();
	
	//TODO: run part 2
	
	printf("Part 1: %ld\nPart 2: %ld\n", res1, res2);
}
