#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 512

struct rule{
	int next[2][3];
	char token;
	struct strarr *possWords;
} rules[256] = {0};

struct strarr{
	int length;
	char **s;
};

static void die(char *msg){
	perror(msg);
	abort();
}

static struct strarr *copy(struct strarr *this){
	struct strarr *res = malloc(sizeof(*res));
	if(!res) die("malloc");
	
	res->length = this->length;
	
	res->s = malloc(sizeof(*res->s) * res->length);
	if(!res->s) die("malloc");
	
	for(int i=0;i<res->length;i++){
		res->s[i] = strdup(this->s[i]);
		if(!res->s[i]) die("strdup");
	}
	
	return res;
}

static void concat(struct strarr *this, struct strarr *tmp){
	this->s = realloc(this->s, sizeof(*this->s)*this->length*tmp->length);
	if(!this->s) die("realloc");
	memset(this->s+this->length, 0, sizeof(*this->s)*this->length*(tmp->length-1));
	
	for(int i = tmp->length*this->length-1;i>=0;i--){
		int j = i/tmp->length;
		int k = i%tmp->length;
		this->s[i] = realloc(this->s[i], strlen(this->s[j])+strlen(tmp->s[k])+1);
		if(!this->s[i]) die("realloc");
		strcpy(this->s[i], this->s[j]);
		strcat(this->s[i], tmp->s[k]);
	}
	
	this->length *= tmp->length;
}

static struct strarr *calcPossWords(int rule){
	if(rules[rule].possWords) return rules[rule].possWords;
	
	struct strarr *res = malloc(sizeof(*res));
	if(!res) die("malloc");
	
	if(rules[rule].token != 0) {
		res->length = 1;
		res->s = malloc(sizeof(*res->s));
		if(!res->s) die("malloc");
		res->s[0] = calloc(sizeof(**res->s), 2);
		if(!res->s[0]) die("calloc");
		res->s[0][0] = rules[rule].token;
		
		rules[rule].possWords = res;
		return res;
	}
	
	res->length = 0;
	res->s = NULL;
	
	for(int i=0;i<2;i++){
		struct strarr *cat = NULL;
		
		for(int j=0;j<3;j++){
			if(rules[rule].next[i][j] != 0){
				struct strarr *tmp = calcPossWords(rules[rule].next[i][j]);
				if(!cat) cat = copy(tmp);
				else concat(cat, tmp);
			}
			else break;
		}
		
		if(!cat) continue;
		
		res->s = realloc(res->s, sizeof(*res->s)*(res->length+cat->length));
		for(int i=0;i<cat->length;i++){
			res->s[i+res->length] = cat->s[i];
		}
		res->length+=cat->length;
		free(cat->s);
		free(cat);
	}
	
	rules[rule].possWords = res;
	return res;
}

static int strPtrCmp(const char **a, const char **b){
	return strcmp(*a, *b);
}

static int contains(char *str, int rule){
	struct strarr *poss = rules[rule].possWords;
	int lower = 0, upper = poss->length-1;
	for(;;){
		int avg = (lower+upper)>>1;
		int tmp = strncmp(poss->s[avg], str, strlen(poss->s[avg]));
		if(tmp == 0) return (1 + (strlen(str) == strlen(poss->s[avg])));
		else if(lower >= upper) return 0;
		else if(tmp > 0) upper = avg-1;
		else lower = avg+1;
	}
	return 0;
}

static int check31Plus(char *str, int max){
	int totalLength = strlen(str);
	int matchLength = strlen(rules[31].possWords->s[0]);
	if(matchLength > totalLength) return 0;
	int rVal = contains(str, 31);
	if(rVal == 0) return 0;
	else if(rVal == 2) return 1;
	
	for(;max > 1 && rVal == 1 && strlen(str) > matchLength;rVal = contains(str+=matchLength, 31)) max--;
	
	if(rVal==2) return 1;
	return 0;
}

static int checkPart2(char *str){
	int totalLength = strlen(str);
	int matchLength = strlen(rules[42].possWords->s[0]);
	if(matchLength >= totalLength) return 0;
	if(contains(str, 42) != 1) return 0;
	int max=1;
	for(;strlen(str) > matchLength && contains(str+=matchLength, 42) == 1;){
		if(check31Plus(str+matchLength, max++)) return 1;
	}
	return 0;
}


int main(){
	char line[MAX_LINE_LENGTH];
	
	for(;;){
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line) <= 1) break;
		
		char *endptr = NULL;
		int i = strtol(line, &endptr, 10);
		if(endptr[2] == '"'){
			rules[i].token = endptr[3];
			continue;
		}
		
		rules[i].next[0][0] = strtol(endptr+2, &endptr, 10);
		for(int j=1;endptr[0] != '\n' && endptr[1] != '|';j++){
			rules[i].next[0][j] = strtol(endptr+1, &endptr, 10);
		}
		if(endptr[0] == '\n') continue;
		
		rules[i].next[1][0] = strtol(endptr+3, &endptr, 10);
		for(int j=1;endptr[0] != '\n';j++){
			rules[i].next[1][j] = strtol(endptr+1, &endptr, 10);
		}
	}
	
	calcPossWords(0);
	
	qsort(rules[0].possWords->s, rules[0].possWords->length, sizeof(char *), (int (*)(const void *, const void *)) strPtrCmp); 
	qsort(rules[31].possWords->s, rules[31].possWords->length, sizeof(char *), (int (*)(const void *, const void *)) strPtrCmp); 
	qsort(rules[42].possWords->s, rules[42].possWords->length, sizeof(char *), (int (*)(const void *, const void *)) strPtrCmp); 
	
	int res1=0, res2=0;
	
	for(;;){
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line) <= 1) break;
		
		line[strlen(line) - 1] = '\0';
		if(contains(line,0) == 2){
			res1++;
			res2++;
		}
		else if(checkPart2(line)) res2++;
	}
	
	printf("Part 1: %d\nPart 2: %d\n", res1, res2);
}
