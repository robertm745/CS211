#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Node stores index of tag in the relevant set of cache and the next node in LL
typedef struct node {
	long long int value;
	struct node *next;
} Node;

// inserts node into LL of relevant set where the front holds the least-recently-used and the back holds most-recently-used
void insert(Node **lru, int set, long long int val) {
	Node *ptr = lru[set];
	while (ptr->next != NULL) 
		ptr = ptr->next;
	Node *temp = (Node*) malloc(sizeof(Node));
	temp->value = val;
	temp->next = NULL;
	ptr->next = temp;
	return;
}

// updates node after a cache hit
void update(Node **lru, int set, long long int val) {
	Node *ptr = lru[set], *prev = NULL;
	if (ptr->next == NULL)
		return;
	while (ptr != NULL) {
		if (ptr->value == val) 
			break;
		prev = ptr;
		ptr = ptr->next;
	}
	if (prev == NULL) 
		lru[set] = ptr->next;
	else 
		prev->next = ptr->next;
	free(ptr);
	insert(lru, set, val);
	return;
}
 
// log base 2 utility
int mlog2(int value) {
	if (value < 1)
		return 0;
	int count = 0;
	while (value != 1) {
		value = value/2;
		count++;
	}
	return count;
}

// checks if given tag value is in cache and return index of tag; also if value = 0, returns the next unused line in the cache if any remain
int contains(long long int **arr, int lines, int setNum, long long int value) {
	int i;
	for (i = 0; i < lines; i++) {
		if (arr[setNum][i] == value)
			return i;
	}
	return -1;
}

// free LRU nodes
void freeList(Node **lru, int sets) {
	Node *ptr, *temp;
	int i;
	for (i = 0; i < sets; i++) {
		ptr = lru[i];
		while (ptr != NULL) {
			temp = ptr;
			ptr = ptr->next;
			free(temp);
		}
	}
	return;
}
 

