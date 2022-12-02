#include <string.h>
#include <stdlib.h>
#include <libgen.h>
#include <stdio.h>
#include <errno.h>

#include "cmdline.h"

struct option{
	char *key, *value;
	struct option *next;
} *options;

static char *programName = NULL;
static char **rest = NULL;

static int restc=0;

static int addToList(char *key, char* value){
	for(struct option **walk=&options; ; walk= &(*walk)->next){
		if(!*walk){
			*walk = malloc(sizeof(**walk));
			if(!*walk) return -1;
			
			(*walk)->key=key;
			(*walk)->value=value;
			(*walk)->next=NULL;
			return 0;
		}
		
		int cmp = strcmp(key, (*walk)->key);
		
		if(cmp == 0){
			(*walk)->value = value;
			return 0;
		}
		if(cmp < 0){
			struct option *new = malloc(sizeof(*new));
			if(!new) return -1;
			
			new->key = key;
			new->value = value;
			new->next= *walk;
			*walk = new;
			return 0;
		}
	}
}

int cmdlineInit(int argc, char *argv[]){
	
	char konfigpath[strlen(argv[0])+6];
	strcpy(konfigpath, argv[0]);
	strcpy(konfigpath+strlen(argv[0]), ".conf");
	
	FILE *konfig = fopen(konfigpath, "r");
	
	programName = basename(argv[0]);
	
	char *line=NULL;
	
	while(konfig && !feof(konfig)){
		int line_length=1;
		line = calloc(1,1);
		if(!line) goto cleanup_init;
		do{
			line = realloc(line, line_length+=127);
			if(!line) goto cleanup_init;
			
			if(!fgets(line+strlen(line), 128, konfig)) {
				if(ferror(konfig)) goto cleanup_init;
				free(line);
				goto eof_init;
			}
		}while(line[strlen(line)-1] != '\n');
		line[strlen(line)-1] = '\0';
		
		char *key=strdup(strtok(line, "="));
		if(!key) goto cleanup_init;
		char *value=strdup(strtok(NULL, "="));
		if(!key) goto cleanup_init;
		if(addToList(key, value) < 0) return -1;
		free(line);
	}
eof_init:
	
	if(konfig && fclose(konfig) != 0) return -1;
	
	int i=1;
	for(;i<argc;i++){
		if(strncmp(argv[i], "--", 2) != 0) break;
		
		char *key = argv[i]+2;
		strtok(argv[i], "=");
		char *value = strtok(NULL, "=");
		
		if(addToList(key, value) < 0) return -1;
	}
	
	if(argc > i){
		restc = argc - i;
		rest = argv + i;
	}
	
	return 0;
cleanup_init:
	free(line);
	int tmp = errno;
	if(konfig && fclose(konfig) != 0) return -1;
	errno = tmp;
	return -1;
}

char *const getProgramName(){return programName;}

char *const getValueByKey(const char *key){
	for(struct option *walk=options; walk != NULL; walk=walk->next){
		int cmp = strcmp(key, walk->key);
		if(cmp == 0){
			return walk->value;
		}
		else if(cmp < 0) break;
	}
	return NULL;
}

char **getRemainingArguments(int *count){
	*count = restc;
	return rest;
}
