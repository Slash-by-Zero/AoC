#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sim(int *cups, int ncups, int turns, int curr){
	for(int i = 0; i<turns;i++){
		int dest = curr - 1;
		if(dest < 0) dest = ncups-1;
		int next = cups[cups[cups[cups[curr]]]];
		for(int j = cups[curr]; j != next; j = cups[j]){
			if(dest == j){
				if(dest-- == 0) dest = ncups-1;
				j = curr;
			}
		}
		cups[cups[cups[cups[curr]]]] = cups[dest];
		cups[dest] = cups[curr];
		cups[curr] = next;
		curr = next;
	}
}

int main(){
	int cups_1[9];
	int cups_2[1000000];
	
	int curr = fgetc(stdin) - '1';
	int i=curr;
	for(int c = fgetc(stdin); c!=EOF && c!='\n'; c = fgetc(stdin)){
		c = c - '1';
		cups_1[i] = c;
		i = c;
	}
	cups_1[i] = curr;
	
	memcpy(cups_2, cups_1, sizeof(cups_1));
	
	cups_2[i] = 9;
	
	for(i = 9;i<1000000;i++){
		cups_2[i] = i+1;
	}
	cups_2[999999] = curr;
	
	sim(cups_1, 9, 100, curr);
	sim(cups_2, 1000000, 10000000, curr);
	
	printf("Part 1: ");
	for(int j = cups_1[0];j != 0; j = cups_1[j]){
		printf("%c", j + '1');
	}
	
	printf("\nPart 2: %ld\n",  (cups_2[0]+1) * ((long) cups_2[cups_2[0]]+1));
}
