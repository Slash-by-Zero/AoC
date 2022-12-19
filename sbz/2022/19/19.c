#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#define MAX_LINE_LENGTH 256

static int blueprints[32][4][3];
static int maxres[32][4];

static int maxGeodes(int, int, int[2][4],int, int);

int main(int argc, char *argv[]){
	long res1=0, res2=1;
	char line[MAX_LINE_LENGTH];
	
	memset(maxres, 0, 32*4*4);
	memset(blueprints, 0, 32*4*3*sizeof(***blueprints));
	int blueprints_count=0;
	
	for(;;){
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line)<=1) break;
		
		char *endptr=line+10;
		strtol(endptr, &endptr, 10);
		blueprints[blueprints_count][0][0] = strtol(endptr+23, &endptr, 10);
		blueprints[blueprints_count][1][0] = strtol(endptr+28, &endptr, 10);
		blueprints[blueprints_count][2][0] = strtol(endptr+32, &endptr, 10);
		blueprints[blueprints_count][2][1] = strtol(endptr+9, &endptr, 10);
		blueprints[blueprints_count][3][0] = strtol(endptr+30, &endptr, 10);
		blueprints[blueprints_count][3][2] = strtol(endptr+9, &endptr, 10);
		for(int i=0;i<4;i++){
			if(blueprints[blueprints_count][i][0] > maxres[blueprints_count][0]) maxres[blueprints_count][0] = blueprints[blueprints_count][i][0];
		}
		maxres[blueprints_count][1] = blueprints[blueprints_count][2][1];
		maxres[blueprints_count][2] = blueprints[blueprints_count][3][2];
		maxres[blueprints_count][3] = 1;
		blueprints_count++;
	}
	
	int init[2][4] = {{1,0,0,0},{0,0,0,0}};
	
	for(int i=0;i<blueprints_count;i++){
		int inittmp[2][4];
		memcpy(inittmp, init, 32);
		long tmp = maxGeodes(0,i,inittmp,24,0);
		res1 += (i+1) * tmp;
	}
	
	for(int i=0;i < blueprints_count && i < 3;i++){
		int inittmp[2][4];
		memcpy(inittmp, init, 32);
		long tmp = maxGeodes(0,i,inittmp,32,0);
		res2 = res2 * tmp;
	}
	
	printf("Part 1: %ld\nPart 2: %ld\n", res1, res2);
}

static int maxGeodes(int time, int blueprint, int state[2][4], int limit, int cutoff){
	int max=0;
	int built[4];
	memset(built, 0, 16);
	for(;;){
		int canBuild[4];
		for(int i=0;i<4;i++){
			canBuild[i]=1;
			for(int j=0;j<3;j++) canBuild[i] = canBuild[i] && state[1][j] >= blueprints[blueprint][i][j];
		}
		for(int i=0;i<4;i++) state[1][i]+=state[0][i];
		time++;
		int tmp = limit-time;
		if(state[1][3]+(tmp*(tmp+1))/2 < cutoff) break;
		if(time>=limit) break;
		for(int i=0;i<4;i++){
			if(built[i] || !canBuild[i] || state[0][i]>=maxres[blueprint][i]) continue;
			int newState[2][4];
			memcpy(newState, state, 32);
			if(i<3){
				newState[0][i]++;
			}
			else{
				newState[1][i]+=limit-time;
			}
			for(int j=0;j<3;j++) newState[1][j] -= blueprints[blueprint][i][j];
			int tmp = maxGeodes(time, blueprint, newState, limit, max);
			if(tmp>max) max=tmp;
			built[i] = 1;
		}
	}
	if(state[1][3] >= max) return state[1][3];
	return max;
}
