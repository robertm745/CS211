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

double** transpose (double ***mat, int n, int m) {
        double **matrix = *mat;
        double **tmat = (double**) malloc(sizeof(double*) * m);
        if (tmat == NULL) {
                printf("Out of memory A\n");
                return NULL;
        }
        int i, j;
        for (i = 0; i < m; i++) {
                tmat[i] = (double*) malloc(sizeof(double) * n);
                if (tmat[i] == NULL) {
                        printf("Out of memory B\n");
                        return NULL;
                }
        }
        for (i = 0; i < m; i++) {
                for (j = 0; j < n; j++) {
                        tmat[i][j] = matrix[j][i];
                }
        }
        //double*** ret = tmat;
        return tmat;
}

double** multiply(double ***mat1, int n1, int m1, double ***mat2, int n2, int m2) {
        if (m1 != n2) {
                printf("Invalid input matrices\n");
                return NULL;
        }
        double **A = *mat1;
        double **B = *mat2;
        double **pmat = (double**) malloc(sizeof(double*) * n1);
        if (pmat == NULL) {
                printf("Out of memory C\n");
                return NULL;
        }
        int h, i, j, k;
        for (i = 0; i < n1; i++) {
                pmat[i] = (double*) malloc(sizeof(double) * m2);
                if (pmat[i] == NULL) {
                        printf("Out of memory D\n");
                        return NULL;
                }
        }
        double sum = 0;
        for (i = 0; i < n1; i++) {
                for (j = 0; j < m2; j++) {
                        for (k = 0; k < n2; k++) {
                                sum += A[i][k] * B[k][j];
                        }
                        pmat[i][j] = sum;
                        sum = 0;
                }
        }
        //printf("Matrix product: \n");
        //printMatrix(&pmat, n1, m2);
        //double ***temp = pmat;
        return pmat;
}

double** rowReduce(double ***mat, int n, int m) {
	double **matrix = *mat;
	int i, j, k, a, b;
        double c, temp;
	for (i = 0; i < (n-1); i++) {
                temp = matrix[i][i];
                for (a = i; a < m; a++) {
                        matrix[i][a] = matrix[i][a]/temp;
                }
		for (j = i+1; j < n; j++) {
                        c = matrix[j][i]/matrix[i][i];
			for (k = i; k < m; k++) {
                                matrix[j][k] = matrix[j][k] - (c*matrix[i][k]);
                        }
		}
                //printMatrix(&matrix, n, m);
	}
        //printf("rref step 1\n");
        //printMatrix(&matrix, n, m);
        for (a = (n-1); a >= 0; a--) {
                temp = matrix[a][a];
                for (b = a; b < m; b++) {
                        matrix[a][b] = matrix[a][b]/temp;
                }
                for (i = (n-1); i >= 0; i--) {
                        for (j = (i-1); j >= 0; j--) {
                                c = matrix[j][i]/matrix[i][i];
                                for (k = i; k < m; k++) {
                                        matrix[j][k] = matrix[j][k] - (c*matrix[i][k]);
                                }
                        }
                }
                //printMatrix(&matrix, n, m);
        }

 
        // printf("rref final:\n");
        // printMatrix(&matrix, n, m);
        return matrix;

}

double** inverse(double ***mat, int n, int m){
        double **matrix = *mat;
        double **inv = (double**) malloc(sizeof(double*) * n);
        if (inv == NULL){
                printf("Out of memory\n");
                return NULL;
        }
        int i, j;
        for (i = 0; i < n; i++) {
                inv[i] = (double*) malloc(sizeof(double) * m * 2);
                if (inv[i] == NULL){
                        printf("Out of memory\n");
                        return NULL;
                }
        }
        for (i = 0; i < n; i++){
                for (j = 0; j < (2*m); j++){
                        if (i == j-m)
                                inv[i][j] = 1;
                        else if (j < m)
                                inv[i][j] = matrix[i][j];
			else 
				inv[i][j] = 0;
                }
        }
        // printf("Inv mat: \n");
        // printMatrix(&inv, n, (2*m));
	double **tmat = rowReduce(&inv, n, (2*m));
        for (i = 0; i < n; i++) {
                for (j = 0; j < n; j++){
                        inv[i][j] = tmat[i][j+n];
                }
        }


        return inv;
}

