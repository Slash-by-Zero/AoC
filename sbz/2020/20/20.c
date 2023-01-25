#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void die(char *msg){
	perror(msg);
	abort();
}

struct tile{
	int n;
	char img[8][8];
	struct tile *neighbours[4];
};

#define MAX_LINE_LENGTH 128

int main(){
	char line[MAX_LINE_LENGTH];
	struct{
		int n;
		struct tile *t;
	} edgeCode[1024] = {0};
	struct tile *new[128];
	int count=0;
	for(;;){
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line) <= 1) break;
		new[count] = calloc(1, sizeof(**new));
		new[count]->n = strtol(line+5, NULL, 10);
		
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line) <= 1) break;
		int edges[4][2] = {0};
		for(int i=0;i<strlen(line)-1;i++){
			edges[0][0] <<=1;
			edges[0][0] |= line[i] == '#';
			edges[0][1] |= (line[i] == '#') << i;
		}
		
		edges[3][0] |= line[0] == '#';
		edges[3][1] |= line[0] == '#';
		
		edges[1][0] |= line[strlen(line)-2] == '#';
		edges[1][1] |= line[strlen(line)-2] == '#';
		
		for(int i=1;i<=8;i++){
			if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line) <= 1) break;
			
			memcpy(new[count].img[i-1], line+1, 8);
			
			edges[3][0] <<=1;
			edges[3][0] |= line[0] == '#';
			edges[3][1] |= (line[0] == '#') << i;
			
			edges[1][1] <<=1;
			edges[1][1] |= line[strlen(line)-2] == '#';
			edges[1][0] |= (line[strlen(line)-2] == '#') << i;
		}
		
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line) <= 1) break;
		
		edges[3][0] <<=1;
		edges[3][0] |= line[0] == '#';
		edges[3][1] |= (line[0] == '#') << 9;
		
		edges[1][1] <<=1;
		edges[1][1] |= line[strlen(line)-2] == '#';
		edges[1][0] |= (line[strlen(line)-2] == '#') << 9;
		
		for(int i=0;i<strlen(line)-1;i++){
			edges[2][1] <<=1;
			edges[2][1] |= line[i] == '#';
			edges[2][0] |= (line[i] == '#') << i;
		}
		
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line) <= 1) break;
		
		for(int i=0;i<4;i++){
			if(edgeCode[edges[i][0]].t){
				edgeCode[edges[i][0]].t->neighbours[edgeCode[edges[i][0]].n] = new[count];
				new[count]->neighbours[i] = edgeCode[edges[i][0]].t;
			}
			else{
				edgeCode[edges[i][0]].t = new[count];
				edgeCode[edges[i][0]].n = i;
				edgeCode[edges[i][1]].t = new[count];
				edgeCode[edges[i][1]].n = i;
			}
		}
		count++;
	}
	if(ferror(stdin)) die("fgets");
	
	long res1=1;
	
	int width = (int) sqrt((double) count);
	char img[width][width];
	
	struct tile *corner= NULL;
	int rot=0;
	for(int i=0;i<count;i++){
		int ns = 0;
		for(int j=0;j<4;j++) {
			if(new[i]->neighbours[j]){
				ns++;
				if(!new->neighbours[(j+1)&3]) rot = j;
			}
		}
		if(ns == 2) {
			corner = new[i];
			break;
		}
	}
	
	int flip = 0;
	
	for(struct tile *walk1 = corner; walk1;){
		struct tile *next = walk1->neighbours[rot];
		
		for(int j=0;j<4;j++) {
			if(new[i]->neighbours[j^2] == walk1){
				rot = j;
				break;
			}
		}
		
		flip = new[i]->neighbours[(rot+1)&3] == NULL;
		
		
		
		walk1 = next;
	}
	
	printf("Part 1: %ld\n", res1);
}
