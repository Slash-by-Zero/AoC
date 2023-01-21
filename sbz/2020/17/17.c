#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

static void die(char *msg){
	perror(msg);
	abort();
}

struct area{
	int bounds[4][2];
	int mem[];
};

static struct area *dupArea(const struct area *old){
	size_t size = 1;
	for(int i=0;i<4;i++){
		size *= old->bounds[i][1];
	}
	struct area *new = malloc(size * sizeof(*new->mem) + sizeof(*new));
	if(!new) die("malloc");
	memcpy(new, old, size *sizeof(*new->mem) + sizeof(*new));
	
	return new;
}

static struct area *initArea(int bounds[4][2]){
	size_t size = 1;
	for(int i=0;i<4;i++){
		size *= bounds[i][1] - bounds[i][0];
	}
	struct area *new = malloc(size * sizeof(*new->mem) + sizeof(*new));
	if(!new) die("malloc");
	memset(new, 0, size * sizeof(*new->mem) + sizeof(*new));
	memcpy(new->bounds, bounds, 4*2*sizeof(**bounds));
	
	for(int i=0;i<4;i++){
		new->bounds[i][1] -= bounds[i][0];
	}
	
	return new;
}

static void destroyArea(struct area *this){
	free(this);
}

static int get(struct area *this, const int x[4]){
	int acces = 0;
	for(int i=0;i<4;i++){
		int tmp = x[i] - this->bounds[i][0];
		if(tmp < 0 || tmp >= this->bounds[i][1]) return 0;
		acces = acces * this->bounds[i][1] + tmp;
	}
	return this->mem[acces];
}

static void set(struct area *this, const int x[4]){
	int acces = 0;
	for(int i=0;i<4;i++){
		int tmp = x[i] - this->bounds[i][0];
		if(tmp < 0 || tmp >= this->bounds[i][1]){
			errno = EINVAL;
			die("set");
		}
		acces = acces * this->bounds[i][1] + tmp;
	}
	this->mem[acces] = 1;
}

int simLife(int dim, const struct area *init, int cycles, int bounds_init[4][2]){
	struct area *new = dupArea(init), *old = NULL;
	int bounds[4][2], x[4];
	memcpy(bounds, bounds_init, 4*2*sizeof(**bounds));
	
	for(int turn=1;;turn++){
		destroyArea(old);
		old = new;
		for(int i =4-dim;i<4;i++){
			bounds[i][0]--;
			bounds[i][1]++;
		}
		new = initArea(bounds);
		
		int new_bounds[4][2];
		for(int i=0;i<4;i++){
			x[i] = bounds[i][0];
			new_bounds[i][0] = INT_MAX;
			new_bounds[i][1] = INT_MIN;
		}
		
		int blocks = 0, blocks_p1 = 0;
		for(;x[0] < new->bounds[0][1];){
			
			int tmp[4];
			int n = 0;
			for(tmp[0] = x[0]-1; tmp[0]<=x[0]+1;tmp[0]++){
				for(tmp[1] = x[1]-1; tmp[1]<=x[1]+1;tmp[1]++){
					for(tmp[2] = x[2]-1; tmp[2]<=x[2]+1;tmp[2]++){
						for(tmp[3] = x[3]-1; tmp[3]<=x[3]+1;tmp[3]++){
							n += get(old, tmp);
						}
					}
				}
			}
			
			if(n == 3 || (get(old, x) && n == 4)){
				set(new, x);
				blocks++;
				if(x[0] == 0) blocks_p1++;
				for(int i=0;i<4;i++){
					if(x[i] < new_bounds[i][0]) new_bounds[i][0] = x[i];
					if(x[i] >= new_bounds[i][1]) new_bounds[i][1] = x[i] + 1;
				}
			}
			
			x[3]++;
			for(int i=3;i>0;i--){
				if(x[i] == bounds[i][1]){
					x[i] = bounds[i][0];
					x[i-1]++;
				}
				else break;
			}
		}
		
		memcpy(bounds, new_bounds, 4*2*sizeof(**bounds));
		
		if(turn == cycles){
			return blocks;
		}
	}
}

#define MAX_LINE_LENGTH 128

int main(){
	char line[MAX_LINE_LENGTH];
	
	struct area *new;
	int bounds[4][2] = {{0,1},{0,1}, {0,0}, {0,0}};
	
	if(!fgets(line, MAX_LINE_LENGTH, stdin)) die("fgets");
	bounds[2][1] = strlen(line) - 1;
	bounds[3][1] = strlen(line) - 1;
	new = initArea(bounds);
	
	char *rVal = line;
	int x[4] = {0,0,0};
	for(; rVal && strlen(line) > 1; x[2]++){
		x[3] = 0;
		for(;x[3] < strlen(line) - 1; x[3]++){
			if(line[x[3]] == '#'){
				set(new, x);
			}
		}
		rVal = fgets(line, MAX_LINE_LENGTH, stdin);
	}
	if(ferror(stdin)) die("fgets");
	
	int res1 = simLife(3, new, 6, bounds);
	int res2 = simLife(4, new, 6, bounds);
	
	printf("Part 1: %d\nPart 2: %d\n", res1, res2);
}
