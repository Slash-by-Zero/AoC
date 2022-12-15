#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#define MAX_LINE_LENGTH 128

#define abs(c) (((c>0)-(c<0))*(c))

#define row 2000000
#define MAXX (row*2)

struct sensor{
	int x,y;
	int dist;
};

int main(int argc, char *argv[]){
	long res1=0, res2=0;
	char line[MAX_LINE_LENGTH];
	
	long regions[11][2];
	memset(regions, 0x00, sizeof(long)*22);
	
	struct sensor sensors[20];
	int sensor_count=0;
	
	
	for(;;){
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line) <= 1) break;
		
		int xS, yS, xB, yB;
		char *endptr;
		xS = strtol(line+12, &endptr, 10);
		yS = strtol(endptr+4, &endptr, 10);
		xB = strtol(endptr+25, &endptr, 10);
		yB = strtol(endptr+4, NULL, 10);
		
		int dist = abs(xS-xB) + abs(yS-yB);
		
		int upper = xS + dist - abs(yS-row);
		int lower = xS - dist + abs(yS-row);
		
		sensors[sensor_count].x=xS;
		sensors[sensor_count].y=yS;
		sensors[sensor_count++].dist=dist;
		
		if(lower > xS) continue;
		
		for(int i=0;i<10;i++){
			if(regions[i][0] == 0 && regions[i][1] == 0){
				regions[i][0] = lower;
				regions[i][1] = upper;
				break;
			}
			if((lower >= regions[i][0] && lower <= regions[i][1]) || (upper >= regions[i][0] && upper <= regions[i][1]) || (regions[i][0] >= lower && regions[i][0] <= upper)){
				if(lower > regions[i][0]) lower = regions[i][0];
				if(upper < regions[i][1]) upper = regions[i][1];
				memcpy(&regions[i], &regions[i+1], (10-i)*sizeof(long));
				i--;
			}
		}
	}
	
	for(int i=0;i<=MAXX;i++){
		for(int j=0;j<=MAXX;j++){
			int p=1;
			for(int k=0;k<sensor_count;k++){
				int dist = abs(sensors[k].x-i) + abs(sensors[k].y-j);
				p &= dist > sensors[k].dist;
				if(!p) break;
			}
			if(p){
				res2 = i*4000000+j;
				goto part2f;
			}
		}
	}
part2f:
	
	for(int i=0;i<10;i++){
		if(regions[i][0] == 0 && regions[i][1] == 0){
			break;
		}
		res1 += regions[i][1] - regions[i][0] + 1;
	}
	
	printf("Part 1: %ld\nPart 2: %ld\n", res1, res2);
}
