#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#define MAX_LINE_LENGTH 128

struct Node{
	int x, y, dist;
	struct Node *next;
};

int main(int argc, char *argv[]){
	long res1=0, res2=0;
	char line[MAX_LINE_LENGTH][MAX_LINE_LENGTH];
	int width=0, height=0;
	
	for(;;height++){
		if(!fgets(line[height], MAX_LINE_LENGTH, stdin) || strlen(line[height]) <= 1) break;
		line[height][strlen(line[height])-1] = '\0';
		width = strlen(line[height]);
	}
	
	int xE=0, yE=0, xS=0, yS=0;
	
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			if(line[i][j] == 'E'){
				xE=j;
				yE=i;
				line[i][j] = 'z';
			}
			if(line[i][j] == 'S'){
				xS=j;
				yS=i;
				line[i][j] = 'a';
			}
		}
	}
	
	int dist;
	struct Node *curr = malloc(sizeof(*curr));
	curr->x = xE;
	curr->y = yE;
	curr->next = NULL;
	
	struct Node *next=NULL, **last=&next;
	
	for(;;){
		if(curr == NULL){
			curr = next;
			next = NULL;
			last = &next;
			dist++;
		}
		
		int x = curr->x, y=curr->y;
		
		if(res2 == 0 && line[y][x] == 'a') res2 = dist;
		
		if(y == yS && x == xS) break;
		char el = line[y][x] - 1;
		
		if(el == '\0'-1){
			struct Node *tmp=curr;
			curr=curr->next;
			free(tmp);
			continue;
		}
		
		line[y][x] = '\0';
		
		if(x > 0 && line[y][x-1] >= el){
			(*last) = malloc(sizeof(**last));
			(*last)->x=x-1;
			(*last)->y=y;
			(*last)->next=NULL;
			last=&((*last)->next);
		}
		if(y > 0 && line[y-1][x] >= el){
			(*last) = malloc(sizeof(**last));
			(*last)->x=x;
			(*last)->y=y-1;
			(*last)->next=NULL;
			last=&((*last)->next);
		}
		if(x < width-1 && line[y][x+1] >= el){
			(*last) = malloc(sizeof(**last));
			(*last)->x=x+1;
			(*last)->y=y;
			(*last)->next=NULL;
			last=&((*last)->next);
		}
		if(y < height-1 && line[y+1][x] >= el){
			(*last) = malloc(sizeof(**last));
			(*last)->x=x;
			(*last)->y=y+1;
			(*last)->next=NULL;
			last=&((*last)->next);
		}
		
		struct Node *tmp=curr;
		curr=curr->next;
		free(tmp);
	}
	res1 = dist;
	
	printf("Part 1: %ld\nPart 2: %ld\n", res1, res2);
}
