#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	int res1=0, res2=0;
	char line[128];
	
	for(int count=0;;count++){
		if(!fgets(line, 128, stdin)) break;
		
		if(strlen(line) == 1) break;
		
		char *strings[4];
		strings[0] = strtok(line, "-,");
		for(int i=1;i<4;i++){
			strings[i] = strtok(NULL, "-,");
		}
		
		int numbers[4];
		for(int i=0;i<4;i++){
			numbers[i] = strtol(strings[i], NULL, 10);
		}
		
		if(numbers[0] >= numbers[2] && numbers[1] <= numbers[3]) res1++;
		else if(numbers[2] >= numbers[0] && numbers[3] <= numbers[1]) res1++;
		else if(numbers[0] < numbers[2] && numbers[1] >= numbers[2]) res2++;
		else if(numbers[1] > numbers[3] && numbers[0] <= numbers[3]) res2++;
	}
	
	res2+=res1;
	
	printf("Part 1: %d\nPart 2: %d\n", res1, res2);
}
