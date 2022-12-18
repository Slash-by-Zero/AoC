#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 128

#define abs(c) (((c)<0 ? -1 : 1) * (c))

int adj[676][676];
int flow[676];

int exist[MAX_LINE_LENGTH];
int node_count=0;

int calcPressure(int, int);
int calcPressure2(int, int, int, int);

int main(int argc, char* argv[]){
	long res1=0, res2=0;
	
	char line[MAX_LINE_LENGTH];
	
	memset(adj, 0, 676*676*sizeof(int));
	
	memset(flow, 0, 676*sizeof(int));
	
	for(;;){
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line)<=1) break;
		
		int i = (line[6] - 'A')*26 + line[7] - 'A';
		char *endptr;
		flow[i] = strtol(line+23, &endptr, 10);
		exist[node_count++] = i;
		endptr+=24;
		for(char *tun = strtok(endptr, ", "); tun; tun=strtok(NULL, ", ")){
			int j=(tun[0] - 'A')*26+tun[1]-'A';
			adj[i][j] = 1;
		}
	}
	
	for(int ti=0;ti<node_count;ti++){
		int i=exist[ti];
		//if(flow[i] != 0) continue;
		for(int tj=0;tj<node_count;tj++){
			int j=exist[tj];
			if(adj[i][j] == 0) continue;
			for(int tk=0;tk<node_count;tk++){
				int k=exist[tk];
				if(adj[i][k] == 0) continue;
				
				int new = adj[i][k] + adj[i][j];
				if(new < adj[j][k] || adj[j][k] == 0){
					adj[j][k] = new;
					adj[k][j] = new;
				}
			}
		}
	}
	
	for(int ti=0;ti<node_count;ti++){
		int i = exist[ti];
		if(flow[i] != 0) continue;
		
		node_count--;
		memcpy(exist+ti, exist+ti+1, (node_count-ti)*sizeof(int));
		ti--;
	}
	
	for(int ti=0;ti<node_count;ti++){
		int i=exist[ti];
		for(int tj=0;tj<node_count;tj++){
			int j=exist[tj];
			if(adj[i][j] == 0) continue;
			for(int tk=0;tk<node_count;tk++){
				int k=exist[tk];
				if(adj[i][k] == 0) continue;
				
				int total = abs(adj[i][k]) + abs(adj[i][j]);
				if(abs(adj[i][k]) * flow[i] > (total - abs(adj[j][k])) * flow[k]) adj[j][k] = -abs(adj[j][k]);
				if(abs(adj[i][j]) * flow[i] > (total - abs(adj[k][j])) * flow[j]) adj[k][j] = -abs(adj[k][j]);
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
	
	for(int ti=0;ti<node_count;ti++){
		int i = exist[ti];
		if(adj[curr][i] <= 0 || flow[i] == 0 || time+adj[curr][i]+1 > 29) continue;
		
		int tmp = calcPressure(time + adj[curr][i], i);
		if(tmp > max) max = tmp;
	}
	
	for(int ti=0;ti<node_count;ti++){
		int i = exist[ti];
		if(adj[curr][i] >= 0 || flow[i] == 0 || time-adj[curr][i]+1 > 29) continue;
		
		int tmp = calcPressure(time - adj[curr][i], i);
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
	
	for(int ti=0;ti<node_count;ti++){
		int i=exist[ti];
		if(adj[curr][i] <= 0 || flow[i] == 0 || time+adj[curr][i]+1 >= 26 || i == other) continue;
		int new = time + adj[curr][i];
		int tmp;
		if(new < other_time) tmp = calcPressure2(new, i, other_time, other);
		else tmp = calcPressure2(other_time, other, new, i);
		if(tmp > max) max = tmp;
	}
	
	if(max == 0){
		for(int ti=0;ti<node_count;ti++){
			int i=exist[ti];
			if(adj[curr][i] >= 0 || flow[i] == 0 || time-adj[curr][i]+1 >= 26 || i == other) continue;
			int new = time - adj[curr][i];
			int tmp;
			if(new < other_time) tmp = calcPressure2(new, i, other_time, other);
			else tmp = calcPressure2(other_time, other, new, i);
			if(tmp > max) max = tmp;
		}
	}
	
	if(max == 0 && other_time != INT_MAX) max = calcPressure2(other_time, other, INT_MAX, 0);
	flow[curr] = myflow;
	
	return max + res; 
}
