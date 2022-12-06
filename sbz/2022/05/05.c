#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct stack{
	char el;
	struct stack *next;
};

int main(int argc, char *argv[]){
	char  line[128];
	
	fgets(line, 128, stdin);
	
	int stack_count = (strlen(line)+1)/4;
	
	struct stack *stacks[stack_count];
	memset(stacks, 0, stack_count*sizeof(*stacks));
	struct stack *stacks2[stack_count];
	memset(stacks2, 0, stack_count*sizeof(*stacks2));
	
	for(;;){
		
		for(int i=0;i<strlen(line);i+=4){
			if(line[i] == '['){
				struct stack *new=malloc(sizeof(*new));
				new->next = NULL;
				new->el = line[i+1];
				
				struct stack **end;
				for(end = &stacks[i/4];*end;end = &((*end)->next)){}
				*end = new;
				
				new=malloc(sizeof(*new));
				new->next = NULL;
				new->el = line[i+1];
				
				for(end = &stacks2[i/4];*end;end = &((*end)->next)){}
				*end = new;
			}
		}
		
		if(!fgets(line, 128, stdin)){
			break;
		}
		if(line[1] == '1') break;
	}
	
	fgets(line, 128, stdin);
	
	for(;;){
		if(!fgets(line, 128, stdin) || line[0] == '\n') break;
		
		char *next;
		int count = strtol(line+5, &next, 10);
		int from = strtol(next+6, &next, 10)-1;
		int to = strtol(next+4, NULL, 10)-1;
		
		struct stack *end = stacks2[from];
		for(int i=1;i<count;i++){
			end =end->next;
		}
		struct stack *tmp = stacks2[from];
		stacks2[from] = end->next;
		end->next = stacks2[to];
		stacks2[to] = tmp;
		
		for(int i=0;i<count;i++){
			struct stack *move = stacks[from];
			if(!move){
				printf("Tried to move from emptz stack!");
				break;
			}
			stacks[from] = stacks[from]->next;
			move->next = stacks[to];
			stacks[to] = move;
		}
	}
	
	printf("Part 1: ");
	for(int i=0;i<stack_count;i++){
		printf("%c", stacks[i] ? stacks[i]->el : ' ');
	}
	printf("\n");
	printf("Part 2: ");
	for(int i=0;i<stack_count;i++){
		printf("%c", stacks2[i] ? stacks2[i]->el : ' ');
	}
	printf("\n");
}