int main(int argc, char *argv[]){
        if (argc < 2) {
                printf("Need argument\n");
                return -1;
        }
        char *filename = argv[1];
        char *testfile = argv[2];
        FILE *file = fopen(filename, "r");
        FILE *tfile = fopen(testfile, "r");
        int n, m;
        fscanf(file, "%d\n", &m);
        m++;
        fscanf(file, "%d\n", &n);
        double **matrix = (double**) malloc(sizeof(double*) * n);
        double **y = (double**) malloc(sizeof(double*) * n);
        if (matrix == NULL || y == NULL) {
                printf("Out of memory E\n");
                return -1;
        }
        int i, j;
        for (i = 0; i < n; i++) {
                matrix[i] = (double*) malloc(sizeof(double) * m);
                if (matrix[i] == NULL) {
                        printf("Out of memory F\n");
                        return -1;
                }
        }
        for (i = 0; i < n; i++) {
                y[i] = (double*) malloc(sizeof(double));
                if (y[i] == NULL) {
                        printf("Out of memory F\n");
                        return -1;
                }
        }
        for (i = 0; i < n; i++) {
                for (j = 0; j < m; j++) {
                        if (j == 0){
                                fscanf(file, "%lf,", &y[i][0]);
                                matrix[i][j] = 1;
                                continue;
                        }
                        else
                                fscanf(file, "%lf,", &matrix[i][j]);
                }
                fscanf(file, "\n");
        }
        printf("Y vector: \n");
        printMatrix(&y, n, 1);
        printf("Original: \n");
        printMatrix(&matrix, n, m);
        double **tmat = transpose(&matrix, n, m);
        printf("Transpose: \n");
        printMatrix(&tmat, m, n);
        double **pmat = multiply(&tmat, m, n, &matrix, n, m);
        printf("Transpose * Original: \n");
        printMatrix(&pmat, m, m);
	double **inv = inverse(&pmat, m, m);
        printf("Inverse: \n");
        printMatrix(&inv, m, m);
        double **cmat = multiply(&tmat, m, n, &y, n, 1);
        printf("Transpose * y\n");
        printMatrix(&cmat, m, 1);
        double **w = multiply(&inv, m, m, &cmat, m, 1);
        printf("W:\n");
        printMatrix(&w, m, 1);
        int x;
        fscanf(tfile, "%d\n", &x);
        double **fmat = (double**) malloc(sizeof(double*) * x);
        if (fmat == NULL) {
                printf("Out of memory\n");
                return -1;
        }
        for (i = 0; i < x; i++) {
                fmat[i] = (double*) malloc(sizeof(double) * (m-1));
                if (fmat[i] == NULL) {
                        printf("Out of memory\n");
                        return -1;
                }
        }
        for (i = 0; i < x; i++) {
                for (j = 0; j < (m-1); j++) {
                        fscanf(tfile, "%lf,", &fmat[i][j]);
                }
        }
        printf("Test:\n");
        printMatrix(&fmat, x, (m-1)); 
        double **result = (double**) malloc(sizeof(double*) * x);
        for (i = 0; i < x; i++) {
                result[i] = (double*) malloc(sizeof(double));
                result[i][0] = w[0][0];
        }
        for (i = 0; i < x; i++) {
                for (j = 0; j < (m-1); j++) {
                        result[i][0] += w[j+1][0] * fmat[i][j];
                }
        }
        printf("Result:\n");
        printMatrix(&result, x, 1);
        return 0;
}
