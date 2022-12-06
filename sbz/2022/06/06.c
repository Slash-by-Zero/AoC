#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	int res1 = 0, res2 = 0;
	int c, i;
	char contained[256];
	int distinct;
	char last[14];
	
	memset(contained, 0, 'z');
	distinct = 0;
	
	for(c = fgetc(stdin);;c = fgetc(stdin)){
		res1++;
		memcpy(last+1, last, 3);
		last[0] = c;
		if(contained[c]){
			for(i = 3; last[i] != c; i--){
				contained[last[i]] = 0;
			}
			distinct -= i;
			memset(last+i
		}
		else{
			contained[c] = 1;
			if(++distinct == 4) break;
		}
	}
	
	
	res2=res1;
	
	for(c = fgetc(stdin);;c = fgetc(stdin)){
		res2++;
		memcpy(last+1, last, 13);
		last[0] = c;
		if(contained[c]){
			for(i = 13; last[i] != c; i--){
				contained[last[i]] = 0;
			}
			distinct -= i;
		}
		else if(++distinct == 14) break;
		else contained[c] = 1;
	}
	
	printf("Part 1: %d\nPart 2: %d\n", res1, res2);
}
