#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define sign(x) ((x > 0) - (x < 0))
#define arr_size 700 
#define arr_off 350

int main(int argc, char *argv[]){
	int res1=0, res2=0;
	int x[10], y[10];
	int arr[2][arr_size][arr_size];
	memset(arr, 0, 2*arr_size*arr_size*sizeof(int));
	memset(x, 0, 10*sizeof(int));
	memset(y, 0, 10*sizeof(int));
	
	for(int c = fgetc(stdin);c != EOF && c != '\n'; c = fgetc(stdin)){
		int diffX=0, diffY=0;
		switch(c){
			case('R'):
				diffX=1;
				break;
			case('L'):
				diffX=-1;
				break;
			case('U'):
				diffY=1;
				break;
			case('D'):
				diffY=-1;
				break;
		}
		
		fgetc(stdin);
		int i = fgetc(stdin) - '0';
		for(c = fgetc(stdin); c != '\n'; c = fgetc(stdin)) i= i*10 + c - '0';
		for(; i > 0; i--){
			x[0]+=diffX;
			y[0]+=diffY;
			for(int j=1;j<10;j++){
				if(x[j-1]-x[j] > 1 || x[j-1]-x[j] < -1 || y[j-1]-y[j] > 1 || y[j-1]-y[j] < -1){
					x[j]+=sign(x[j-1]-x[j]);
					y[j]+=sign(y[j-1]-y[j]);
				}
			}
			if(!arr[0][x[1]+arr_off][y[1]+arr_off]){
				arr[0][x[1]+arr_off][y[1]+arr_off] = 1;
				res1++;
			}
			if(!arr[1][x[9]+arr_off][y[9]+arr_off]){
				arr[1][x[9]+arr_off][y[9]+arr_off] = 1;
				res2++;
			}
		}
	}
	
	printf("Part 1: %d\nPart 2: %d\n", res1, res2);
}
