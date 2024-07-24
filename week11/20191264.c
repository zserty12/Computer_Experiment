#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char** maze;
int** set;

int main() {
	//double start = clock();
	int width, height, row, col;
	int setnum = 1;
	int i, j, k, m;
	scanf("%d %d", &col, &row);
	width = 2 * col + 1;
	height = 2 * row + 1;
	maze = (char*)malloc(sizeof(char*) * height);
	for (i = 0; i < height; i++)
		maze[i] = (char**)malloc(sizeof(char**) * width);;
	for (i = 0; i < height; i++) {
		if (i % 2 == 0) {
			for (j = 0; j < width; j += 2) {
				if (j == width - 1) maze[i][j] = '+';
				else {
					maze[i][j] = '+';
					maze[i][j + 1] = '-';
				}
			}
		}
		else {
			for (j = 0; j < width; j += 2) {
				if (j == width - 1) maze[i][j] = '|';
				else {
					maze[i][j] = '|';
					maze[i][j + 1] = ' ';
				}
			}
		}
	}
	set = (int**)malloc(sizeof(int*) * row);
	for (i = 0; i < row; i++)
		set[i] = (int*)malloc(sizeof(int) * col);

	for (i = 0; i < row; i++)
		for (j = 0; j < col; j++)
			set[i][j] = 0;

	srand((unsigned int)time(NULL));
	int random;
	int set_idx = 0;
	//int idx_i, idx_j;
	for (i = 0; i < col; i++)
		set[0][i] = setnum++;
	for (i = 0; i < row-1; i++) {
		for (j = 0; j < col - 1; j++) {
			if (set[i][j] != set[i][j + 1]) {
				random = rand() % 2;
				if (random == 1) {
					maze[2 * i + 1][2 * j + 2] = ' ';
					int temp1 = set[i][j + 1];
					for (k = 0; k < row; k++){
						for (m = 0; m < col; m++) {
							if (set[k][m] == temp1)
								set[k][m] = set[i][j];
						}
					}
				}
			}
		}
		set_idx = 0;
		int random_check = 0;
		j = 0;
		while (1) {
			random = rand() % 2;
			random_check += random;
			if (random == 1) {
				set[i + 1][j] = set[i][j];
				maze[2 * i + 2][2 * j + 1] = ' ';
			}
			if (set[i][j] != set[i][j + 1] || j == col - 1) {
				if (random_check == 0) {
					j = set_idx;
					continue;
				}
				else {
					if (j == col - 1) {
						break;
					}
					random_check = 0;
					set_idx = j + 1;
				}
			}
			j++;
		}
		for (j = 0; j < col; j++) {
			if (set[i + 1][j] == 0) set[i + 1][j] = setnum++;
		}
	}
	if (i == row - 1) {
		for (j = 0; j < col - 1; j++) {
			if (set[i][j] != set[i][j + 1]) {
				maze[2 * i + 1][2 * j + 2] = ' ';
				int temp2 = set[i][j + 1];
				for (k = 0; k < row; k++) {
					for (m = 0; m < col; m++) {
						if (set[k][m] == temp2)
							set[k][m] = set[i][j];
					}
				}
			}
		}
	}

	
	FILE* fp = fopen("maze.maz", "w");
	if (fp == NULL) {
		printf("Error! No file!!\n");
		exit(0);
	}
	else {
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				fprintf(fp, "%c", maze[i][j]);
			}
			fprintf(fp, "\n");
		}

	}
	fclose(fp);

	/*for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}*/
	for (i = 0; i < height; i++)
		free(maze[i]);
	free(maze);
	for (i = 0; i < row; i++)
		free(set[i]);
	free(set);
	//double end = clock();
	//printf("%lf", (end - start) / CLOCKS_PER_SEC);
	return 0;
}
