#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#define MAX_LINE_LENGTH 128

struct cyclicList{
	int data;
	int mixed;
	struct cyclicList *next, *prev, *equiv;
};

int main(int argc, char *argv[]){
	long res1=0, res2=0;
	char line[MAX_LINE_LENGTH];
	
	struct cyclicList zero, *start=NULL;
	zero.data = 0;
	zero.mixed = 1;
	zero.next = &zero;
	zero.prev = &zero;
	
	struct cyclicList zero2, *start2 = NULL;
	zero2.data=0;
	zero2.mixed=1;
	zero2.next = &zero2;
	zero2.prev = &zero2;
	
	zero.equiv = &zero2;
	
	int before = 1;
	int total=0;
	
	for(;;){
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line)<=1) break;
		
		long curr = strtol(line, NULL, 10);
		total++;
		
		if(curr==0){
			start = zero.next;
			start2 = zero2.next;
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
			
			struct cyclicList *new2 = malloc(sizeof(*new));
			
			new2->data = curr;
			new2->mixed=0;
			
			new2->prev = zero2.prev;
			zero2.prev->next = new2;
			new2->next = &zero2;
			zero2.prev = new2;
			
			new->equiv = new2;
		}
		else{
			struct cyclicList *new = malloc(sizeof(*new));
			
			new->data = curr;
			new->mixed = 0;
			
			new->next = start;
			new->prev = start->prev;
			start->prev->next = new;
			start->prev = new;
			
			struct cyclicList *new2 = malloc(sizeof(*new));
			
			new2->data = curr;
			new2->mixed = 0;
			
			new2->next = start2;
			new2->prev = start2->prev;
			start2->prev->next = new2;
			start2->prev = new2;
			
			new->equiv = new2;
		}
		
		
	}
	
	long key=811589153;
	
	for(int round=0;round<10;round++){
		int count=0;
		for(struct cyclicList *curr1=start;;){
			struct cyclicList *curr=curr1->equiv, *insertBefore=curr->next;
			int tmp=(curr->data*key)%(total-1);
			
			curr->next->prev=curr->prev;
			curr->prev->next=curr->next;
			
			while(tmp < 0){
				insertBefore = insertBefore->prev;
				tmp++;
			}
			while(tmp > 0){
				insertBefore=insertBefore->next;
				tmp--;
			}
			
			
			curr->next = insertBefore;
			curr->prev = insertBefore->prev;
			curr->next->prev = curr;
			curr->prev->next = curr;
			
			
			count++;
			if(count==total) break;
			curr1 = curr1->next;
		}
	}
	
	int places[3];
	for(int i=0;i<3;i++){
		places[i] = ((i+1)*1000)%total;
	}
	
	struct cyclicList *walk=&zero2;
	for(int i=0;i<total;i++){
		for(int j=0;j<3;j++){
			if(places[j] == i){
				res2 += (walk->data*key);
			}
		}
		walk = walk->next;
	}
	
	int count=1;
	for(struct cyclicList *curr=start;;){
		struct cyclicList *insertBefore=curr->next, *next=curr->next;
		int tmp=curr->data%(total-1);
		
		curr->next->prev=curr->prev;
		curr->prev->next=curr->next;
		
		while(tmp < 0){
			insertBefore = insertBefore->prev;
			tmp++;
		}
		while(tmp > 0){
			insertBefore=insertBefore->next;
			tmp--;
		}
		
		
		curr->next = insertBefore;
		curr->prev = insertBefore->prev;
		curr->next->prev = curr;
		curr->prev->next = curr;
		
		curr->mixed=1;
		count++;
		if(count==total) break;
		curr = next;
		while(curr->mixed) {
			curr = curr->next;
		}
	}
	
	walk=&zero;
	for(int i=0;i<total;i++){
		for(int j=0;j<3;j++){
			if(places[j] == i){
				res1 += walk->data;
			}
		}
		walk = walk->next;
	}
	
	printf("Part 1: %ld\nPart 2: %ld\n", res1, res2);
}
