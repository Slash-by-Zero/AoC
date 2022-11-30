#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void die(char *msg){
	perror(msg);
	exit(EXIT_FAILURE);
}



int main(int argc, char *argv[]){
	int rows=0, columns=1;
	int layout_length=16;
	int **layout=calloc(layout_length, sizeof(*layout));
	if(!layout) die("calloc");
	
	{
		char *line=malloc(1);
		if(!line) die("malloc");
		
		do{
			line=realloc(line, columns+=127);
			if(!line) die("realloc");
			
			char *retVal=fgets(line+columns-128, 128, stdin);
			
			if(feof(stdin)){
				printf("Input is expected to be more than one line otherwise jsut count the L's!\n");
				exit(EXIT_FAILURE);
			}
			if(!retVal || ferror(stdin)) die("fgets");
		}while(line[strlen(line)-1]!='\n');
		
		line[strlen(line)-1]='\0';
		
		layout[0]=malloc(strlen(line)*sizeof(**layout));
		if(!layout[0]) die("malloc");
		
		for(int i=0;i<strlen(line);i++){
			layout[0][i]=(line[i]-76)/30;
		}
		
		columns=strlen(line);
		
		free(line);
	}
	
	for(rows=1;1;rows++){
		char line[columns+2];
		
		char *retVal=fgets(line, columns+2, stdin);
		if(feof(stdin)) break;
		if(!retVal || ferror(stdin)) die("fgets");
		
		line[strlen(line)-1]='\0';
		
		if(rows>=layout_length){
			layout_length+=16;
			layout=realloc(layout, layout_length*sizeof(*layout));
			if(!layout) die("realloc");
		}
		
		layout[rows]=malloc(columns*sizeof(**layout));
		if(!layout[rows]) die("malloc");
		
		for(int i=0;i<strlen(line);i++){
			layout[rows][i]=(line[i]-76)/30;
		}
	}
	
	rows--;
	
	int occupied=0;
	
	while(1){
		int **next=malloc(sizeof(*next)*rows), neighbours[rows][columns], last_row[columns];
		if(!next) die("malloc");
		
		for(int i=0;i<rows;i++){
			next[i]=malloc(sizeof(**next)*columns);
			if(!next[i]) die("malloc");
			
			for(int j=0;j<columns;j++){
				int tmp=layout[i][j]>0?1:0;
				
				if(j>0) tmp+=layout[i][j-1]>0?1:0;
				if(j<columns-1) tmp+=layout[i][j+1]>0?1:0;
				
				neighbours[i][j]=tmp;
				if(i>0){
					neighbours[i-1][j]+=tmp;
					neighbours[i][j]+=last_row[j];
				}
				last_row[j]=tmp;
			}
		}
		
		int change=0;
		
		for(int i=0;i<rows;i++){
			for(int j=0;j<columns;j++){
				if(layout[i][j]==0 && neighbours[i][j]==0){
					change=1;
					occupied++;
					next[i][j]=1;
				}
				else if(layout[i][j]==1 && neighbours[i][j]>=5){
					change=1;
					occupied--;
					next[i][j]=0;
				}
				else next[i][j]=layout[i][j];
			}
		}
		
		int **tmp=layout;
		layout=next;
		for(int i=0;i<rows;i++){
			free(tmp[i]);
		}
		free(tmp);
		
		if(!change) break;
	}
	
	printf("Part 1: %d\n", occupied);
}
