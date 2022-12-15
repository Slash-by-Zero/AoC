#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#define MAX_LINE_LENGTH 128

#define abs(c) ((((c)>0)-((c)<0))*(c))

struct sensor{
	int x,y;
	int dist;
};

int main(int argc, char *argv[]){
	long res1=0, res2=0;
	char line[MAX_LINE_LENGTH];
	
	long regions[11][2];
	memset(regions, 0x00, sizeof(long)*22);
	
	struct sensor sensors[32];
	int sensor_count=0;
	
	fgets(line, MAX_LINE_LENGTH, stdin);
	int row=strtol(line, NULL, 10);
	int MAXX=row*2;
	
	long plDiag[64];
	memset(plDiag, 0, 64*sizeof(long));
	int pld_count=1;
	plDiag[0]=row;
	long mnDiag[64];
	memset(mnDiag, 0, 64*sizeof(long));
	int mnd_count=1;
	mnDiag[0]=0;
	
	for(;;sensor_count++){
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
		sensors[sensor_count].dist=dist;
		
		int tmp = xS + dist + 1 + yS;
		for(int i=0;i<=pld_count;i++){
			if(i==pld_count){
				plDiag[pld_count++] = tmp;
				break;
			}
			else if(plDiag[i] == tmp) break;
		}
		tmp = xS - dist - 1 + yS;
		for(int i=0;i<=pld_count;i++){
			if(i==pld_count){
				plDiag[pld_count++] = tmp;
				break;
			}
			else if(plDiag[i] == tmp) break;
		}
		tmp = xS + dist + 1 - yS;
		for(int i=0;i<=mnd_count;i++){
			if(i==mnd_count){
				mnDiag[mnd_count++] = tmp;
				break;
			}
			else if(mnDiag[i] == tmp) break;
		}
		tmp = xS - dist - 1 - yS;
		for(int i=0;i<=mnd_count;i++){
			if(i==mnd_count){
				mnDiag[mnd_count++] = tmp;
				break;
			}
			else if(mnDiag[i] == tmp) break;
		}
		
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
	
	for(int i=0;i<pld_count;i++){
		for(int j=0;j<mnd_count;j++){
			int tmp = plDiag[i]-mnDiag[j];
			if(tmp%2 != 0) continue;
			int p=1;
			int y = tmp/2;
			int x = mnDiag[j] + y;
			if(x<0 || y<0 || x>MAXX || y>MAXX) continue;
			for(int k=0;k<sensor_count;k++){
				int dist = abs(sensors[k].x-x)+abs(sensors[k].y-y);
				p = p && dist > sensors[k].dist;
			}
			if(p)
			{
				res2 = x;
				res2 *= 4000000l;
				res2+= y;
				printf("%d %d = %ld\n", x, y, ((long) x)*4000000+y);
			}
		}
	}
	
	for(int i=0;i<10;i++){
		if(regions[i][0] == 0 && regions[i][1] == 0){
			break;
		}
		res1 += regions[i][1] - regions[i][0] + 1;
	}
	
	printf("Part 1: %ld\nPart 2: %ld\n", res1, res2);
}
