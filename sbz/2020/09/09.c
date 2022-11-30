#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

void die(char *msg){
	perror(msg);
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
	int preamble_length = argc>1 ? strtol(argv[1], NULL, 10) : 25;
	int last_numbers[preamble_length];
	
	int total_numbers=0, all_numbers_length=preamble_length<<2;
	int *all_numbers=malloc(all_numbers_length*sizeof(*all_numbers));
	if(!all_numbers) die("malloc");
	
	while(1){
		char line[17]="";
		
		char *retVal = fgets(line, 17, stdin);
		
		if(feof(stdin)){
			break;
		}
		else if(!retVal) die("fgets");
		
		if(total_numbers >= all_numbers_length){
			all_numbers_length+=preamble_length<<2;
			all_numbers=realloc(all_numbers, all_numbers_length*sizeof(*all_numbers));
			
			if(!all_numbers) die("realloc");
		}
		
		all_numbers[total_numbers]=strtol(line, NULL, 10);
		if(total_numbers<preamble_length) last_numbers[total_numbers]=all_numbers[total_numbers];
		total_numbers++;
	}
	
	int number;
	
	for(int i=preamble_length, cyclic_counter=0;1;i++){
		number=all_numbers[i];
		
		int pair_found=0;
		for(int i=0;i<preamble_length;i++){
			for(int j=0;j<i;j++){
				if(last_numbers[i] != last_numbers[j] && (last_numbers[i] + last_numbers[j]) == number){
					pair_found=1;
				}
			}
		}
		
		if(pair_found){
			last_numbers[cyclic_counter++]=number;
			if(cyclic_counter>=preamble_length) cyclic_counter=0;
		}else{
			printf("Part 1: %d\n", number);
			break;
		}
	}
	
	for(int start=0;start<all_numbers_length;start++){
		for(int end=start;end<all_numbers_length;end++){
			int max=0, min=INT_MAX, total=0;
			
			for(int i=start;i<=end;i++){
				total+=all_numbers[i];
				if(all_numbers[i]>max) max=all_numbers[i];
				if(all_numbers[i]<min) min=all_numbers[i];
			}
			
			if(total==number){
				printf("Part 2: %d\n", min+max);
				exit(EXIT_SUCCESS);
			}
			else if(total>number && end-start+1 >= preamble_length){
				break;
			}
		}
	}
}
