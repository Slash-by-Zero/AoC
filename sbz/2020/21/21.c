#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static void die(char *msg){
	perror(msg);
	abort();
}

struct ingredient{
	char *name;
	int appearances;
	int index;
	struct ingredient *next;
} *ingredients[256];

static int ingredient_count=0;

static int getIndexOfIngredient(const char *name){
	
	uint8_t hashCode=0;
	for(const char *tmp = name; *tmp != '\0'; tmp++) hashCode = ((hashCode << 1) + (hashCode >> 7)) ^ (*tmp);
	
	struct ingredient **walk = ingredients+hashCode;
	
	for(;*walk;walk = &(*walk)->next){
		if(strcmp(name, (*walk)->name) == 0){
			(*walk)->appearances++;
			return (*walk)->index;
		}
	}
	
	*walk = malloc(sizeof(**ingredients));
	if(!*walk) die("malloc");
	(*walk)->index=ingredient_count;
	(*walk)->appearances = 1;
	(*walk)->name = strdup(name);
	if(!(*walk)->name) die("strdup");
	(*walk)->next = 0;
	
	return ingredient_count++;
}

struct bitmap{
	size_t size;
	uint8_t *map;
};

static void bmAnd(struct bitmap *this, struct bitmap *other){
	if(this->size < other->size){
		this->map = realloc(this->map, other->size);
		if(!this->map) die("realloc");
		memset(this->map + this->size, 0, other->size - this->size);
		this->size = other->size;
	}
	
	size_t i=0;
	for(;i < other->size;i++){
		this->map[i] &= other->map[i];
	}
	memset(this->map+i, 0, this->size-i);
}

static void bmAndNot(struct bitmap *this, struct bitmap *other){
	if(this->size < other->size){
		this->map = realloc(this->map, other->size);
		if(!this->map) die("realloc");
		memset(this->map + this->size, 0, other->size - this->size);
		this->size = other->size;
	}
	
	for(size_t i=0;i < other->size;i++){
		this->map[i] &= ~other->map[i];
	}
}

static void bmOr(struct bitmap *this, struct bitmap *other){
	if(this->size < other->size){
		this->map = realloc(this->map, other->size);
		if(!this->map) die("realloc");
		memset(this->map + this->size, 0, other->size - this->size);
		this->size = other->size;
	}
	
	size_t i=0;
	for(;i<other->size;i++){
		this->map[i] |= other->map[i];
	}
}

static void bmSet(struct bitmap *this, size_t loc, bool val){
	size_t i = loc >> 3;
	if(this->size <= i){
		this->map = realloc(this->map, i + 1);
		if(!this->map) die("realloc");
		
		memset(this->map + this->size, 0, i + 1 - this->size);
		this->size = i + 1;
	}
	
	this->map[i] |= val << (loc & 7);
}

static int bmGet(struct bitmap *this, size_t loc){
	size_t i = loc >> 3;
	return this->size > i && (this->map[i] & (1 << (loc&7))) != 0;
}

static int bmCountOnes(struct bitmap *this){
	int res=-1;
	for(int i=0;i<this->size;i++){
		for(int j = 0;j<8;j++){
			if((this->map[i] & (1 << j)) == 0) continue;
			if(res != -1) return -1;
			res = (i << 3) + j;
		}
	}
	return res;
}

static void bmFree(struct bitmap *this){
	free(this->map);
}

static struct bitmap *bmCopy(struct bitmap *this){
	struct bitmap *new = malloc(sizeof(*new));
	if(!new) die("malloc");
	new->size = this->size;
	new->map = malloc(new->size);
	if(!new->map) die("malloc");
	memcpy(new->map, this->map, new->size);
	return new;
}

static size_t allergen_count = 0;

struct allergen{
	char *name;
	struct bitmap *possIngredients;
	struct allergen *next;
} *head = NULL;

static void ingredientSet(char *name, struct bitmap *bm){
	struct allergen **walk = &head;
	for(;*walk; walk = &(*walk)->next){
		int cmp = strcmp((*walk)->name, name);
		if(cmp == 0){
			bmAnd((*walk)->possIngredients, bm);
			return;
		}
		else if(cmp > 0) break;
	}
	
	allergen_count++;
	struct allergen *new = malloc(sizeof(*new));
	if(!new) die("calloc");
	new->name = strdup(name);
	if(!new->name) die("strdup");
	new->possIngredients = bmCopy(bm);
	new->next = *walk;
	*walk = new;
}

int main(){
	char wordbuf[32];
	int charCount;
	
	for(int c = fgetc(stdin); c != EOF && c != '\n'; c = fgetc(stdin)){
		struct bitmap containedIngredients = {0};
		charCount = 0;
		for(; c != EOF && c != '('; c = fgetc(stdin)){
			if(c == ' '){
				wordbuf[charCount] = '\0';
				int index = getIndexOfIngredient(wordbuf);
				bmSet(&containedIngredients, index, 1);
				charCount = 0;
				continue;
			}
			wordbuf[charCount++] = (char) c;
		}
		
		while(c != ' ' && c != EOF) c = fgetc(stdin);
		
		charCount = 0;
		for(c = fgetc(stdin); c != EOF && c != '\n'; c = fgetc(stdin)){
			if(c == ',') c = fgetc(stdin);
			if(c == ')' || c == ' '){
				wordbuf[charCount] = '\0';
				ingredientSet(wordbuf, &containedIngredients);
				charCount=0;
				continue;
			}
			wordbuf[charCount++] = (char) c;
		}
		
		bmFree(&containedIngredients);
	}
	if(ferror(stdin)) die("fgetc");
	
	struct bitmap bm = {0};
	struct bitmap mask = {0};
	
	for(struct allergen *walk = head; walk; walk=walk->next){
		bmOr(&bm, walk->possIngredients);
	}
	
	char *allergensIngredients[allergen_count];
	int ingredientsAllergen[ingredient_count];
	memset(ingredientsAllergen, 0xff, sizeof(ingredientsAllergen));
	
	for(int i=0;i<allergen_count;i++){
		int j=0;
		for(struct allergen *walk = head; walk; walk=walk->next){
			bmAndNot(walk->possIngredients, &mask);
			int index = bmCountOnes(walk->possIngredients);
			if(index != -1){
				ingredientsAllergen[index] = j;
				bmSet(&mask, index, 1);
				break;
			}
			j++;
		}
	}
	
	long res1 = 0;
	
	for(int i=0;i<256;i++){
		for(struct ingredient *walk = ingredients[i]; walk; walk=walk->next){
			if(!bmGet(&bm, walk->index)) res1 += walk->appearances;
			if(ingredientsAllergen[walk->index] != -1){
				allergensIngredients[ingredientsAllergen[walk->index]] = walk->name;
			}
		}
	}
	
	bmFree(&bm);
	
	printf("Part 1: %ld\nPart 2: %s", res1, allergensIngredients[0]);
	for(int i=1;i<allergen_count;i++){
		printf(",%s", allergensIngredients[i]);
	}
	printf("\n");
	
	exit(0);
}
