#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CARDS 64

struct result{
	int winner;
	size_t cardCount;
	uint8_t deck[MAX_CARDS];
};

struct gameState{
	size_t cardCounts[2];
	uint8_t decks[2][MAX_CARDS];
	struct gameState *next;
};


struct tree{
	uint8_t decks[2][MAX_CARDS];
	int winner;
	struct tree *left, *right;
} *trees[MAX_CARDS];

static struct tree *get(struct gameState *curr){
	struct tree *tree = trees[curr->cardCounts[0]+curr->cardCounts[1]];
	for(;;){
		if(!tree) return NULL;
		int cmp = memcmp(curr->decks, tree->decks, sizeof(curr->decks));
		if(cmp == 0) return tree;
		else if(cmp < 0) tree = tree->left;
		else tree = tree->right;
	}
}

static void insert(struct gameState *curr, int winner){
	struct tree **tree = &trees[curr->cardCounts[0]+curr->cardCounts[1]];
	for(;;){
		if(!*tree) break;
		int cmp = memcmp(curr->decks, (*tree)->decks, sizeof(curr->decks));
		if(cmp == 0) return;
		else if(cmp < 0) tree = &(*tree)->left;
		else tree = &(*tree)->right;
	}
	
	*tree = calloc(sizeof(**tree),1);
	(*tree)->winner = winner;
	memcpy((*tree)->decks, curr->decks, sizeof(curr->decks));
}

struct result recursiveCombat(struct gameState *head){
	struct result res = {0};
	
	for(struct gameState *curr = head;;curr = curr->next){
		struct tree *other = get(curr);
		if(other){
			res.winner = other->winner;
			goto end_of_game;
		}
		for(struct gameState *walk = head; walk->next; walk = walk->next){
			if(memcmp(curr->decks, walk->decks, sizeof(curr->decks)) == 0){
				res.winner = 0;
				memcpy(res.deck, curr->decks[0], MAX_CARDS);
				res.cardCount = curr->cardCounts[0];
				goto end_of_game;
			}
		}
		struct gameState *next = malloc(sizeof(struct gameState));
		for(int i=0;i<2;i++){
			next->cardCounts[i] = curr->cardCounts[i]-1;
			memcpy(next->decks[i], curr->decks[i]+1, MAX_CARDS-1);
			next->decks[i][MAX_CARDS-1] = 0;
		}
		next->next = NULL;
		curr->next = next;
		
		int winner;
		if(next->cardCounts[0] >= curr->decks[0][0] && next->cardCounts[1] >= curr->decks[1][0]){
			struct gameState init = {0};
			for(int i=0;i<2;i++){
				init.cardCounts[i] = curr->decks[i][0];
				memcpy(init.decks[i], next->decks[i], init.cardCounts[i]);
			}
			
			struct result subgame = recursiveCombat(&init);
			winner = subgame.winner;
		}
		else winner = curr->decks[1][0] > curr->decks[0][0];
		
		next->decks[winner][next->cardCounts[winner]++] = curr->decks[winner][0];
		next->decks[winner][next->cardCounts[winner]++] = curr->decks[!winner][0];
		if(next->cardCounts[!winner] == 0){
			res.winner = winner;
			memcpy(res.deck, next->decks[winner], MAX_CARDS);
			res.cardCount = next->cardCounts[winner];
			goto end_of_game;
		}
		
	}
	
end_of_game:
	insert(head, res.winner);
	struct gameState *walk;
	for(walk = head->next; walk;){
		insert(walk, res.winner);
		struct gameState *tmp = walk->next;
		free(walk);
		walk = tmp;
	}
	
	return res;
}

int main(){
	uint8_t decks[2][MAX_CARDS] = {0};
	size_t cardCounts[2];
	
	for(int player = 0; player < 2; player++){
		
		for(int c = fgetc(stdin); c != EOF && c != '\n'; c = fgetc(stdin));
		
		int cards=0;
		for(int c = fgetc(stdin);c != '\n' && c != EOF; c = fgetc(stdin)){
			uint8_t n = c - '0';
			for(c = fgetc(stdin); c != '\n' && c != EOF; c = fgetc(stdin)) n = n*10 + c - '0';
			decks[player][cards++] = n;
		}
		cardCounts[player] = cards;
	}
	
	struct gameState init = {0};
	memcpy(init.cardCounts, cardCounts, sizeof(cardCounts));
	memcpy(init.decks, decks, sizeof(decks));
	
	
	struct result deckP2 = recursiveCombat(&init);
	long res1 = 0, res2 = 0;
	
	for(int i=0;i<deckP2.cardCount;i++) {
		res2 += (deckP2.cardCount-i) * deckP2.deck[i];
	}
	
	for(;;){
		int winner = *decks[1] > *decks[0];
		decks[winner][cardCounts[winner]++] = *decks[winner];
		decks[winner][cardCounts[winner]] = *decks[!winner];
		memcpy(decks[0], decks[0]+1, MAX_CARDS-1);
		memcpy(decks[1], decks[1]+1, MAX_CARDS-1);
		if(--cardCounts[!winner] == 0){
			for(int i=0;i<cardCounts[winner];i++) {
				res1 += (cardCounts[winner]-i) * decks[winner][i];
			}
			break;
		}
	}
	
	printf("Part 1: %ld\nPart 2: %ld\n", res1, res2);
}
