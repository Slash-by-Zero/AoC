#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 256

struct monkey{
	unsigned long items[100];
	int item_count;
	int test;
	int throw_to[2]; 
	int op_val;
	long activity;
	char op_type;
};

int main(int argc, char *argv[]){
	long res1=0, res2=0;
	
	char line[MAX_LINE_LENGTH];
	
	struct monkey monkeys[10];
	memset(monkeys, 0, sizeof(*monkeys) * 10);
	
	int i;
	
	for(i = 0;;){
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line) <= 1) break;
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line) <= 1) exit(EXIT_FAILURE);
		
		for(char *item=strtok(line+18, ", "); item; item = strtok(NULL, ", ")){
			monkeys[i].items[monkeys[i].item_count++] = strtol(item, NULL, 10);
		}
		
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line) <= 1) exit(EXIT_FAILURE);
		
		monkeys[i].op_type = line[23];
		monkeys[i].op_val = strtol(line+25, NULL, 10);
		
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line) <= 1) exit(EXIT_FAILURE);
		
		monkeys[i].test = strtol(line+21, NULL, 10);
		
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line) <= 1) exit(EXIT_FAILURE);
		
		monkeys[i].throw_to[1] = strtol(line+29, NULL, 10);
		
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line) <= 1) exit(EXIT_FAILURE);
		
		monkeys[i].throw_to[0] = strtol(line+30, NULL, 10);
		
		i++;
		if(!fgets(line, MAX_LINE_LENGTH, stdin)) break;
		else if(strlen(line) != 1) exit(EXIT_FAILURE);
	}
	
	struct monkey monkeys2[10];
	memcpy(monkeys2, monkeys, sizeof(*monkeys)*10);
	
	int gcd=1;
	for(int j=0;j<i;j++) gcd*=monkeys[j].test;
	
	int highest_index, highest_index2, turn;
	for(turn=0; turn<20;turn++){
		for(int j=0;j<i;j++){
			for(int item=0;item<monkeys[j].item_count;item++){
				switch(monkeys[j].op_type){
					case('+'):
						monkeys[j].items[item] += monkeys[j].op_val == 0 ? monkeys[j].items[item] : monkeys[j].op_val;
						break;
					case('*'):
						monkeys[j].items[item] *= monkeys[j].op_val == 0 ? monkeys[j].items[item] : monkeys[j].op_val;
				}
				monkeys[j].items[item] /= 3;
				int to = monkeys[j].throw_to[monkeys[j].items[item]%monkeys[j].test == 0];
				monkeys[to].items[monkeys[to].item_count++] = monkeys[j].items[item];
			}
			monkeys[j].activity+=monkeys[j].item_count;
			if(monkeys[j].activity > res1){
				highest_index = j;
				res1=monkeys[j].activity;
			}
			monkeys[j].item_count=0;
		}
	}
	
	for(turn = 0; turn<10000; turn++){
		for(int j=0;j<i;j++){
			for(int item=0;item<monkeys2[j].item_count;item++){
				if(monkeys2[j].items[item] < 0) printf("Alarm");
				switch(monkeys2[j].op_type){
					case('+'):
						monkeys2[j].items[item] += monkeys2[j].op_val == 0 ? monkeys2[j].items[item] : monkeys2[j].op_val;
						break;
					case('*'):
						monkeys2[j].items[item] *= monkeys2[j].op_val == 0 ? monkeys2[j].items[item] : monkeys2[j].op_val;
				}
				monkeys2[j].items[item] %= gcd;
				int to = monkeys2[j].throw_to[monkeys2[j].items[item]%monkeys2[j].test == 0];
				monkeys2[to].items[monkeys2[to].item_count++] = monkeys2[j].items[item];
			}
			monkeys2[j].activity+=monkeys2[j].item_count;
			if(monkeys2[j].activity > res2){
				highest_index2 = j;
				res2=monkeys2[j].activity;
			}
			monkeys2[j].item_count=0;
		}
	}
	
	int getSecond=0, getSecond2=0;;
	for(int j=0;j<i;j++) {
		if(j != highest_index && monkeys[j].activity > getSecond) getSecond = monkeys[j].activity;
		if(j != highest_index2 && monkeys2[j].activity > getSecond2) getSecond2 = monkeys2[j].activity;
	}
	res1 = res1*getSecond;
	res2 *= getSecond2;
	
	printf("Part 1: %ld\nPart 2: %ld\n", res1, res2);
}
