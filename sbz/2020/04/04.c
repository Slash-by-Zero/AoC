#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct passport{
	char *byr, *iyr, *eyr, *hcl, *pid, *ecl, *hgt;
};

struct passport * readPassport();

int checkValidity(struct passport *);

int main(int argc, char *argv[]){
	
	int valid1=0, valid2=0;
	
	while(!feof(stdin)){
		struct passport *passport=readPassport();
		
		if(passport){
			valid1++;
			
			if(checkValidity(passport)){
				valid2++;
			}
			
			free(passport->byr);
			free(passport->iyr);
			free(passport->eyr);
			free(passport->hcl);
			free(passport->pid);
			free(passport->ecl);
			free(passport->hgt);
			free(passport);
		}
	}

	printf("Part 1: %d\nPart 2: %d\n", valid1, valid2);
}

struct passport * readPassport(){
	char passportString[1024];
	
	{
		int strlength=1;
		
		char *retVal = fgets(passportString, 1024, stdin);
		
		if(!retVal && !feof(stdin)){
			perror("fgets");
			exit(EXIT_FAILURE);
		}
		
		while(!feof(stdin) && strlength+1 != strlen(passportString)){
			strlength=strlen(passportString);
			
			passportString[strlength-1]=' ';
			
			char *retVal = fgets(&passportString[strlength], 1024-strlength, stdin);
			
			if(!retVal && !feof(stdin)){
				perror("fgets");
				exit(EXIT_FAILURE);
			}
		}
	}
	
	struct passport *passport=calloc(1, sizeof(struct passport));
	char *field;
	
	field = strtok(passportString, " ");
	
	while(field){
		field[3]='\0';
		if(strcmp(field, "byr")==0){
			passport->byr=strdup(&field[4]);
		}else if(strcmp(field, "iyr")==0){
			passport->iyr=strdup(&field[4]);
		}else if(strcmp(field, "eyr")==0){
			passport->eyr=strdup(&field[4]);
		}else if(strcmp(field, "hgt")==0){
			passport->hgt=strdup(&field[4]);
		}else if(strcmp(field, "hcl")==0){
			passport->hcl=strdup(&field[4]);
		}else if(strcmp(field, "ecl")==0){
			passport->ecl=strdup(&field[4]);
		}else if(strcmp(field, "pid")==0){
			passport->pid=strdup(&field[4]);
		}
		
		field = strtok(NULL, " ");
	}
	
	if(!passport->byr || !passport->iyr || !passport->eyr || !passport->hgt || !passport->hcl || !passport->ecl || !passport->pid){
		free(passport->byr);
		free(passport->iyr);
		free(passport->eyr);
		free(passport->hcl);
		free(passport->pid);
		free(passport->ecl);
		free(passport->hgt);
		free(passport);
		return NULL;
	}

	return passport;
}

int checkValidity(struct passport *passport){
	char *endptr;
	
	long tmp=strtol(passport->byr, &endptr, 10);
	if(tmp < 1920 || tmp > 2002 || endptr[0]!='\0' || strlen(passport->byr)!=4){
		return 0;
	}

	tmp=strtol(passport->iyr, &endptr, 10);
	if(tmp < 2010 || tmp > 2020 || endptr[0]!='\0' || strlen(passport->iyr)!=4){
		return 0;
	}
	
	tmp=strtol(passport->eyr, &endptr, 10);
	if(tmp < 2020 || tmp > 2030 || endptr[0]!='\0' || strlen(passport->eyr)!=4){
		return 0;
	}
	
	tmp=strtol(passport->hgt, &endptr, 10);
	if((strcmp(endptr, "in")!=0 && strcmp(endptr, "cm")!=0) ||tmp < 59 || tmp > 193 || (strcmp(endptr, "cm")==0 && tmp < 150) || (strcmp(endptr, "in")==0 && tmp > 76)){
		return 0;
	}
	
	tmp=strtol(&(passport->hcl[1]), &endptr, 16);
	if(passport->hcl[0]!='#' || strlen(passport->hcl)!=7 || endptr[0]!='\0'){
		return 0;
	}
	
	if(strcmp(passport->ecl, "amb")!=0 && strcmp(passport->ecl, "blu")!=0 && strcmp(passport->ecl, "brn")!=0 && strcmp(passport->ecl, "gry")!=0 && strcmp(passport->ecl, "grn")!=0 && strcmp(passport->ecl, "hzl")!=0 && strcmp(passport->ecl, "oth")!=0){
		return 0;
	} 
	
	tmp=strtol(passport->pid, &endptr, 10);
	if(endptr[0]!='\0' || strlen(passport->pid)!=9){
		return 0;
	}

	return 1;
}
