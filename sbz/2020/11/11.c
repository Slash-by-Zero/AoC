#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 512

static int dirs[8][2] = {{1,0}, {1,1}, {0,1}, {-1,1}, {-1,0},{-1,-1},{0,-1},{1,-1}};

void die(char *msg){
	perror(msg);
	exit(EXIT_FAILURE);
}



int main(int argc, char *argv[]){
	int res1=0, res2=0;
	
	char input[MAX_LINE_LENGTH][MAX_LINE_LENGTH][2];
	
	char line[MAX_LINE_LENGTH];
	
	int rows = 0, columns = 0;
	for(;;){
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line) <= 1) break;
		line[strlen(line)-1] = '\0';
		columns = strlen(line);
		for(int i=0;i<columns;i++){
			if(line[i] == '.') memset(input[rows][i], 0x9f, 2);
			else {
				memset(input[rows][i], 0x30, 2);
				res1++;
			}
		}
		rows++;
	}
	
	res2=res1;
	
	for(;;){
		int change = 0;
		
		for(int i=0;i<rows;i++){
			for(int j=0;j<columns;j++){
				if(input[i][j][0] < 0) {
					continue;
				}
				
				input[i][j][0] = (input[i][j][0] << 1) & 0x40;
				input[i][j][0] |= input[i][j][0] >> 1;
				
				input[i][j][1] = (input[i][j][1] << 1) & 0x40;
				input[i][j][1] |= input[i][j][1] >> 1;
				
				
				for(int dir = 0; dir < 8; dir++){
					int x=j+dirs[dir][0], y=i+dirs[dir][1];
					if(x<0 || x>=columns || y<0 || y>=rows) continue;
					if((input[y][x][0] & (dir < 4 ? 0x20 : 0x40)) != 0) {
						input[i][j][0]++;
					}
					while(input[y][x][1] < 0 && x>=0 && y>=0 && x<columns && y<rows){
						x+=dirs[dir][0];
						y+=dirs[dir][1];
					}
					if(x<0 || x>=columns || y<0 || y>=rows) continue;
					if((input[y][x][1] & (dir < 4 ? 0x20 : 0x40)) != 0) {
						input[i][j][1]++;
					}
				}
				
				if(input[i][j][0] == 0){
					change = 1;
					input[i][j][0] ^= 0x20;
					res1++;
				}
				else if((input[i][j][0]) >= 0x64){
					change = 1;
					input[i][j][0] ^= 0x20;
					res1--;
				}
				
				if(input[i][j][1] == 0){
					change = 1;
					input[i][j][1] ^= 0x20;
					res2++;
				}
				else if(input[i][j][1] >= 0x65){
					change = 1;
					input[i][j][1] ^= 0x20;
					res2--;
				}
			}
		}
		
		if(!change) break;
	}
	
	printf("Part 1: %d\nPart 2: %d\n", res1, res2);
}

