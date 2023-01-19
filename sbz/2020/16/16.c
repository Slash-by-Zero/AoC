#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 512

static void die(char *msg){
	perror(msg);
	abort();
}

struct range{
	int lower, upper;
	struct range *next;
} *head = NULL;

static int contains(int n){
	struct range *walk=head;
	for(;walk;walk = walk->next){
		if(walk->upper >= n) break;
	}
	
	return walk && walk->lower <= n;
}

static void insert(int lower, int upper){
	struct range *walk = head;
	struct range *prev = NULL;
	for(;walk;walk = walk->next){
		if(walk->lower > lower) break;
		prev = walk;
	}
	
	if(prev && lower-1 <= prev->upper){
		if(upper > prev->upper) prev->upper = upper;
	}
	else{
		struct range *new = malloc(sizeof(*prev));
		if(!new) die("malloc");
		new->lower = lower;
		new->upper = upper;
		new->next = walk;
		if(prev) prev->next = new;
		else head = new;
		prev = new;
	}
	
	if(walk && prev->upper >= walk->lower-1){
		if(walk->upper > prev->upper) prev->upper = walk->upper;
		
		prev->next = walk->next;
		free(walk);
	}
}

int main(){
	int res1=0, res2=0;
	char line[MAX_LINE_LENGTH];
	
	for(;;){
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line) <= 1) break;
		
		char *words[4];
		
		int i=0;
		for(words[i++] = strtok(line, ":"); i<4; words[i++] = strtok(NULL, " "));
		
		for(int i=1;i<4;i+=2){
			int lower = strtol(words[i], &words[i], 10);
			int upper = strtol(words[i]+1, NULL, 10);
			insert(lower, upper);
		}
	}
	if(ferror(stdin)) die("fgets");
	
	if(!fgets(line, MAX_LINE_LENGTH, stdin)) die("fgets");
	if(!fgets(line, MAX_LINE_LENGTH, stdin)) die("fgets");
	if(!fgets(line, MAX_LINE_LENGTH, stdin)) die("fgets");
	if(!fgets(line, MAX_LINE_LENGTH, stdin)) die("fgets");
	
	for(;;){
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line) <= 1) break;
		
		char *endptr = line;
		for(int n=strtol(endptr, &endptr, 10);*endptr != '\0'; n=strtol(endptr+1, &endptr, 10)){
			if(!contains(n)) res1+=n;
		}
	}
	
	printf("Part 1: %d\nPart 2: %d\n", res1, 0);
}
