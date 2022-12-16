#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 128

int adj[676][677];
int flow[676];

int calcPressure(int, int);
int calcPressure2(int, int, int, int);

int main(int argc, char* argv[]){
	long res1=0, res2=0;
	
	char line[MAX_LINE_LENGTH];
	
	memset(adj, 0, 676*677*sizeof(int));
	
	memset(flow, 0, 676*sizeof(int));
	
	for(;;){
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line)<=1) break;
		
		int i = (line[6] - 'A')*26 + line[7] - 'A';
		char *endptr;
		flow[i] = strtol(line+23, &endptr, 10);
		adj[i][676] = 1;
		endptr+=24;
		for(char *tun = strtok(endptr, ", "); tun; tun=strtok(NULL, ", ")){
			int j=(tun[0] - 'A')*26+tun[1]-'A';
			adj[i][j] = 1;
		}
	}
	
	for(int i=0;i<676;i++){
		if(!adj[i][676]) continue;
		for(int j=0;j<676;j++){
			if(adj[i][j] == 0) continue;
			for(int k=0;k<676;k++){
				if(adj[i][k] == 0) continue;
				
				int new = adj[i][k] + adj[i][j];
				if(new < adj[j][k] || adj[j][k] == 0){
					adj[j][k] = new;
					adj[k][j] = new;
				}
			}
		}
	}
	
	res1 = calcPressure(0, 0);
	res2 = calcPressure2(0,0,0,0);
	
	printf("Part 1: %ld\nPart 2: %ld\n", res1, res2);
}

int calcPressure(int time, int curr){
	int myflow = flow[curr];
	flow[curr] = 0;
	
	int res=0;
	if(myflow > 0){
		time++;
		res = myflow * (30 - time);
	}
	int max=0;
	
	for(int i=0;i<676;i++){
		if(flow[i] == 0 || time+adj[curr][i]+1 > 29) continue;
		
		int tmp = calcPressure(time + adj[curr][i], i);
		if(tmp > max) max = tmp;
	}
	
	flow[curr] = myflow;
	return max + res; 
}

int calcPressure2(int time, int curr, int other_time, int other){
	int myflow = flow[curr];
	flow[curr] = 0;
	
	int res=0;
	if(myflow > 0){
		time++;
		res = myflow * (26 - time);
	}
	int max=0;
	
	for(int i=0;i<676;i++){
		if(flow[i] == 0 || time+adj[curr][i]+1 >= 26 || i == other) continue;
		if(time == 0 || other_time == 0) printf("Yep %d\n", other_time);
		int new = time + adj[curr][i];
		int tmp;
		if(new < other_time) tmp = calcPressure2(new, i, other_time, other);
		else tmp = calcPressure2(other_time, other, new, i);
		if(tmp > max) max = tmp;
	}
	if(max == 0 && other_time != INT_MAX) max = calcPressure2(other_time, other, INT_MAX, 0);
	flow[curr] = myflow;
	return max + res; 
}
