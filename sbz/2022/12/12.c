#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#define MAX_LINE_LENGTH 128

int main(int argc, char *argv[]){
	long res1, res2;
	char line[MAX_LINE_LENGTH][MAX_LINE_LENGTH];
	int width=0, height=0;
	
	for(;;height++){
		if(!fgets(line[height], MAX_LINE_LENGTH, stdin) || strlen(line[height]) <= 1) break;
		line[height][strlen(line[height])-1] = '\0';
		width = strlen(line[height]);
	}
	
	int xE=0, yE=0, xS=0, yS=0;
	
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			if(line[i][j] == 'E'){
				xE=j;
				yE=i;
				line[i][j] = 'z';
			}
			if(line[i][j] == 'S'){
				xS=j;
				yS=i;
				line[i][j] = 'a';
			}
		}
	}
	
	int dists[height][width], checked[height][width];
	memset(dists, 0, height*width*4);
	memset(checked, 0, height*width*4);
	dists[yE][xE]=1;
	for(;;){
		int x, y, dist = INT_MAX;
		for(int i=0;i<height;i++){
			for(int j=0;j<width;j++){
				if(dists[i][j] != 0 && !checked[i][j] && dists[i][j] < dist){
					dist = dists[i][j];
					y = i;
					x = j;
				}
			}
		}
		
		checked[y][x] = 1;
		if(y == yS && x == xS) break;
		char el = line[y][x] - 1;
		
		if(x > 0 && line[y][x-1] >= el && (dist+1 < dists[y][x-1] || dists[y][x-1] == 0)) dists[y][x-1] = dist+1;
		if(y > 0 && line[y-1][x] >= el && (dist+1 < dists[y-1][x] || dists[y-1][x] == 0)) dists[y-1][x] = dist+1;
		if(x < width-1 && line[y][x+1] >= el && (dist+1 < dists[y][x+1] || dists[y][x+1] == 0)) dists[y][x+1] = dist+1;
		if(y < height-1 && line[y+1][x] >= el && (dist+1 < dists[y+1][x] || dists[y+1][x] == 0)) dists[y+1][x] = dist+1;
	}
	res1 = dists[yS][xS] - 1;
	
	res2 = res1;
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			if(dists[i][j] < res2 && dists[i][j] != 0 && line[i][j] == 'a'){
				res2 = dists[i][j] - 1;
			}
		}
	}
	
	printf("Part 1: %ld\nPart 2: %ld\n", res1, res2);
}
