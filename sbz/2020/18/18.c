#include <stdio.h>
#include <stdlib.h>

static void die(char *msg){
	perror(msg);
	abort();
}

static long eval2(){
	long totals[16] = {0};
	int counter = 0;
	int op = '+';
	for(int c=fgetc(stdin);c != EOF && c != '\n';c=fgetc(stdin)){
		long curr;
		if(c == '('){
			curr = eval2();
			c = fgetc(stdin);
		}
		else{
			curr = c - '0';
			for(c = fgetc(stdin);c >= '0' && c <= '9'; c = fgetc(stdin)) curr = curr * 10 + c - '0';
		}
		
		switch(op){
			case('+'):
				totals[counter] += curr;
				break;
			case('*'):
				totals[++counter] = curr;
				break;
		}
		
		if(c == EOF) die("fgetc");
		if(c == ')' || c == '\n'){
			long res = 1;
			for(;counter >= 0;counter--){
				res *= totals[counter];
			}
			return res;
		}
		
		op = fgetc(stdin);
		if(op == EOF) die("fgetc");
		if((c = fgetc(stdin)) == EOF) die("fgetc");
	}
	if(ferror(stdin)) die("fgetc");
	return 0;
}

static long eval(){
	long total = 0;
	int op = '+';
	for(int c=fgetc(stdin);c != EOF && c != '\n';c=fgetc(stdin)){
		long curr;
		if(c == '('){
			curr = eval();
			c = fgetc(stdin);
		}
		else{
			curr = c - '0';
			for(c = fgetc(stdin);c >= '0' && c <= '9'; c = fgetc(stdin)) curr = curr * 10 + c - '0';
		}
		
		switch(op){
			case('+'):
				total += curr;
				break;
			case('*'):
				total *= curr;
				break;
		}
		
		if(c == EOF) die("fgetc");
		if(c == ')' || c == '\n') return total;
		
		op = fgetc(stdin);
		if(op == EOF) die("fgetc");
		if((c = fgetc(stdin)) == EOF) die("fgetc");
	}
	if(ferror(stdin)) die("fgetc");
	return 0;
}

int main(){
	long res1 = 0, res2 = 0;
	for(;!feof(stdin);){
		res1 += eval();
	}
	
	rewind(stdin);
	
	for(;!feof(stdin);){
		res2 += eval2();
	}
	
	printf("Part 1: %ld\nPart 2: %ld\n", res1, res2);
}