int main(int argc, char *argv[]){
// store cache and block sizes
	int cacheSize = atoi(argv[1]), blockSize = atoi(argv[2]), opt;
	if (cacheSize < 1) {
		printf("Invalid argument format(s) - A\n");
		return 1;
	}
	if (blockSize < 1 || blockSize > cacheSize) {
		printf("Invalid argument format(s) - B\n");
		return 1;
	}
	char *policy = argv[3], *p1 = "fifo", *p2 = "lru";
// determine eviction policy
	if (!strcmp(policy, p1)) {
		opt = 1;
	}
	else {
		if (strcmp(policy, p2)) {
			printf("Invalid argument format(s) - C\n");
			return 1;
		}
		else 
			opt = 2;
	}
	char *assoc = argv[4], *filename = argv[5];
	int lines = cacheSize/blockSize, sets, i = 6, j;
// manual string parser to determine associativity
	if (strlen(assoc) < 5) { 
		printf("Invalid argument format(s) - D\n");
		return 1;
	}
	else if (strlen(assoc) == 6)
		sets = lines;
	else if (strlen(assoc) == 5)
		sets = 1;
	else {
		while (assoc[i] != '\0') {
			i++;
		}
		i -= 5;
		char temp[i];
		int t = 6;
		for (j = 0; j < i; j++){
			temp[j] = assoc[t++];
		}	
		temp[j] = '\0';
		sets = lines/atoi(temp);
	}	
	int setlines = lines/sets;
//	create 2D array of ints to store the tag of each address, where col = set and row = cache line
	long long int **arr = (long long int**) malloc(sizeof(long long int*) * sets);	
//	create 1D array to track FIFO index for each set
	int *fifo = (int*) malloc(sizeof(int) * sets);
//	create array of Linked Lists where each index corresponds to each set's LL that holds the order of LRU addresses;
	Node **lru = (Node**) malloc(sizeof(Node*) * sets);
//	initialize relevant eviction policy
	if (opt == 1) {
		if (fifo == NULL || arr == NULL) {
			printf("Out of memory - A1\n");
			return 1;
		}
		for (i = 0; i < sets; i++) {
			arr[i] = (long long int*) malloc(sizeof(long long int) * setlines);
			if (arr[i] == NULL) {
				printf("Out of memory - B1\n");
				return 1;
			}
			for (j = 0; j < setlines; j++) 
				arr[i][j] = 0;
			fifo[i] = 0;
		}
	}
	else {
		if (lru == NULL || arr == NULL) {
			printf("Out of memory - A2\n");
			return 1;
		}
		for (i = 0; i < sets; i++) {
			arr[i] = (long long int*) malloc(sizeof(long long int) * setlines);
			if (arr[i] == NULL) {
				printf("Out of memory - B2\n");
				return 1;
			}
			for (j = 0; j < setlines; j++) 
				arr[i][j] = 0;
			lru[i] = (Node*) malloc(sizeof(Node));
			if (lru[i] == NULL) {
				printf("Out of memory - C\n");
				return 1;
			}
			lru[i]->value = 0;
			lru[i]->next = NULL;
		}
	}
//	open file
	FILE *file = fopen(filename, "r");
	if (file == NULL) {	
		printf("Invalid argument format(s) - E\n");
		return 1;
	}
	char c;
	long long int value, tag, temptag;
	int index, blockOffset, setOffset, set, hits = 0, misses = 0, reads = 0, writes = 0, t;
	Node *temp;
	blockOffset = mlog2(blockSize);
	setOffset  = mlog2(sets);
// scan through input file line by line
	while (fscanf(file, "%c %llx\n", &c, &value) && c != '#') {
//		get set value
		set = (value >> blockOffset) & ((1 << (setOffset)) - 1);
//		get tag value
	 	tag = value >> (setOffset + blockOffset);
//		switch based on eviction policy
		switch (opt) {
			case 1:
			// switch on R or W
				switch (c) { 
					case 'R':
					// check if in cache
						if (contains(arr, setlines, set, tag) != -1){
							hits++;
						}
						else {
							misses++;	
							reads++;
						// if not in cache, check if any unused lines left
							t = contains(arr, setlines, set, 0);
							if (t != -1) {
							// set next available unused line to missed address; address now in cache
								arr[set][t] = tag;
							}
							else {
							// if no unused lines left, need to evict; fifo[set] holds next index of set to evict
								arr[set][fifo[set]] = tag;
							// increment fifo[set] to next index for future eviction and check if at the last line of the set; if so, reset to index 0
								fifo[set]++;
								if (fifo[set] == setlines)
									fifo[set] = 0;
							}
						}
					break;
				// repeat above steps for case W; increment writes 
					case 'W':
						writes++;
						if (contains(arr, setlines, set, tag) != -1){
							hits++;
						}
						else {
							misses++;	
							reads++;
							t = contains(arr, setlines, set, 0);
							if (t != -1) {
								arr[set][t] = tag;
							}
							else {
								arr[set][fifo[set]] = tag;
								fifo[set]++;
								if (fifo[set] == setlines)
									fifo[set] = 0;
							}
						}
					break;
				}
			break;
		// case 2 = LRU policy
		// I was off by a small amount for most LRU cases, I couldn't figure out why
			case 2:
				switch (c) {
					case 'R':
					// check if in cache
						t = contains(arr, setlines, set, tag);
						if (t != -1){
							hits++;
					// given address is in cache at arr[set][t], update set's corresponding LRU linked list so this index is moved to most-recently-used position of LL
							update(lru, set, tag);
						}
						else {
							misses++;
							reads++; 
						// check if any unused lines; if so, insert this index into set's LRU LL at most-recently-used position 
							t = contains(arr, setlines, set, 0);
							if (t != -1) {
								arr[set][t] = tag;
								insert(lru, set, tag);
							}
							else {
							// given no unused lines, evict; LRU index of this set is at the front of LRU's LL for this set
								temptag = lru[set]->value;
							// overwrite cache line at LRU index
								t = contains(arr, setlines, set, temptag);
								arr[set][t] = tag;
							// insert index into most-recently-used positon in LRU LL
								insert(lru, set, tag);
							// if only one node in LRU LL, no update needed
								if (lru[set]->next != NULL) {
								// save old front of LL
									temp = lru[set];
								// update LRU front to new least-recently-used index
									lru[set] = lru[set]->next;
								// free old front node
									free(temp);
								}
							}
						}
					break;
				// repeat above for case W
					case 'W':
						writes++;
						t = contains(arr, setlines, set, tag);
						if (t != -1){
							hits++;
							update(lru, set, tag);
						}
						else {
							misses++;	
							reads++;
							t = contains(arr, setlines, set, 0);
							if (t != -1) {
								arr[set][t] = tag;
								insert(lru, set, tag);
							}
							else {
								temptag = lru[set]->value;
								t = contains(arr, setlines, set, temptag);
								arr[set][t] = tag;
								insert(lru, set, tag);
								if (lru[set]->next != NULL) {
									temp = lru[set];
									lru[set] = lru[set]->next;
									free(temp);
								}
							}
						}
					break;
				}
			break;
		 }
	}
	freeList(lru, sets);
	free(arr);
	free(fifo);
	printf("Memory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n", reads, writes, hits, misses);
	return 0;
} 
