#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 512

int compareConst(const void *, const void *);

int compare(const char **, const char **);

int compareHelper(const char *, const char *);

int main(int argc, char *argv[]){
	long res1=0, res2=0;
	char line[2][MAX_LINE_LENGTH];
	int i=1;
	
	char *div1 = "[[2]]\n", *div2 = "[[6]]\n";
	int div1i=1, div2i=2;
	
	
	memset(line, 0, 2*MAX_LINE_LENGTH);
	
	for(;;i++){
		if(!fgets(line[0], MAX_LINE_LENGTH, stdin) || strlen(line[0]) <= 1) break;
		if(!fgets(line[1], MAX_LINE_LENGTH, stdin) || strlen(line[1]) <= 1) break;
		
		char *left=line[0], *right=line[1];
		
		if(compareHelper(left, right) == 1){
			res1+=i;
		}
		
		div1i += (compareHelper(left, div1) == 1) + (compareHelper(right, div1) == 1);
		div2i += (compareHelper(left, div2) == 1) + (compareHelper(right, div2) == 1);
		
		if(!fgets(line[0], MAX_LINE_LENGTH, stdin) || strlen(line[0]) != 1) break;
	}
	
	res2 = div1i*div2i;
	
	printf("Part 1: %ld\nPart 2: %ld\n", res1, res2);
}

int compareConst(const void *leftC, const void *rightC){
	const char *left= (const char *) leftC, *right= (const char *) rightC;
	return -compare(&left, &right);
}

int compareHelper(const char *left, const char *right){
	return compare(&left, &right);
}

int compare(const char **left, const char **right){
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
		if(stack == -1 || (*left)[0] == '\n') break;
		(*left)++;
	}
	stack = 0;
	while(risList){
		stack += ((*right)[0] == '[') - ((*right)[0] == ']');
		if(stack == -1 || (*right)[0] == '\n') break;
		(*right)++;
	}
	
	return cmp;
}
