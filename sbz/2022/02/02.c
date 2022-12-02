#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define newline ('\n'-('A' - 2))

int main(int argc, char *argv[]){
	int res1=0, res2=0;
	int opponent, you, i;
	
	for(i=0;;i++){
		opponent = fgetc(stdin) - ('A'- 2);
		if(opponent == newline) break;
		fgetc(stdin);
		you=fgetc(stdin)-'R';
		fgetc(stdin);
		res1 += you + 3 * ((you - opponent)%3);
		res2 += you * 3 +  (you + opponent)%3;
	}
	res1 -= 5*i;
	res2 -= 17*i;
	
	printf("Part 1: %d\nPart 2: %d\n", res1, res2);
}
