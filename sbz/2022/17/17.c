#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#define MAX_LINE_LENGTH 128

struct data{
	int cycle, max, top[14][7];
};

int hasCycle(struct data *, int);

int main(int argc, char *argv[]){
	long res1=0, res2=0;
	
	int cave[100*2022][7];
	memset(cave, 0, 100*2022*7*sizeof(int));
	long max = 0;
	
	int blocks[5][4][4] = {
		{ 	{1,1,1,1},
			{0,0,0,0},
			{0,0,0,0},
			{0,0,0,0}
		},
		{ 	{0,1,0,0},
			{1,1,1,0},
			{0,1,0,0},
			{0,0,0,0}
		},
		{ 	{1,1,1,0},
			{0,0,1,0},
			{0,0,1,0},
			{0,0,0,0}
		},
		{ 	{1,0,0,0},
			{1,0,0,0},
			{1,0,0,0},
			{1,0,0,0}
		},
		{ 	{1,1,0,0},
			{1,1,0,0},
			{0,0,0,0},
			{0,0,0,0}
		}
	};
	
	struct data tracker[1024];
	int tracker_count=1;
	tracker[0].cycle=0;
	tracker[0].max=0;
	
	long cycle=0, x, y=-1;
	long term_on=1000000000000;
	for(;;){
		int c=fgetc(stdin);
		if(c == EOF || c == '\n'){
			if(cycle > 2022){
				tracker[tracker_count].cycle = cycle;
				tracker[tracker_count].max = max;
				
				for(int i=0;i<14;i++){
					for(int j=0;j<7;j++){
						tracker[tracker_count].top[i][j] = cave[max-i][j];
					}
				}
				
				tracker_count++;
				
				int length = hasCycle(&tracker[0], tracker_count);
				
				if(length != 0){
					int prev_diff = tracker[tracker_count-1].cycle - tracker[tracker_count-1-length].cycle;
					int max_diff = tracker[tracker_count-1].max - tracker[tracker_count-1-length].max;
					
					long tmp = (1000000000000-cycle)/prev_diff;
					res2 = tmp * (max_diff);
					term_on = cycle + (1000000000000-cycle)%prev_diff;
				}
			}
			
			rewind(stdin);
			c = fgetc(stdin);
		}
		
		int block = cycle%5;
		
		if(y == -1){
			y=max+3;
			x=2;
		}
		
		int push=1;
		int move=1;
		
		x += c - '=';
		
		for(int i=0;i<4;i++){
			if(x<0){
				push=0;
				break;
			}
			for(int j=0;j<4;j++){
				if((i+x >= 7 && blocks[block][j][i]) || (i+x < 7 && blocks[block][j][i] && cave[j+y][i+x])){
					push=0;
					goto break_push;
				}
			}
		}
break_push:
		
		if(!push) x -= c - '=';
		
		y--;
		
		for(int i=0;i<4;i++){
			if(y<0){
				move=0;
				break;
			}
			for(int j=0;j<4;j++){
				if((i+x < 7 && blocks[block][j][i] && cave[j+y][i+x])){
					move=0;
					goto break_move;
				}
			}
		}
break_move:
		
		if(move == 0){
			y++;
			for(int i=0;i<4;i++){
				for(int j=0;j<4;j++){
					if(i+x < 7) cave[j+y][i+x] |= blocks[block][j][i];
					if(blocks[block][j][i] && j+y+1 > max) max=j+y+1;
				}
			}
			
			
			y = -1;
			cycle++;
			if(cycle == 2022) res1 = max; 
			if(cycle == term_on) break; 
		}
	}
	
	res2 += max;
	
	printf("Part 1: %ld\nPart 2: %ld\n", res1, res2);
}

int hasCycle(struct data *data, int count){
	for(int i=1; i<(count+1)/2;i++){
		int sumc1 = data[count-1].cycle - data[count-1-i].cycle;
		int sumc2 = data[count-1-i].cycle - data[count-1-2*i].cycle;
		int summ1 = data[count-1].max - data[count-1-i].max;
		int summ2 = data[count-1-i].max - data[count-1-2*i].max;
		
		
		if(sumc1 == sumc2 && summ1 == summ2){
			int topSame=1;
			for(int k=0;k<14;k++){
				for(int j=0;j<7;j++){
					if(data[count-1].top[k][j] != data[count-1-i].top[k][j]) topSame=0;
				}
			}
			if(topSame) return i;
		}
	}
	return 0;
}
