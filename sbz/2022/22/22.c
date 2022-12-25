#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#define MAX_LINE_LENGTH 256

static int map[256][256];
static int cubeSize = INT_MAX;
static int move[4][2] = {{0,1},{1,0},{0,-1},{-1,0}};
static int order[14] = {1,1,1,1,0,0,1,0,0,0,1,1,0,0};
//static int order[14] = {1,1,1,0,0,1,1,1,0,0,0,0,1,0};

static void calcEdges(){
	int x=0, y=1, dir=0;
	
	int pos=0;
	
	struct{
		int x, y, dir;
	} stack[7];
	
	for(;(map[y][x] & ~1) != 0;x++);
	
	for(int i=0;i<14;i++){
		if(order[i]){
			x += move[dir][1] * (cubeSize-1);
			y += move[dir][0] * (cubeSize-1);
			stack[pos].x=x;
			stack[pos].y=y;
			stack[pos].dir = (dir+2)%4;
			pos++;
		}
		else{
			pos--;
			for(int j=0;j<cubeSize;j++){
				int x2=stack[pos].x+move[stack[pos].dir][1]*j, y2=stack[pos].y+move[stack[pos].dir][0]*j;
				int tmp = (x2) << 8 | (y2);
				
				map[y+move[(dir+3)%4][0]][x+move[(dir+3)%4][1]] &= (dir%2 == 0) ? (0xffff0000 | tmp) : (0x0000ffff | (tmp << 16));
				
				tmp = (x) << 8 | (y);
				
				map[y2+move[(stack[pos].dir+1)%4][0]][x2+move[(stack[pos].dir+1)%4][1]] &= (stack[pos].dir%2 == 0) ? (tmp | 0xffff0000) : (0x0000ffff | (tmp << 16));
				
				y+=move[dir][0];
				x+=move[dir][1];
			}
			y-=move[dir][0];
			x-=move[dir][1];
		}
		
		if((map[y+move[(dir+3)%4][0]+move[dir][0]][x+move[(dir+3)%4][1]+move[dir][1]] & ~1) == 0){
			x+=move[(dir+3)%4][1]+move[dir][1];
			y+=move[(dir+3)%4][0]+move[dir][0];
			dir = (dir+3)%4;
		}
		else if((map[y+move[dir][0]][x+move[dir][1]] & ~1) == 0){
			x+=move[dir][1];
			y+=move[dir][0];
		}
		else{
			dir = (dir+1)%4;
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
	
	
	calcEdges();
	
	for(x=1; map[y][x] != 0;x++);
	
	int x2=x, y2=y, dir2=0;
	
	for(c = fgetc(stdin); c != EOF && c != '\n'; c = fgetc(stdin)){
		int n = c - '0';
		for(c = fgetc(stdin); c >= '0' && c <= '9'; c = fgetc(stdin)){
			n = n*10 + c - '0';
		}
		
		int prevx=x, prevy=y, prevdir;
		for(int i=n;i>0; i--){
			x += move[dir][1];
			y += move[dir][0];
			if((map[y][x] & ~1) != 0){
				for(;(map[(y+256)%256][(x+256)%256] & ~1) != 0; x += (y += move[dir][0], move[dir][1]));
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
		
		prevx=x2; prevy=y2; prevdir=dir2;
		for(;n>0; n--){
			x2 += move[dir2][1];
			y2 += move[dir2][0];
			if((map[y2][x2] & ~1) != 0){
				if(prevx == 1 && prevy == 151){ 
				}
				int tmp = map[y2][x2];
				if(dir2%2 != 0){
					x2 = (0xff00 & tmp) >> 8;
					y2 = (0xff & tmp) >> 0;
				}
				else{
					x2 = (0xff000000 & tmp) >> 24;
					y2 = (0xff0000 & tmp) >> 16;
				}
				if(map[y2][x2] == 1){
					dir2=prevdir;
					x2=prevx;
					y2=prevy;
					break;
				}
				tmp = prevx << 8 | prevy;
				for(int i=0;i<4;i++){
					int tmp2 = tmp;
					if(i%2==0) tmp2 = tmp2 << 16;
					tmp2 = ~tmp2 & map[y2-move[i][0]][x2-move[i][1]];
					if((map[y2-move[i][0]][x2-move[i][1]] & ~1) != 0 && (tmp2 == -65536 || tmp2 == 65535)) {dir2 = i; break;}
				}
			}
			else if(map[y2][x2] == 1){
				x2=prevx;
				y2=prevy;
				break;
			}
			prevx=x2;
			prevy=y2;
			prevdir=dir2;
		}
		
		
		if(c == EOF || c == '\n') break;
		
		if(c == 'R') dir++;
		if(c == 'L') dir--;
		dir = (dir+4)%4;
		if(c == 'R') dir2++;
		if(c == 'L') dir2--;
		dir2 = (dir2+4)%4;
	}
	
	res1 = 1000*y+4*x+dir;
	res2 = 1000*y2+4*x2+dir2;
	
	
	
	printf("Part 1: %ld\nPart 2: %ld\n", res1, res2);
}
