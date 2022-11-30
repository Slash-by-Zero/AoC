#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv){
	int max=0;
	int yours=0;
	int all[1024];
	memset(all, 0, 1024*sizeof(int));

	for(int lines=0; !feof(stdin); lines++){
		char line[12];
		char *retVal=fgets(line, 12, stdin);
		
		if(feof(stdin)){
			break;
		}

		if(!retVal){
			perror("fgets");
			exit(EXIT_FAILURE);
		}
		
		int tmp=0;

		for(int i=0; i<10; i++){
			if(line[i]=='B' || line[i]=='R'){tmp++;}
			tmp=tmp<<1;
		}
		tmp=tmp>>1;
		
		all[tmp]=1;
		
		max=tmp>max?tmp:max;
	}
	
	for(int i=1;i<1024;i++){
		if(all[i]==0 && all[i-1]==1){
			yours=i;
			break;
		}
	}
	
	printf("Part 1: %d\nPart 2: %d\n", max, yours);
}
