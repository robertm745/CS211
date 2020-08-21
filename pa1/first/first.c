#include <stdio.h>
#include <stdlib.h>

int sortOdds(int *array, int size){
	int i, j, temp;
	j = 0;
	int *result = (int*) malloc(sizeof(int) * size);
	for (i = 0; i < size; i++) {
		if (array[i] % 2 == 1) {
			// printf("%d\t", array[i]);
			result[j] = array[i];
			j++;
		}
	}
	temp = j;
	//printf("\n");
	//printf("%d is temp in function\n", temp);
	for (i = 0; i < size; i++){
		if (array[i] % 2 == 0){
			result[j] = array[i];
			j++;
		}
	}
	for (int i = 0; i < size; i ++){
		array[i] = result[i];
	}
	/*
	printf("\n");
	for (int i = 0; i < size; i++){
		printf("%d\t", array[i]);
	}
	printf("\n");
	*/
	return temp;
}

int * sort(int *array, int size) {
	int i, j, min, temp;
	for (i = 0; i < size-1; i++){
		min = i;
		for (j = i + 1; j < size; j++){
			if (array[j] < array[min])
				min = j;
		}
		temp = array[i];
		array[i] = array[min];
		array[min] = temp;
	}
	printf("in sorted function:\n");
	for (int i = 0; i < size; i++){
		printf("%d\t", array[i]);
	}
	printf("\n");
	return array;
}

int * revsort(int *array, int size, int start) {
	int i, j, max, temp;
	for (i = start; i < size-1; i++){
		max = i;
		for (j = i + 1; j < size; j++){
			if (array[j] > array[max])
				max = j;
		}
		temp = array[i];
		array[i] = array[max];
		array[max] = temp;
	}
	return array;
}

int main(int argc, char *argv[]) {	
	
	if (argc < 2){
		printf("Need an argument\n");
		return -1;
	}

	/*
	int arr[] = {5, 4, 3, 0, 1, 2};
	sort(&arr, 3);
	revsort(&arr, 6, 3);
	printf("test sort:\n");
	for (int i = 0; i < 6; i++){
		printf("%d\t", arr[i]);
	}
	*/

	char *filename = argv[1];
	FILE *infile = fopen(filename, "r");
	int arraySize;
	fscanf(infile, "%d\n", &arraySize);
	printf("Array size is %d\n", arraySize);
	int *array = (int*) malloc(sizeof(int) * arraySize);
	for (int i = 0; i < arraySize; i++) {
		fscanf(infile, "%d", array + i);
	}
	int temp = sortOdds(array, arraySize);
	
	
	printf("sortOdds array:\n");
	for (int i = 0; i < arraySize; i++){
		printf("%d\t", array[i]);
	}
	printf("\n");
	

	printf("%d is temp in main\n", temp);

	sort(array, temp);
	revsort(array, arraySize, temp);

	printf("final array w/o front sort:\n");
	for (int i = 0; i < arraySize; i++){
		printf("%d\n", array[i]);
	}

	fclose(infile);
	free(array);
	return 0;
}
