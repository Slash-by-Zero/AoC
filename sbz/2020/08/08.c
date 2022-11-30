#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void die(char *msg){
	perror(msg);
	exit(EXIT_FAILURE);
}

struct instruction{
	char ins;
	int val;
	int visited;
};

int main(int argc, char *argv){
	int programm_length=100;
	struct instruction *programm=malloc(100*sizeof(*programm));
	if(!programm) die("malloc");
	
	int max_progcount=0;
	
	for(int i=0; !feof(stdin); i++){
		char line[11];
		
		char *rVal=fgets(line, 11, stdin);
		if(feof(stdin)) break;
		if(!rVal || line[strlen(line)-1]!='\n') die("fgets");
		
		if(i>=programm_length){
			programm=realloc(programm, (programm_length+=100)*sizeof(*programm));
			if(!programm) die("realloc");
		}
		
		programm[i].ins=line[0];
		programm[i].val=strtol(line+4, NULL, 10);
		programm[i].visited=0;
		
		max_progcount=i;
	}
	
	int acc=0, progcount=0;
	
	while(!programm[progcount].visited){
		programm[progcount].visited=1;
		switch(programm[progcount].ins){
			case('j'):
				progcount+=programm[progcount].val;
				break;
			case('a'):
				acc+=programm[progcount].val;
			default:
				progcount++;
		}
	}
	
	printf("Part 1: %d\n", acc);
	
	int passed_jumps=0;
	for(int i=0;i<=max_progcount;i++){
		if(programm[i].visited && programm[i].ins=='j') passed_jumps++;
	}
	
	int to_check[passed_jumps];
	
	for(int i=0, j=0;i<=max_progcount;i++){
		if(programm[i].visited && programm[i].ins=='j') {
			to_check[j++]=i;
		}
	}
	
	for(int i=0;i<passed_jumps;i++){
		acc=0;
		progcount=0;
		
		for(int j=0;j<programm_length;j++){
			programm[j].visited=0;
		}
		
		programm[to_check[i]].ins='n';
		
		
		while(!programm[progcount].visited){
			programm[progcount].visited=1;
			switch(programm[progcount].ins){
				case('j'):
					progcount+=programm[progcount].val;
					break;
				case('a'):
					acc+=programm[progcount].val;
				default:
					progcount++;
			}
			if(progcount==max_progcount+1){
				printf("Changed Instruction: %d\n", to_check[i]);
				goto terminated;
			}
			else if(progcount>max_progcount+1){
				break;
			}
			
		}
		
		
		programm[to_check[i]].ins='j';
	}
terminated:
	
	printf("Part 2: %d\n", acc);
}
