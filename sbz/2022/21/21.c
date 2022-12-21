#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#define MAX_LINE_LENGTH 256

struct arr{
	double x[2];
};

struct monkeys{
	struct arr number;
	char op;
	struct monkeys *operators[2];
};


struct arr calc(struct monkeys *monkey){
	
	if(monkey->number.x[0]!=0 || monkey->number.x[1]) {
		return monkey->number;
	}
	
	struct arr x = calc(monkey->operators[0]);
	struct arr y = calc(monkey->operators[1]);
	switch(monkey->op){
		case('+'):
			monkey->number.x[0]=x.x[0]+y.x[0];
			monkey->number.x[1]=x.x[1]+y.x[1];
			break;
		case('-'):
			monkey->number.x[0]=x.x[0]-y.x[0];
			monkey->number.x[1]=x.x[1]-y.x[1];
			break;
		case('*'):
			monkey->number.x[0]=x.x[0]*y.x[0];
			monkey->number.x[1]=x.x[1]*y.x[0]+x.x[0]*y.x[1];
			break;
		case('/'):
			if(y.x[1] != 0){
				if(x.x[1] != 0){
					printf("Its on both sides!\n");
					exit(EXIT_FAILURE);
				}
				printf("Theyre dividing by it!\n");
				exit(EXIT_FAILURE);
			}
			monkey->number.x[0] = x.x[0]/y.x[0];
			monkey->number.x[1] = x.x[1]/y.x[0];
			break;
	}
	return monkey->number;
}



int main(int argc, char *argv[]){
	double res1=0, res2=1;
	char line[MAX_LINE_LENGTH];
	
	struct monkeys *monkeys = calloc(676*676, sizeof(struct monkeys));
	
	int humn;
	
	for(;;){
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line)<=1) break;
		
		int i=0, j;
		for(j=0;j<4;j++){
			i=i*26+line[j]-'a';
		}
		
		if(i == 136877){
			monkeys[i].number.x[1] = 1.0;
			humn = strtol(line+6, NULL, 10);
			continue;
		}
		
		if(line[6] <= '9'){
			monkeys[i].number.x[0] = (double) strtol(line+6, NULL, 10);
			continue;
		}
		
		monkeys[i].op = line[11];
		
		int k=0;
		for(j=0;j<4;j++){
			k=k*26+line[j+6]-'a';
		}
		
		monkeys[i].operators[0] = &monkeys[k];
		
		k=0;
		for(j=0;j<4;j++){
			k=k*26+line[j+13]-'a';
		}
		
		monkeys[i].operators[1] = &monkeys[k];
	}
	
	struct arr left = calc(monkeys[308639].operators[0]);
	struct arr right = calc(monkeys[308639].operators[1]);
	
	left.x[1] -= right.x[1];
	right.x[0] -= left.x[0];
	res2 = right.x[0]/left.x[1];
	
	struct arr part1 = calc(&monkeys[308639]);
	res1 = part1.x[0] + part1.x[1] * humn;
	
	printf("Part 1: %.0lf\nPart 2: %.0lf\n", res1, res2);
}
