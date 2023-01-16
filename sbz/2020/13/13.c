#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


struct bus{
	int id, offset;
	struct bus *next;
};

int main(){
	int res1 = INT_MAX;
	unsigned long res2 = 0;
	
	int departTime = 0;
	for(int c = fgetc(stdin); c != '\n' && c != EOF; c = fgetc(stdin)){
		departTime = departTime * 10 + c - '0';
	}
	
	departTime--;
	
	struct bus *first = NULL;
	
	{
		int diff = -1;
		
		struct bus **curr = &first;
		
		for(int c = fgetc(stdin); c != '\n' && c != EOF; c = fgetc(stdin)){
			diff++;
			if(c == 'x'){
				c = fgetc(stdin);
				continue;
			}
			
			int id = c - '0';
			for(c = fgetc(stdin); c != '\n' && c != ',' && c != EOF; c = fgetc(stdin)){
				id = id * 10 + c - '0';
			}
			
			*curr = malloc(sizeof(**curr));
			(*curr)->id = id;
			(*curr)->offset = diff % id;
			curr = &(*curr)->next;
			*curr = NULL;
		}
	}
	
	int firstId = 0;
	long cycle = 1;
	
	for(struct bus *walk = first; walk; walk = walk->next){
		long nextDepart = walk->id - departTime % walk->id;
		
		if(res1 > nextDepart){
			res1 = nextDepart;
			firstId = walk->id;
		}
		
		nextDepart = walk->id - res2 % walk->id;
		if(nextDepart == walk->id){
			nextDepart = 0;
		}
		
		while(nextDepart != walk->offset){
			res2 += cycle;
			nextDepart = walk->id - res2 % walk->id;
		}
		
		cycle *= walk->id;
	}
	
	res1 = (res1 - 1) * firstId;
	
	printf("Part 1: %d\nPart 2: %ld\n", res1, res2);
}
