#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#define MAX_LINE_LENGTH 128

static int move[5][2] = {{0,1},{1,0},{0,-1},{-1,0},{0,0}};

struct blizzard{
	int x, y, dir;
	struct blizzard *next;
};

static struct blizzard *push(struct blizzard *head, struct blizzard *new){
	new->next=head;
	return new;
}

static void moveAndEnter(struct blizzard *head, int **map, int width, int height){
	head->x += move[head->dir][1];
	head->y += move[head->dir][0];
	head->x = (head->x+width)%width;
	head->y = (head->y+height)%height;
	map[head->y][head->x] = -1;
	
	if(head->next) moveAndEnter(head->next, map, width, height);
}

static int moveFrom(int xF, int yF, int xT, int yT, int turn, struct blizzard *head, int width, int height){
	int **map = calloc(height, sizeof(*map));
	for(int i=0;i<height;i++) map[i] = calloc(width, sizeof(**map));
	
	for(;map[yT][xT] != 1;turn++){
		int **newmap = calloc(height, sizeof(*map));
		
		for(int i=0;i<height;i++){
			newmap[i] = calloc(width, sizeof(**map));
		}
		
		moveAndEnter(head, newmap, width, height);
		
		for(int i=0;i<height;i++){
			for(int j=0;j<width;j++){
				if(map[i][j] != 1) continue;
				for(int dir=0;dir<5;dir++){
					int y=i+move[dir][0];
					int x=j+move[dir][1];
					if(y<0 || x<0 || y>= height || x >= width || newmap[y][x] == -1) continue;
					newmap[y][x] = 1;
				}
			}
		}
		
		if(newmap[yF][xF] == 0) newmap[yF][xF] = 1;
		
		for(int i=0;i<height;i++){
			free(map[i]);
		}
		
		//printf("Minute %d:\n", turn);
		//printMap(newmap, width, height);
		
		free(map);
		map = newmap;
	}
	
	return turn;
}

int main(int argc, char *argv[]){
	long res1=0, res2=0;
	char line[MAX_LINE_LENGTH];
	
	int width=0;
	int height=0;
	
	struct blizzard *head=NULL;
	
	fgets(line, MAX_LINE_LENGTH, stdin);
	
	for(;;height++){
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line) <= 1) break;
		
		width = strlen(line)-3;
		
		for(int i=0;i < (int) (strlen(line)-3);i++){
			int dir=0;
			switch(line[i+1]){
				case('^'):
					dir++;
				case('<'):
					dir++;
				case('v'):
					dir++;
				case('>'):;
					struct blizzard *new = calloc(1, sizeof(*new));
					new->x=i;
					new->y=height;
					new->dir=dir;
					head = push(head, new);
					break;
			}
		}
	}
	
	height--;
	
	res1 = moveFrom(0,0,width-1, height-1,1,head,width,height);
	res2 = moveFrom(width-1, height-1,0,0,res1,head,width,height);
	res2 = moveFrom(0,0,width-1, height-1,res2,head,width,height);
	
	printf("Part 1: %ld\nPart 2: %ld\n", res1, res2);
}
