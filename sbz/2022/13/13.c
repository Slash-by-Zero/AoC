#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 512

int compareConst(const void *, const void *);

int compare(char **, char **);

int main(int argc, char *argv[]){
	long res1=0, res2=0;
	char line[MAX_LINE_LENGTH][MAX_LINE_LENGTH];
	int i=1;
	int curr_line=0;
	
	memset(line, 0, MAX_LINE_LENGTH*MAX_LINE_LENGTH);
	
	for(;;i++){
		if(!fgets(line[curr_line++], MAX_LINE_LENGTH, stdin) || strlen(line[curr_line-1]) <= 1) break;
		if(!fgets(line[curr_line++], MAX_LINE_LENGTH, stdin) || strlen(line[curr_line-1]) <= 1) break;
		char *left=line[curr_line-2], *right=line[curr_line-1];
		if(compare(&left, &right) == 1){
			res1+=i;
		}
		if(!fgets(line[MAX_LINE_LENGTH-1], MAX_LINE_LENGTH, stdin) || strlen(line[MAX_LINE_LENGTH-1]) != 1) break;
	}
	
	strcpy(line[curr_line-1], "[[2]]\n");
	strcpy(line[curr_line++], "[[6]]\n");
	
	qsort(line, curr_line, MAX_LINE_LENGTH, compareConst);
	
	for(int j=0;j<curr_line;j++){
		if(strcmp(line[j], "[[2]]\n") == 0){
			res2 = j+1;
		}
		if(strcmp(line[j], "[[6]]\n") == 0){
			res2 *= j+1;
		}
	}
	
	printf("Part 1: %ld\nPart 2: %ld\n", res1, res2);
}

int compareConst(const void *leftC, const void *rightC){
	char *left= (const char *) leftC, *right= (const char*) rightC;
	return -compare(&left, &right);
}

int compare(char **left, char **right){
	int lisList = (*left)[0] == '[', risList = (*right)[0] == '[';
	
	if(!risList && !lisList){
		long l=strtol(*left, NULL, 10), r=strtol(*right, NULL, 10);
		while((*left)[0] != ',' && (*left)[0] != ']') (*left)++;
		while((*right)[0] != ',' && (*right)[0] != ']') (*right)++;
		
		return (l<r)-(r<l);
	}
	
	if(lisList) (*left)++;
	if(risList) (*right)++;
	
	int cmp;
	
	if((*left)[0] == ']' || (*right)[0] == ']'){
		cmp = ((*left)[0] == ']') - ((*right)[0] == ']');
		if((*left)[0] == ']'){
			(*left)++;
			lisList = 0;
		}
		if((*right)[0] == ']'){
			(*right)++;
			risList = 0;
		}
	}
	else{
		
		cmp = compare(left, right);
		
		while(cmp == 0){
			if((*left)[0] == ']'){
				(*left)++;
				lisList = 0;
			}
			if((*right)[0] == ']'){
				(*right)++;
				risList = 0;
			}
			if(risList == 0 || lisList == 0){
				cmp = risList - lisList;
				break;
			}
			
			(*left)++;
			(*right)++;
			
			cmp = compare(left, right);
		}
	}
	
	int stack=0;
	while(lisList){
		stack += ((*left)[0] == '[') - ((*left)[0] == ']');
		if(stack == -1) break;
		(*left)++;
	}
	stack = 0;
	while(risList){
		stack += ((*right)[0] == '[') - ((*right)[0] == ']');
		if(stack == -1) break;
		(*right)++;
	}
	
	return cmp;
}
