#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	int res1=0, res2=0;
	char line[128][128];
	char vis[128][128];
	int scores[128][128]; 
	memset(vis, 0, 128*128);
	memset(scores, 0, 4*128*128);
	
	for(int j=0;;j++){
		if(!fgets(line[j], 128, stdin) || strlen(line[j]) <= 1){
			memset(line[j], 0, 128);
			break;
		}
		line[j][strlen(line[j])-1] = '\0';
	}
	
	for(int i = 0; line[i][0] != '\0';i++){
		int left = '0'-1;
		int right[10];
		memset(right, 0, 40);
		for(int j=0; line[i][j] != '\0'; j++){
			if(line[i][j] > left){
				vis[i][j]=1;
				left = line[i][j];
			}
			right[line[i][j] - '0'] = j;
			memset(right, 0, 4 * (line[i][j]-'0'));
		}
		for(int j=0; j<10; j++){
			vis[i][right[j]] = 1;
		}
	}
	
	for(int i = 0; line[0][i] != '\0';i++){
		int top = '0'-1;
		int bot[10];
		memset(bot, 0, 40);
		for(int j=0; line[j][i] != '\0'; j++){
			if(line[j][i] > top){
				vis[j][i]=1;
				top = line[j][i];
			}
			bot[line[j][i] - '0'] = j;
			memset(bot, 0, 4 * (line[j][i]-'0'));
		}
		for(int j=0; j<10; j++){
			vis[bot[j]][i] = 1;
		}
	}
	
	for(int i=0; i<128; i++){
		for(int j=0;j<128;j++){
			res1+=vis[i][j];
			
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
			scores[i][j] = score;
			if(score > res2) res2 = score;
		}
	}
	
	printf("Part 1: %d\nPart 2: %d\n", res1, res2);
}
