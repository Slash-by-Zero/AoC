#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	int res1 = 0, res2 = 0;
	char c;
	char marker[14];
	memset(marker, 0, 4);
	
	
	for(c = fgetc(stdin);c!=EOF && c!='\n';c = fgetc(stdin)){
		memcpy(marker+1, marker, 3);
		marker[0] = c;
		int equiv=0;
		for(int i=0;i<3;i++){
			for(int j=i+1;j<4;j++){
				if(marker[i] == marker[j] || marker[j] == 0){
					equiv = 1;
					break;
				}
			}
		}
		
		res1++; 
		
		if(!equiv) break;
		
	}
	
	res2=res1;
	
	for(c = fgetc(stdin);c!=EOF && c!='\n';c = fgetc(stdin)){
		memcpy(marker+1, marker, 13);
		marker[0] = c;
		int equiv=0;
		for(int i=0;i<13;i++){
			for(int j=i+1;j<14;j++){
				if(marker[i] == marker[j] || marker[j] == 0){
					equiv = 1;
					break;
				}
			}
		}
		
		res2++; 
		
		if(!equiv) break;
	}
	
	printf("Part 1: %d\nPart 2: %d\n", res1, res2);
}
