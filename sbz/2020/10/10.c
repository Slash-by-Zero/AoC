#include <stdio.h>
#include <stdlib.h>

void die(char *msg){
	perror(msg);
	exit(EXIT_FAILURE);
}

int icomp(const void *a, const void *b){
	return (* (int *) a) - (* (int *) b);
}

int count_solutions(int *, int);

int main(int argc, char *argv[]){
	int adapters_length=32, adapters_count=1;
	int *adapters=malloc(adapters_length*sizeof(*adapters));
	if(!adapters) die("malloc");
	
	adapters[0]=0;
	
	while(1){
		char line[5]="";
		
		char *retVal=fgets(line, 5, stdin);
		
		if(feof(stdin)) break;
		if(!retVal) die("fgets");
		
		if(adapters_count>=adapters_length){
			adapters_length+=32;
			adapters=realloc(adapters, adapters_length*sizeof(*adapters));
			if(!adapters) die("realloc");
		}
		
		adapters[adapters_count++]=strtol(line, NULL, 10);
	}
	
	if(adapters_count>=adapters_length){
		adapters_length++;
		adapters=realloc(adapters, adapters_length*sizeof(*adapters));
		if(!adapters) die("realloc");
	}
	
	qsort(adapters, adapters_count, sizeof(*adapters), icomp);
	
	adapters[adapters_count]=adapters[adapters_count-1]+3;
	adapters_count++;
	
	int inc_by_one=0, inc_by_three=0;
	unsigned long long total_solutions=1;
	int last_three_inc=0;
	
	for(int i=1;i<adapters_count;i++){
		if(adapters[i] == adapters[i-1]+1) inc_by_one++;
		if(adapters[i] == adapters[i-1]+3){
			inc_by_three++;
			total_solutions*=count_solutions(adapters+last_three_inc, i-last_three_inc);
			last_three_inc=i;
		}
	}
	
	
	
	printf("Part 1: %d * %d = %d\nPart 2: %lld\n", inc_by_one, inc_by_three, inc_by_one*inc_by_three, total_solutions>>1);
}

struct listel{
	int data;
	struct listel* next;
};

void add(struct listel **head, int el){
	struct listel **walk=head;
	
	while(*walk && (*walk)->data<el){
		walk=&((*walk)->next);
	}
	
	struct listel *new=malloc(sizeof(struct listel));
	if(!new) die("malloc");
	
	new->next=(*walk);
	new->data=el;
	
	(*walk)=new;
}

int count_solutions_helper(struct listel *head){
	if(!head){
		printf("Unexpted call with list of size < 2! Terminating further execution!\n");
		exit(EXIT_FAILURE);
	}
	if(!head->next) return 1;
	if(!head->next->next) return 1;
	int total=count_solutions_helper(head->next);
	
	struct listel *el=head->next;
	head->next=head->next->next;
	
	int diff=head->next->data-head->data;
	if(diff<3){
		total+=count_solutions_helper(head);
	}else if(diff==3){
		total+=count_solutions_helper(head->next);
	}
	
	head->next=el;
	
	return total;
}

int count_solutions(int *arr, int size){
	if(size < 2){
		return 1;
	}
	else if(size == 2){
		return 1;
	}
	
	struct listel *head=NULL;
	
	for(int i=0;i<size;i++){
		add(&head, arr[i]);
	}
	
	int retVal=count_solutions_helper(head);
	
	while(!head){
		struct listel *tmp=head;
		head=head->next;
		free(tmp);
	}
	
	return retVal;
}
