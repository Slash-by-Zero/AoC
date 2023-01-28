#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

static void die(char *msg){
	perror(msg);
	abort();
}

struct tile{
	int n;
	char img[8][8];
	struct tile *neighbours[4];
};

static void reorient(struct tile *this, int rot, int flip){
	{
		struct tile *tmp[rot];
		memcpy(tmp, this->neighbours, rot*sizeof(*tmp));
		memcpy(this->neighbours, this->neighbours+rot, (4-rot)*sizeof(*tmp));
		memcpy(this->neighbours+4-rot, tmp, rot*sizeof(*tmp));
	}
	
	if(flip){
		struct tile *tmp = this->neighbours[0];
		this->neighbours[0] = this->neighbours[2];
		this->neighbours[2] = tmp;
	}
}

#define MAX_LINE_LENGTH 128

int main(){
	char line[MAX_LINE_LENGTH];
	
	struct{
		int n;
		struct tile *t;
	} edgeCode[1024] = {0};
	
	struct tile *new[128];
	
	int count=0;
	for(;;){
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line) <= 1) break;
		new[count] = calloc(1, sizeof(**new));
		new[count]->n = strtol(line+5, NULL, 10);
		
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line) <= 1) break;
		
		int edges[4][2] = {0};
		for(int i=0;i<strlen(line)-1;i++){
			edges[0][0] <<=1;
			edges[0][0] |= line[i] == '#';
			edges[0][1] |= (line[i] == '#') << i;
		}
		
		edges[3][0] |= line[0] == '#';
		edges[3][1] |= line[0] == '#';
		
		edges[1][0] |= line[strlen(line)-2] == '#';
		edges[1][1] |= line[strlen(line)-2] == '#';
		
		for(int i=1;i<=8;i++){
			if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line) <= 1) break;
			
			memcpy(new[count]->img[i-1], line+1, 8);
			
			edges[3][0] <<=1;
			edges[3][0] |= line[0] == '#';
			edges[3][1] |= (line[0] == '#') << i;
			
			edges[1][1] <<=1;
			edges[1][1] |= line[strlen(line)-2] == '#';
			edges[1][0] |= (line[strlen(line)-2] == '#') << i;
		}
		
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line) <= 1) break;
		
		edges[3][0] <<=1;
		edges[3][0] |= line[0] == '#';
		edges[3][1] |= (line[0] == '#') << 9;
		
		edges[1][1] <<=1;
		edges[1][1] |= line[strlen(line)-2] == '#';
		edges[1][0] |= (line[strlen(line)-2] == '#') << 9;
		
		for(int i=0;i<strlen(line)-1;i++){
			edges[2][1] <<=1;
			edges[2][1] |= line[i] == '#';
			edges[2][0] |= (line[i] == '#') << i;
		}
		
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line) > 1) die("fgets");
		
		for(int i=0;i<4;i++){
			if(edgeCode[edges[i][0]].t){
				edgeCode[edges[i][0]].t->neighbours[edgeCode[edges[i][0]].n] = new[count];
				new[count]->neighbours[i] = edgeCode[edges[i][0]].t;
			}
			else{
				edgeCode[edges[i][0]].t = new[count];
				edgeCode[edges[i][0]].n = i;
				edgeCode[edges[i][1]].t = new[count];
				edgeCode[edges[i][1]].n = i;
			}
		}
		count++;
	}
	if(ferror(stdin)) die("fgets");
	
	long res1=1;
	
	int width = (int) sqrt((double) count);
	uint8_t img[width << 3][width];
	memset(img, 0, 8*width*width);
	
	struct tile *corner= NULL;
	int rot=0;
	for(int i=0;i<count;i++){
		int ns = 0;
		for(int j=0;j<4;j++) {
			if(new[i]->neighbours[j]){
				ns++;
				if(new[i]->neighbours[(j+1)&3]) rot = j;
			}
		}
		if(ns == 2) {
			corner = new[i];
			break;
		}
	}
	
	rot = (rot-1)&3;
	
	int flip = 0;
	reorient(corner, rot, flip);
	
	int l=0;
	static int loopArgs[2][2] = {{0,1},{7,-1}};
	for(struct tile *walk1 = corner;;l+=8){
		int k=0;
		
		for(struct tile *walk2 = walk1;;k++){
			for(int y = 0; y<8; y++){
				for(int x=0;x<8;x++){
					int tx, ty;
					if((rot&1) == 0){
						tx = loopArgs[rot>>1][0] + x * loopArgs[rot>>1][1];
						ty = loopArgs[rot>>1][0] + y * loopArgs[rot>>1][1];
					}
					else{
						tx = loopArgs[(rot>>1)^1][0] + y * loopArgs[(rot >> 1)^1][1];
						ty = loopArgs[(rot>>1)][0] + x * loopArgs[(rot >> 1)][1];
					}
					
					img[l + (flip ? (7-y) : y)][k] = (img[l + (flip ? (7-y) : y)][k] << 1) + (walk2->img[ty][tx] == '#');
				}
			}
			
			
			struct tile *next = walk2->neighbours[1];
			if(!next){
				walk1->n *= walk2->n;
				break;
			}
			
			struct tile *above = walk2->neighbours[0];
			if(above) above = above->neighbours[1];
			
			for(int j=0;j<4;j++) {
				if(next->neighbours[j] == walk2){
					rot = (j+1)&3;
					break;
				}
			}
			
			flip = next->neighbours[rot] != above;
			
			reorient(next, rot, flip);
			
			walk2 = next;
		}
		
		struct tile *next = walk1->neighbours[2];
		if(!next){
			res1 *= walk1->n;
			break;
		}
		
		for(int j=0;j<4;j++) {
			if(next->neighbours[j] == NULL && (next->neighbours[(j-1)&3] == walk1 || next->neighbours[(j+1)&3] == walk1)){
				rot = (j+1)&3;
				break;
			}
		}
		
		flip = next->neighbours[rot] != walk1;
		
		reorient(next, rot, flip);
		
		walk1 = next;
	}
	res1 *= corner->n;
	
	static int horr_monster[3] = {
		0x00002, 
		0x86187, 
		0x49248
	};
	
	static int ver_monster[20] = {
		0x2,
		0x6,
		0x2,
		0x1,
		0x0,
		0x0,
		0x1,
		0x2,
		0x2,
		0x1,
		0x0,
		0x0,
		0x1,
		0x2,
		0x2,
		0x1,
		0x0,
		0x0,
		0x1,
		0x2
	};
	
	uint8_t img_cpy[width << 3][width];
	memcpy(img_cpy, img, sizeof(img));
	
	for(int i=0;i < 3;i++){
		horr_monster[i] <<= 4;
	}
	
	for(int i=0;i < (width << 3); i++){
		uint32_t curr[3];
		for(int k=0;k<3;k++){
			curr[k] = (((uint32_t) img[i+k][0]) << 16) | (((uint32_t) img[i+k][1]) << 8) | (((uint32_t) img[i+k][2]) << 0);
			horr_monster[k] <<= 5;
		}
		for(int j=3;;){
			int match = 1;
			for(int k=0;k<3;k++) match &= (horr_monster[k] & curr[k]) == horr_monster[k];
			if(match) for(int k=0;k<3;k++) for(int l=0;l<4;l++) if(j-1-l >= 0) img_cpy[i+k][j-1-l] &= ~((horr_monster[k] >> (8 * l)) & 0xff) ;
			
			if((horr_monster[1] & 1) == 1){
				if(j >= width) break;
				for(int k=0;k<3;k++){
					horr_monster[k] <<= 8;
					curr[k] = curr[k] << 8 | img[i+k][j];
				}
				j++;
			}
			for(int k=0;k<3;k++) horr_monster[k] >>= 1;
		}
	}
	
	int res2 = 0;
	for(int i=0;i< width << 3; i++){
		for(int j=0;j<width;j++){
			for(int m = 1 << 7; m > 0; m >>= 1) if((img_cpy[i][j] & m) != 0) res2++;
		}
	}
	
	printf("Part 1: %ld\nPart 2: %d\n", res1, res2);
	
	exit(0);
}
