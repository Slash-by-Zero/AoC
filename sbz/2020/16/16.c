#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 512

static void die(char *msg){
	perror(msg);
	abort();
}

int main(){
	long res1=0, res2=1;
	char line[MAX_LINE_LENGTH];
	
	int fields_size=16, fields_count=0;
	struct{
		char name[9];
		int arr[4];
	} *fields = calloc(sizeof(*fields), fields_size);
	if(!fields) die("calloc");
	
	for(;;){
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line) <= 1) break;
		
		char *words[4];
		
		int i=0;
		for(words[i++] = strtok(line, ":"); i<4; words[i++] = strtok(NULL, " "));
		
		if(fields_count >= fields_size){
			fields = realloc(fields, (fields_size+16)*sizeof(*fields));
			if(!fields) die("realloc");
			memset(fields+fields_size, 0, 16*sizeof(*fields));
			fields_size+=16;
		}
		
		strncpy(fields[fields_count].name, words[0], 9);
		fields[fields_count].arr[0] = strtol(words[1], &words[1], 10);
		fields[fields_count].arr[1] = strtol(words[1]+1, NULL, 10);
		fields[fields_count].arr[2] = strtol(words[3], &words[3], 10);
		fields[fields_count].arr[3] = strtol(words[3]+1, NULL, 10);
		fields_count++;
	}
	if(ferror(stdin)) die("fgets");
	
	int own_ticket[fields_count];
	
	if(!fgets(line, MAX_LINE_LENGTH, stdin)) die("fgets");
	if(!fgets(line, MAX_LINE_LENGTH, stdin)) die("fgets");
	
	{
		char *endptr=line;
		int i=0;
		for(int n = strtol(endptr, &endptr, 10); *endptr != '\0'; n = strtol(endptr+1, &endptr, 10)){
			own_ticket[i++] = n;
		}
	}
	
	if(!fgets(line, MAX_LINE_LENGTH, stdin)) die("fgets");
	if(!fgets(line, MAX_LINE_LENGTH, stdin)) die("fgets");
	
	int poss_fields[fields_count][fields_count];
	memset(poss_fields, -1, sizeof(**poss_fields)*fields_count*fields_count);
	
	for(;;){
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line) <= 1) break;
		
		int ticket[fields_count];
		char *endptr = line;
		int i=0;
		for(int n=strtol(endptr, &endptr, 10);*endptr != '\0'; n=strtol(endptr+1, &endptr, 10)){
			int valid = 0;
			for(int j=0;j<fields_count;j++){
				valid |= (n >= fields[j].arr[0] && n <= fields[j].arr[1]) || (n >= fields[j].arr[2] && n <= fields[j].arr[3]);
			}
			if(!valid) {
				res1+=n;
				break;
			}
			ticket[i++] = n;
		}
		if(i != fields_count) continue;
		
		for(i=0;i<fields_count;i++){
			for(int j=0;j<fields_count;j++){
				poss_fields[i][j] &= ((ticket[i] >= fields[j].arr[0] && ticket[i] <= fields[j].arr[1]) || (ticket[i] >= fields[j].arr[2] && ticket[i] <= fields[j].arr[3]));
			}
		}
	}
	
	for(int k=0;k<fields_count;k++){
		for(int i=0;i<fields_count;i++){
			int sum = 0;
			int last_occ;
			for(int j=0;j<fields_count;j++){
				sum += poss_fields[i][j];
				if(poss_fields[i][j]) last_occ = j;
			}
			if(sum == 1){
				for(int j=0;j<fields_count;j++){
					poss_fields[j][last_occ] = (j == i);
				}
			}
		}
	}
	
	for(int i=0;i<fields_count;i++){
		int last_occ;
		for(int j=0;j<fields_count;j++){
			if(poss_fields[i][j]) last_occ = j;
		}
		if(strncmp(fields[last_occ].name, "departure", 9) == 0){
			res2*=own_ticket[i];
		}
	}
	
	
	printf("Part 1: %ld\nPart 2: %ld\n", res1, res2);
}
