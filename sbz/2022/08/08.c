#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	int res1=0, res2=0;
	char line[128][128];
	char vis[128][128];
	char top[128];
	int bot[128][10];
	memset(vis, 0, 128*128);
	memset(top, 0, 128);
	memset(bot, 0, 4*128*10);
	
	for(int j=0;;j++){
		if(!fgets(line[j], 128, stdin) || strlen(line[j]) <= 1){
			memset(line[j], 0, 128);
			break;
		}
		line[j][strlen(line[j])-1] = '\0';
	}
	
	for(int i = 0; line[i][0] != '\0';i++){
		char left = '0'-1;
		int right[10];
		memset(right, 0, 40);
		for(int j=0; line[i][j] != '\0'; j++){
			if(line[i][j] > top[j]){
				vis[i][j] = 1;
				top[j] = line[i][j];
			}
			if(line[i][j] > left){
				vis[i][j]=1;
				left = line[i][j];
			}
			right[line[i][j] - '0'] = j;
			memset(right, 0, 4 * (line[i][j]-'0'));
			bot[j][line[i][j] - '0'] = i;
			memset(bot[j], 0, 4 * (line[i][j]-'0'));
			
			int score=1;
			for(int k = 1;;k++){
				if(i - k < 0){
					score *= k-1;
					break;
				}
				else if(line[i-k][j] >= line[i][j]){
					score *= k;
					break;
				}
			}
			for(int k = 1;;k++){
				if(line[i + k][j] == '\0'){
					score *= k-1;
					break;
				}
				else if(line[i+k][j] >= line[i][j]){
					score *= k;
					break;
				}
			}
			for(int k = 1;;k++){
				if(j - k < 0){
					score *= k-1;
					break;
				}
				else if(line[i][j-k] >= line[i][j]){
					score *= k;
					break;
				}
			}
			for(int k = 1;;k++){
				if(line[i][j+k] == '\0'){
					score *= k-1;
					break;
				}
				else if(line[i][j+k] >= line[i][j]){
					score *= k;
					break;
				}
			}
			if(score > res2) res2 = score;
		}
		for(int j=0; j<10; j++){
			vis[i][right[j]] = 1;
		}
	}
	
	for(int i = 0; i<128;i++){
		for(int j=0; j<10; j++)
		{
			if(bot[i][j] != 0) vis[bot[i][j]][i] = 1;
		}
		for(int j=0;j<128;j++){
			res1+=vis[j][i];
		}
	}
	
	printf("Part 1: %d\nPart 2: %d\n", res1, res2);
}
