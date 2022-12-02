#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define newline ('\n'-'A')

int main(int argc, char *argv[]){
	int res1=0, res2=0;
	int opponent, you;
	
	for(;;){
		opponent = fgetc(stdin) - 'A';
		if(opponent == newline) break;
		fgetc(stdin);
		you=fgetc(stdin)-'X';
		fgetc(stdin);
		res1 += you + 1 + 3 * ((you - opponent + 4)%3);
		res2 += you * 3 + 1 +  (you + opponent + 2)%3;
	}
	
	printf("Part 1: %d\nPart 2: %d\n", res1, res2);
}
