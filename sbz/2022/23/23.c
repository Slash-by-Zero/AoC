#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#define MAX_LINE_LENGTH 512

static int move[8][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}, {1,1},{-1,-1},{1,-1},{-1,1}};

static int maxx, maxy, minx, miny, change;

static int *getNeighbours(int dir[2], int x, int y){
	static int tmp[3][2];
	tmp[0][0] = x+dir[1];
	tmp[0][1] = y+dir[0];
	tmp[1][0] = x+dir[1]-dir[0];
	tmp[1][1] = y+dir[0]-dir[1];
	tmp[2][0] = x+dir[1]+dir[0];
	tmp[2][1] = y+dir[0]+dir[1];
	return (int *) tmp;
}

static int *getAllNeighbours(int x, int y){
	static int tmp[8][2];
	for(int i=0;i<8;i++){
		tmp[i][0] = x+move[i][0];
		tmp[i][1] = y+move[i][1];
	}
	return (int *) tmp;
}

struct elf{
	int x, y;
	struct elf *left, *right;
};
static int prios[4] = {0,1,2,3};

static struct elf *add(struct elf *top, struct elf *new){
	if(!top) return new;
	
	if(top->x < new->x || (top->x == new->x && top->y < new->y)) top->left = add(top->left, new);
	else if(top->x == new->x && top->y == new->y) return top;
	else top->right = add(top->right, new);
	
	return top;
}

static struct elf *removeElf(struct elf *top, struct elf *remove){
	if(!top) return NULL;
	
	if(top == remove){
		if(top->right){
			struct elf **walk=&(top->right);
			while((*walk)->left) walk= &(*walk)->left;
			struct elf *tmp = *walk;
			*walk = (*walk)->right;
			tmp->right = top->right;
			tmp->left = top->left;
			return tmp;
		}
		else if(top->left){
			struct elf **walk=&(top->left);
			while((*walk)->right) walk= &(*walk)->right;
			struct elf *tmp = *walk;
			*walk = (*walk)->left;
			tmp->right = top->right;
			tmp->left = top->left;
			return tmp;
		}
		else return NULL;
	}
	else if(top->x < remove->x || (top->x == remove->x && top->y < remove->y)) top->left = removeElf(top->left, remove);
	else top->right = removeElf(top->right, remove);
	
	return top;
}

static struct elf *get(struct elf *top, int x, int y){
	if(!top) return NULL;
	if(top->x == x && top->y == y) return top;
	
	if(top->x < x || (top->x == x && top->y < y)){
		if(top->left) return get(top->left, x, y);
		return NULL;
	}
	
	if(top->right) return get(top->right, x, y);
	return NULL;
}

static void freeElf(struct elf *top){
	if(!top) return;
	freeElf(top->right);
	freeElf(top->left);
	free(top);
}

static struct elf *turnInternal(struct elf *proposals, struct elf *top, struct elf *curr){
	int lonely=1, blocked=1;
	{
		int *nbs = getAllNeighbours(curr->x, curr->y);
		for(int i=0;i<8;i++){
			if(get(top, nbs[i*2], nbs[i*2+1])) lonely=0;
		}
	}
	if(!lonely){
		for(int i=0;i<4;i++){
			int dirFree=1;
			int *nbs = getNeighbours(move[prios[i]], curr->x, curr->y);
			for(int j=0;j<3;j++){
				if(get(top, nbs[j*2], nbs[j*2+1])) dirFree = 0;
			}
			
			if(!dirFree) continue;
			
			blocked=0;
			i = prios[i];
			
			struct elf *to = get(proposals, curr->x+move[i][1], curr->y+move[i][0]);
			
			if(to){
				blocked=1;
				
				proposals = removeElf(proposals, to);
				
				to->x+=move[i][1];
				to->y+=move[i][0];
				
				to->left = NULL;
				to->right = NULL;
				
				if(to->x>maxx) maxx=to->x;
				if(to->x<minx) minx=to->x;
				if(to->y>maxy) maxy=to->y;
				if(to->y<miny) miny=to->y;
				
				proposals = add(proposals, to);
				
				break;
			}
			
			struct elf *new=calloc(1,sizeof(*new));
			
			change=1;
			
			new->x=curr->x+move[i][1];
			new->y=curr->y+move[i][0];
			
			if(new->x>maxx) maxx=new->x;
			if(new->x<minx) minx=new->x;
			if(new->y>maxy) maxy=new->y;
			if(new->y<miny) miny=new->y;
			
			proposals = add(proposals, new);
			break;
		}
	}
	if(lonely || blocked){
		struct elf *new=malloc(sizeof(*new));
		
		memcpy(new, curr, sizeof(*new));
		new->left=NULL;
		new->right=NULL;
		
		if(new->x>maxx) maxx=new->x;
		if(new->x<minx) minx=new->x;
		if(new->y>maxy) maxy=new->y;
		if(new->y<miny) miny=new->y;
		
		proposals = add(proposals, new);
	}
	if(curr->right) proposals = turnInternal(proposals, top, curr->right);
	if(curr->left) proposals = turnInternal(proposals, top, curr->left);
	return proposals;
}

static struct elf *turn(struct elf *top){
	minx=INT_MAX; miny = INT_MAX; maxx = INT_MIN; maxy = INT_MIN;
	change=0;
	
	struct elf *new = turnInternal(NULL, top, top);
	freeElf(top);
	int tmp=prios[0];
	
	memcpy(prios, prios+1, 12);
	prios[3]=tmp;
	return new;
}

int main(int argc, char *argv[]){
	long res1=0, res2=0;
	char line[MAX_LINE_LENGTH];
	
	struct elf *top = NULL;
	
	for(int y=0;;y++){
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line) <= 1) break;
		
		
		for(int i=0;i<strlen(line);i++){
			int ti = strlen(line)/2 + ((i%2)*2-1) * ((i+1)/2);
			if(line[ti] == '#'){
				struct elf *new = calloc(1, sizeof(*top));
				new->x = ti;
				new->y = y;
				res1--;
				top = add(top, new);
			}
		}
	}
	
	for(int i=0;i<10;i++) top = turn(top);
	
	res1 += (maxx-minx+1) * (maxy-miny+1);
	
	for(res2=10;change;res2++) top=turn(top);
	
	printf("Part 1: %ld\nPart 2: %ld\n", res1, res2);
}
