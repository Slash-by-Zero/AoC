#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct bag{
	char *name;
	int prev_length, contents_length;
	struct bag **prev, **contents;
	int *counts;
};

struct listel{
	struct bag *data;
	int count;
	struct listel *next;
} *map;

/*Either return a value if it is contained in bags or creates a new bag structure
 * */
static struct bag * get(struct listel **, char *);

/* Basically get(_, bag->name) but no return of the element.
 * */
static void add(struct listel **, struct bag *);

static void add2(struct listel **, struct bag *, int);

static int contains(struct listel *, char *);

static struct bag * pop(struct listel **);

static struct listel *pop2(struct listel **);

static void freeList(struct listel *);

static void die(char *msg){
	perror(msg);
	exit(EXIT_FAILURE);
}

static void printList(){
	struct listel *walk=map;
	
	while(walk){
		printf("(%s) -> ", walk->data->name);
		walk=walk->next;
	}
	
	printf("(nil)\n");
}

int main(int argc, char *argv[]){
	map=NULL;
	
	while(!feof(stdin)){
		int length=1;
		char *line=malloc(sizeof(*line));
		if(!line) die("malloc");
		
		do{
			line=realloc(line, length+=100);
			if(!line) die("realloc");
			
			char *retVal=fgets(&line[strlen(line)], 101, stdin);
			
			if(feof(stdin)){
				free(line);
				goto eof;
			}
			
			if(!retVal) die("fgets");
		}while(line[strlen(line)-1] != '\n');
		
		struct bag *bag;
		
		for(int i=0;i<strlen(line);i++){
			if(strncmp(line+i, " bags contain ", 14) == 0){
				line[i]='\0';
				bag=get(&map, line);
				
				if(strncmp(line+i+14, "no", 2) == 0){
					break;
				}
				
				char *entry=strtok(line+i+13, ",");
				
				while(entry){
					entry++;
					for(int j=0;j<strlen(entry);j++){
						if(strncmp(entry+j, " bag", 4) == 0){
							entry[j]='\0';
							
							struct bag *content=get(&map, entry+2);
							
							content->prev=realloc(content->prev, (content->prev_length+1)*sizeof(*content->prev));
							if(!content->prev) die("realloc");
							
							content->prev[content->prev_length++]=bag;
							
							bag->contents=realloc(bag->contents, (bag->contents_length+1)*sizeof(*bag->contents));
							if(!bag->contents) die("realloc");
							
							bag->contents[bag->contents_length++]=content;
							
							bag->counts=realloc(bag->counts, bag->contents_length*sizeof(*bag->counts));
							if(!bag->counts) die("realloc");
							
							bag->counts[bag->contents_length-1]=strtol(entry, NULL, 10);
							break;
						}
					}
					
					entry = strtok(NULL, ",");
				}
				
				break;
			}
		}
		
		free(line);
	}
	
eof:
	if(!contains(map, "shiny gold")){
		printf("'shiny gold' bag not in list! ");
		return 1;
	}
	
	int countPreds=0;
	struct listel *processing=NULL, *processed=NULL;
	add(&processing, get(&map, "shiny gold"));
	
	while(processing){
		struct bag *current=pop(&processing);
		add(&processed, current);
		
		for(int i=0;i<current->prev_length;i++){
			if(!contains(processed, current->prev[i]->name) && !contains(processing, current->prev[i]->name)){
				add(&processing, current->prev[i]);
				countPreds++;
			}
		}
	}
	freeList(processed);
	
	int countTotal=0;
	add2(&processing, get(&map, "shiny gold"), 1);
	
	while(processing){
		struct listel *tmp=pop2(&processing);
		struct bag *current=tmp->data;
		int current_count=tmp->count;
		free(tmp);
		
		for(int i=0;i<current->contents_length;i++){
			add2(&processing, current->contents[i], current_count*current->counts[i]);
			countTotal+=current_count*current->counts[i];
		}
	}
	
	printf("Part 1: %d\nPart 2: %d\n", countPreds, countTotal);
}

static struct bag * get(struct listel **head, char * name){
	struct listel **walk=head;
	
	while(*walk!=NULL && strcmp(name, (*walk)->data->name)>=0){
		if(strcmp(name, (*walk)->data->name)==0){
			return (*walk)->data;
		}
		
		walk=&((*walk)->next);
	}
	
	struct listel *new=malloc(sizeof(struct listel));
	if(!new) die("malloc");
	
	new->data=calloc(sizeof(struct bag),1);
	if(!new->data) die("calloc");
	
	new->data->name=strdup(name);
	if(!new->data->name) die("strdup");
	
	new->next=*walk;
	
	(*walk)=new;
	
	return new->data;
}

static void add(struct listel **head, struct bag *bag){
	struct listel **walk=head;
	
	while(*walk!=NULL && strcmp(bag->name, (*walk)->data->name)>=0){
		if(strcmp(bag->name, (*walk)->data->name)==0){
			return;
		}
		
		walk=&((*walk)->next);
	}
	
	struct listel *new=malloc(sizeof(struct listel));
	if(!new) die("malloc");
	
	new->data=bag;
	new->next=*walk;
	
	(*walk)=new;
}

static void add2(struct listel **head, struct bag *bag, int count){
	struct listel **walk=head;
	
	while(*walk!=NULL && strcmp(bag->name, (*walk)->data->name)>0){
		walk=&((*walk)->next);
	}
	
	struct listel *new=malloc(sizeof(struct listel));
	if(!new) die("malloc");
	
	new->data=bag;
	new->next=*walk;
	new->count=count;
	
	(*walk)=new;
}

static int contains(struct listel *head, char *name){
	struct listel *walk=head;
	
	while(walk!=NULL && strcmp(name, walk->data->name)>=0){
		if(strcmp(name, walk->data->name)==0){
			return 1;
		}
		
		walk=walk->next;
	}
	
	return 0;
}

static struct bag * pop(struct listel **head){
	struct bag *rVal=(*head)->data;
	struct listel *tmp=(*head);
	(*head)=(*head)->next;
	free(tmp);
	return rVal;
}

static struct listel * pop2(struct listel **head){
	struct listel *rVal=*head;
	(*head)=(*head)->next;
	return rVal;
}

static void freeList(struct listel *head){
	struct listel *walk=head;
	
	while(walk){
		struct listel *tmp=walk;
		walk=walk->next;
		free(tmp);
	}
}
