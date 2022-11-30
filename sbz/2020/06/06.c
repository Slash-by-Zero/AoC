#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[]){
	int total1=0, total2=0;
	
	while(1){
		char line[28];
		char anyone[26];
		char everyone[26];
		
		memset(everyone, 1, 26);
		memset(anyone, 0, 26);
		
		if(!fgets(line, 28, stdin) && !feof(stdin)){
			perror("fgets");
			exit(EXIT_FAILURE);
		}
		
		if(feof(stdin)){
			break;
		}
		
		while(strlen(line)!=1){
			char questions[26];
			
			memset(questions, 0, 26);
			
			line[strlen(line)-1]='\0';
			
			for(int i=0; i<strlen(line);i++){
				questions[line[i]-'a']=1;
			}
			
			for(int i=0;i<26;i++){
				anyone[i]=anyone[i]|questions[i];
				everyone[i]=everyone[i]&questions[i];
			}
			
			if(!fgets(line, 28, stdin) && !feof(stdin)){
				perror("fgets");
				exit(EXIT_FAILURE);
			}
			
			if(feof(stdin)){
				goto eof;
			}
		}
		
		for(int i=0;i<26;i++){
			total1+=anyone[i];
			total2+=everyone[i];
		}
	}
	
eof:
	
	printf("Part 1: %d\nPart 2: %d\n", (int) total1, (int) total2);
}
