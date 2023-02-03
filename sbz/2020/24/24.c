#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static void die(char *msg){
	perror(msg);
	abort();
}

struct matrix{
	int dim[2];
	int minx, maxx;
	struct array **mem;
} ap_floor = {.dim = {0, -1}};

struct array{
	int dim[2];
	uint8_t *mem;
};

static int flipArr(struct array *this, int x){
	if(x < this->dim[0] || x >= this->dim[1]){
		int newSize = x > this->dim[1] ? x : this->dim[1];
		newSize -= (x < this->dim[0] ? (x) : this->dim[0]);
		newSize++;
		this->mem = realloc(this->mem, sizeof(*this->mem) * newSize);
		if(x < this->dim[0]){
			memcpy(this->mem + this->dim[0] - (x), this->mem, sizeof(*this->mem) * (this->dim[1] - this->dim[0] + 1));
			memset(this->mem, 0, this->dim[0] - (x));
			this->dim[0] = x;
		}
		else if(x > this->dim[1]){
			memset(this->mem + this->dim[1] - this->dim[0] + 1, 0, (x) - this->dim[1]);
			this->dim[1] = x;
		}
	}
	
	this->mem[x - this->dim[0]] = !this->mem[x-this->dim[0]];
	
	return ((this->mem[x-this->dim[0]]) & 1) * 2 - 1; 
}

static int flip(struct matrix *this, const int x[2]){
	if(*x < this->dim[0] || *x >= this->dim[1]){
		int newSize = *x > this->dim[1] ? *x : this->dim[1];
		newSize -= (*x < this->dim[0] ? (*x) : this->dim[0]);
		newSize++;
		this->mem = realloc(this->mem, sizeof(*this->mem) * newSize);
		if(*x < this->dim[0]){
			memcpy(this->mem + this->dim[0] - (*x), this->mem, sizeof(*this->mem) * (this->dim[1] - this->dim[0]+1));
			memset(this->mem, 0, sizeof(*this->mem) * (this->dim[0] - (*x)));
			this->dim[0] = *x;
		}
		else if(*x > this->dim[1]){
			memset(this->mem + this->dim[1] - this->dim[0] + 1, 0, sizeof(*this->mem) * ((*x) - this->dim[1]));
			this->dim[1] = *x;
		}
	}
	
	if(!this->mem[(*x) - this->dim[0]]){
		this->mem[(*x) - this->dim[0]] = malloc(sizeof(**this->mem));
		this->mem[(*x) - this->dim[0]]->dim[0] = 0;
		this->mem[(*x) - this->dim[0]]->dim[1] = -1;
		this->mem[(*x) - this->dim[0]]->mem = NULL;
	}
	
	if(x[1] < this->minx) this->minx = x[1];
	if(x[1] > this->maxx) this->maxx = x[1];
	
	return flipArr(this->mem[(*x) - this->dim[0]], x[1]);
}

static int getArr(struct array *this, int x){
	if(x < this->dim[0] || x > this->dim[1]) return 0;
	
	return this->mem[x - this->dim[0]] != 0;
}

static int get(int y, int x){
	if(y < ap_floor.dim[0] || y > ap_floor.dim[1] || ap_floor.mem[(y) - ap_floor.dim[0]] == NULL) return 0;
	
	return getArr(ap_floor.mem[(y) - ap_floor.dim[0]], x);
}

static int simStep(){
	struct matrix ap_floor_new = {.dim = {0, -1}};
	int tiles = 0;
	
	for(int y = ap_floor.dim[0] - 1; y <= ap_floor.dim[1] + 1; y++){
		int neighbors[3][3] = {0};
		for(int x = ap_floor.minx - 1; x <= ap_floor.maxx + 1; x++){
			memcpy(neighbors, neighbors[1], 2*sizeof(*neighbors));
			for(int i=0;i<3;i++) neighbors[2][i] = get(y+i-1, x+1);
			int count = 0;
			for(int i=0;i<3;i++) for(int j=0;j<3;j++) count += neighbors[i][j];
			if((y&1) == 0) count -= neighbors[0][0] + neighbors[0][2];
			else count -= neighbors[2][0] + neighbors[2][2];
			
			
			if(count == 2 || (count == 3 && neighbors[1][1])){
				int coords[2] = {y, x};
				flip(&ap_floor_new, coords); 
				tiles++;
			}
		}
	}
	
	for(int i = 0; i < ap_floor.dim[1] - ap_floor.dim[0] + 1; i++){
		if(ap_floor.mem[i]) free(ap_floor.mem[i]->mem);
		free(ap_floor.mem[i]);
	}
	
	free(ap_floor.mem);
	
	memcpy(&ap_floor, &ap_floor_new, sizeof(ap_floor));
	return tiles;
}

int main(){
	int res1 = 0, res2 = 0;
	
	for(int c = fgetc(stdin); c!=EOF && c!= '\n'; c = fgetc(stdin)){
		int coords[2] = {0};
		for(;c!=EOF && c!='\n'; c = fgetc(stdin)){
			switch(c){
				case('e'):
					coords[1]++;
					break;
				case('w'):
					coords[1]--;
					break;
				case('n'):
					coords[0]-=2; // So that after the ++ is executed its basically +1 and we can ahndle north and south in one go
				case('s'):
					coords[0]++;
					if((coords[0]&1) == 0) coords[1]--;
					if((c = fgetc(stdin)) == 'e') coords[1]++;
					else if(c != 'w') die("fgetc");
					break;
			}
		}
		res1 += flip(&ap_floor, coords);
	}
	
	for(int i=0;i<100;i++) res2 = simStep();
	
	printf("Part 1: %d\nPart 2: %d\n", res1, res2);
}
