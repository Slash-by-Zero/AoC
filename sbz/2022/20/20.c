#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#define MAX_LINE_LENGTH 128

struct cyclicList{
	int data;
	int mixed;
	struct cyclicList *next, *prev;
};

static void printList(struct cyclicList *start){
	for(struct cyclicList *walk=start; walk != start->prev; walk=walk->next){
		printf("%d, ", walk->data);
	}
	printf("%d\n", start->prev->data);
}

int main(int argc, char *argv[]){
	long res1=0, res2=0;
	char line[MAX_LINE_LENGTH];
	
	struct cyclicList zero, *start=NULL;
	zero.data = 0;
	zero.mixed = 1;
	zero.next = &zero;
	zero.prev = &zero;
	
	int before = 1;
	int total;
	
	for(;;){
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line)<=1) break;
		
		int curr = strtol(line, NULL, 10);
		total++;
		
		if(curr==0){
			start = zero.next;
			before = 0;
		}
		else if(before){
			struct cyclicList *new = malloc(sizeof(*new));
			
			new->data = curr;
			new->mixed=0;
			
			new->prev = zero.prev;
			zero.prev->next = new;
			new->next = &zero;
			zero.prev = new;
		}
		else{
			struct cyclicList *new = malloc(sizeof(*new));
			
			new->data = curr;
			new->mixed = 0;
			
			new->next = start;
			new->prev = start->prev;
			start->prev->next = new;
			start->prev = new;
		}
	}
	
	
	for(struct cyclicList *curr=start;;){
		struct cyclicList *insertBefore=curr, *next=curr->next;
		int tmp=curr->data;
		while(tmp < 0){
			insertBefore = insertBefore->prev;
			tmp++;
		}
		while(tmp > 0){
			insertBefore=insertBefore->next;
			tmp--;
		}
		if(curr->data > 0) insertBefore=insertBefore->next;
		curr->next->prev=curr->prev;
		curr->prev->next=curr->next;
		
		curr->next = insertBefore;
		curr->prev = insertBefore->prev;
		curr->next->prev = curr;
		curr->prev->next = curr;
		
		curr->mixed=1;
		curr = next;
		while(curr->mixed) {
			curr = curr->next;
			if(curr == next) goto mixed_all;
		}
	}
mixed_all:;
	
	int places[3];
	for(int i=1;i<=3;i++){
		places[i-1] = (i*1000)%total;
	}
	
	struct cyclicList *walk=&zero;
	for(int i=0;i<total;i++){
		for(int j=0;j<3;j++){
			if(places[j] == i){
				printf("%d\n", walk->data);
				res1 += walk->data;
			}
		}
		walk = walk->next;
	}
	
	printf("Part 1: %ld\nPart 2: %ld\n", res1, res2);
}
