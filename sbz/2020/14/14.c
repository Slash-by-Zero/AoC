#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_LINE_LENGTH 64

static void die(char *msg){
	perror(msg);
	abort();
}

struct mem{
	int addr;
	long val;
	struct mem *next;
} *head = NULL;

static void setaddr(int addr, long val){
	struct mem **walk = &head;
	for(; *walk; walk = &(*walk)->next){
		if((*walk)->addr == addr){
			(*walk)->val = val;
			return;
		}
		else if((*walk)->addr > addr) break;
	}
	
	struct mem *new = malloc(sizeof(*new));
	if(!new) die("malloc");
	
	new->addr = addr;
	new->val = val;
	new->next = *walk;
	*walk = new;
}

long sum(){
	long total=0;
	for(struct mem *walk=head;walk;walk=walk->next){
		total+=walk->val;
	}
	return total;
}

struct write{
	long mask_x, addr, val;
	struct write *next;
} *last_write = NULL;

long count_writes(long mask_x, long addr, struct write *toProcess, struct write *this){
	if(toProcess == this){
		long count = 1;
		for(;mask_x > 0;mask_x >>= 1){
			if((mask_x & 1) != 0){
				count <<=1;
			}
		}
		return count;
	}
	
	long mask = 0x1L << 35;
	long count = 0;
	long addr_xor = addr ^ toProcess->addr;
	
	for(;mask > 0;mask >>= 1){
		if((toProcess->mask_x & mask) != 0) continue;
		else if((mask_x & mask) != 0){
			mask_x &= ~mask;
			
			if((toProcess->addr & mask) != 0) addr ^= mask;
			
			count += count_writes(mask_x, addr ^ mask, toProcess->next, this);
		}
		else if((addr_xor & mask) != 0){
			return count + count_writes(mask_x, addr, toProcess->next, this);
		}
	}
	
	return count;
}

int main(){
	long mask_1 = 0, mask_0 = -1, res2=0;
	
	char line[MAX_LINE_LENGTH];
	
	for(;;){
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line) <= 1) break;
		
		if(strncmp(line, "mask", 4) == 0){
			mask_1 = 0;
			mask_0 = -1;
			for(int i=7;i<strlen(line)-1;i++){
				mask_1 = mask_1 << 1;
				mask_0 = mask_0 << 1;
				
				if(line[i] == '1'){
					mask_1++;
					mask_0++;
				}
				else if(line[i] == 'X'){
					mask_0++;
				}
			}
		}
		else{
			char *endptr;
			int addr = strtol(line+4, &endptr, 10);
			long val = strtol(endptr+4, NULL, 10);
			
			struct write *new = malloc(sizeof(*new));
			if(!new) die("malloc");
			
			new->mask_x = (mask_1 ^ mask_0) & 0xfffffffffL;
			new->addr = (addr | mask_1) & (~new->mask_x);
			new->val = val;
			new->next = last_write;
			last_write = new;
			
			setaddr(addr, (val&mask_0)|mask_1);
		}
	}
	if(ferror(stdin)) die("fgets");
	
	for(struct write *walk = last_write; walk; walk=walk->next){
		long writes = count_writes(walk->mask_x, walk->addr, last_write, walk);
		res2 += walk->val * writes;
	}
	
	printf("Part 1: %ld\nPart 2: %ld\n", sum(), res2);
}
