#include <stdio.h>
#include <stdlib.h>

void printMatrix(double ***mat, int n, int m) {
	double **matrix = *mat;
	printf("Matrix:\n");
	int i, j;
	for (i = 0; i < n; i ++) {
		for (j = 0; j < m; j++){
			printf("%lf\t", matrix[i][j]);
		}
		printf("\n");
	}
	return;
}

int main(int argc, char *argv[]){
	if (argc < 2) {
		printf("Need argument\n");
		return -1;
	}
	char *filename = argv[1];
	FILE *file = fopen(filename, "r");
	int n, m;
	fscanf(file, "%d\n", &n);
	fscanf(file, "%d\n", &m);
	double **matrix = (double**) malloc(sizeof(double*) * n);
	int i, j;
	for (i = 0; i < n; i++) {
		matrix[i] = (double*) malloc(sizeof(double) * m);
	}
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			matrix[i][j] = 0;
		}
	}
	printf("matrix 0,0 = %lf\n", matrix[0][0]);
	int x = sizeof(matrix[0][0]);
	printMatrix(&matrix, n, m);
	return 0;
}