#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#define MAX_LINE_LENGTH 256

static long map[256][256];
static int cubeSize = INT_MAX;
static int move[4][2] = {{0,1},{1,0},{0,-1},{-1,0}};

#define OUT_OF_MAP(x, y) (map[y][x] < 0)

struct side{
	int x, y;
	struct side *neighbors[4];
} startingSide = {0}, *sides[6];

static int sideCount = 0;

static void createSide(struct side *this, int x, int y){
	this->x = x;
	this->y = y;
	
	sides[sideCount++] = this;
	
	for(int i=0;i<4;i++){
		int newX = x+move[i][1]*cubeSize, newY = y+move[i][0]*cubeSize;
		if(newX < 0 || newX >= 256 || newY < 0 || newY >= 256) continue;
		if(!this->neighbors[i] && !OUT_OF_MAP(newX, newY)){
			this->neighbors[i] = calloc(sizeof(**this->neighbors), 1);
			this->neighbors[i]->neighbors[i ^ 2] = this;
			
			
			
			createSide(this->neighbors[i], newX, newY);
		}
	}
}

static int complete(struct side *this, struct side *a, struct side *b){
	for(int i=0;i<4;i++){
		if(a->neighbors[i] == this){
			if(a->neighbors[(i-1) & 0x3]) return 0;
			a->neighbors[(i-1) & 0x3] = b;
			break;
		}
	}
	for(int i=0;i<4;i++){
		if(b->neighbors[i] == this){
			b->neighbors[(i+1) & 0x3] = a;
			break;
		}
	}
	return 1;
}

static void calcNeighbors(struct side *this){
	for(int i=0;i<4;i++){
		if(this->neighbors[i] && this->neighbors[(i+1) & 0x3] && complete(this, this->neighbors[i], this->neighbors[(i+1) & 0x3])){
			calcNeighbors(this->neighbors[i]);
			calcNeighbors(this->neighbors[(i+1) & 0x3]);
		}
	}
}

static void calcCube(int x, int y){
	createSide(&startingSide, x, y);
	for(int i=0;i<sideCount;i++) calcNeighbors(sides[i]);
}

int main(int argc, char *argv[]){
	long res1=0, res2=0;
	char line[MAX_LINE_LENGTH];
	
	memset(map, 0xff, 256*256*sizeof(**map));
	
	int row=1;
	
	for(;;){
		if(!fgets(line, MAX_LINE_LENGTH, stdin) || strlen(line)<=1) break;
		
		int lower=0, upper=strlen(line);
		for(int i=0;i<strlen(line);i++){
			if(line[i] == '.') map[row][i+1] = 0;
			else if(line[i] == '#') map[row][i+1] = 1;
			else lower++;
		}
		
		if(upper-lower<cubeSize) cubeSize=upper-lower;
		
		row++;
	}
	
	int c;
	int x=1, y=1;
	int dir=0;
	
	for(x=1; map[y][x] != 0;x++);
	
	calcCube(x, y);
	
	fpos_t start_of_instructions;
	
	fgetpos(stdin, &start_of_instructions);
	
	for(c = fgetc(stdin); c != EOF && c != '\n'; c = fgetc(stdin)){
		int n = c - '0';
		for(c = fgetc(stdin); c >= '0' && c <= '9'; c = fgetc(stdin)){
			n = n*10 + c - '0';
		}
		
		int prevx=x, prevy=y;
		for(int i=n;i>0; i--){
			x += move[dir][1];
			y += move[dir][0];
			
			if(OUT_OF_MAP(x,y)){
				
				for(;OUT_OF_MAP(x & 0xff, y & 0xff); x += (y += move[dir][0], move[dir][1]));
				
				x &= 0xff;
				y &= 0xff;
			}
			
			if(map[y][x] == 1){
				x=prevx;
				y=prevy;
				break;
			}
			
			prevx=x;
			prevy=y;
		}
		
		if(c == EOF || c == '\n') break;
		
		if(c == 'R') dir++;
		if(c == 'L') dir--;
		dir = (dir+4)%4;
	}
	
	res1 = 1000*y+4*x+dir;
	
	fsetpos(stdin, &start_of_instructions);
	
	x = 0;
	y = 0;
	dir = 0;
	struct side *currSide = &startingSide;
	
	for(c = fgetc(stdin); c != EOF && c != '\n'; c = fgetc(stdin)){
		int n = c - '0';
		for(c = fgetc(stdin); c >= '0' && c <= '9'; c = fgetc(stdin)){
			n = n*10 + c - '0';
		}
		
		int prevx=x, prevy=y, prevdir = dir;
		struct side *prevSide;
		for(int i=n;i>0; i--){
			x += move[dir][1];
			y += move[dir][0];
			
			if(x >= cubeSize || y >= cubeSize || x < 0 || y < 0){
				int off = 0;
				switch(dir){
					case(0):
						off = y;
						break;
					case(1):
						off = cubeSize - 1 - x;
						break;
					case(2):
						off = cubeSize - 1 - y;
						break;
					case(3):
						off = x;
						break;
				}
				
				currSide = currSide->neighbors[dir];
				for(dir = 0;;dir++) if(currSide->neighbors[dir ^ 2] == prevSide) break;
				
				switch(dir){
					case(0):
						x = 0;
						y = off;
						break;
					case(1):
						y = 0;
						x = cubeSize - 1 - off;
						break;
					case(2):
						x = cubeSize - 1;
						y = cubeSize - 1 - off;
						break;
					case(3):
						y = cubeSize - 1;
						x = off;
						break;
				}
			}
			
			if(map[y + currSide->y][x + currSide->x] == 1){
				x = prevx;
				y = prevy;
				dir = prevdir;
				currSide = prevSide;
				break;
			}
			
			prevSide = currSide;
			prevdir = dir;
			prevx=x;
			prevy=y;
		}
		
		if(c == EOF || c == '\n') break;
		
		if(c == 'R') dir++;
		if(c == 'L') dir--;
		dir = (dir+4) & 0x3;
	}
	
	res2 = 1000 * (y + currSide->y) + 4 * (x + currSide->x) + dir; 
	
	printf("Part 1: %ld\nPart 2: %ld\n", res1, res2);
}
