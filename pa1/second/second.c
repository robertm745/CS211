#include <stdio.h>
#include <stdlib.h>

#define SIZE 10000

typedef struct node {
    int value;
    struct node *next;
} node;

int hash(int num){
    return num % 10000;
}

node* newNode(int num) {
    node *temp = (node*) malloc(sizeof(node));
    temp->value = num;
    temp->next = NULL;
    return temp;
}

int search (int num, node **tabl){
    node **table = tabl;
    int index = hash(num);
    node *ptr = table[index];
    while(ptr != NULL) {
        if (ptr->value == num){
            return 1;
        }
        ptr = ptr->next;
    }
    return 0;
}

int insert(int num, node **table){
    int index = hash(num);
    node *temp = newNode(num);
    temp->next = table[index];
    table[index] = temp;
    return 0;
}

int main(int argc, char *argv[]) {

    node **table = (node**) malloc(sizeof(node*) * SIZE);
    
    int i;
    for (i = 0; i < SIZE; i++) {
        table[i] = NULL;
    }

    if (argc < 2) {
        printf("Need argument\n");
        return -1;
    }
    char *filename = argv[1];
    FILE *file = fopen(filename, "r");
    char *c;
    int num, found;
    while (fscanf(file, "%c\t%i\n", c, &num) != EOF){
        found = search(num, table);
        if (*c == 's') {
            if (found == 1)
                printf("present\n");
            else
                printf("absent\n");            
        }
        else {
            if (found == 1)
                printf("duplicate\n");
            else {
                insert(num, table);
                printf("inserted\n");
            }
        }
    }
    
	return 0;
}
