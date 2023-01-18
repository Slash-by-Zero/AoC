#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void die(char *msg){
	perror(msg);
	abort();
}

int main(){
	int nums_size = 1000;
	int *nums = calloc(nums_size, sizeof(*nums));
	if(!nums) die("calloc");
	
	int turn = 1;
	
	int n=0;
	for(int c=fgetc(stdin); c != '\n' && c != EOF; c = fgetc(stdin)){
		if(c == ','){
			nums[n] = turn++;
			n=0;
			continue;
		}
		n = n * 10 + c - '0';
	}
	if(ferror(stdin)) die("fgetc");
	
	for(;turn < 30000000;turn++){
		int newn = 0;
		
		while(n >= nums_size){
			nums = realloc(nums, (nums_size+1000) * sizeof(*nums));
			if(!nums) die("realloc");
			memset(nums+nums_size, 0, 1000 * sizeof(*nums));
			nums_size+=1000;
		}
		if(nums[n] != 0){
			newn = turn - nums[n];
		}
		nums[n] = turn;
		n = newn;
		
		if(turn == 2019){
			printf("Part 1: %d\n", n);
		}
	}
	
	printf("Part 2: %d\n", n);
}
