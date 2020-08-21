#include <stdlib.h>
#include <stdio.h>


typedef struct node {
	int value;
	struct node *next;
} Node;

Node *head;

void insertLL(int value) {
	Node *temp = (Node *) malloc(sizeof(Node));
	temp->value = value;
	temp->next = head;
	head = temp;
}

void insertRear(int value){
	if (head == NULL){
		printf("NULL front\n");
		return;
	}
	Node *ptr = head;
	while (ptr->next != NULL){
		ptr = ptr->next;
	}
	Node *temp = (Node*) malloc(sizeof(Node));
	temp->value = value;
	temp->next = NULL;
	ptr->next = temp;
	return;	
}

int main(int argc, char *argv[]){
	head = (Node*) malloc(sizeof(Node));
	head->value = 5;
	head->next = NULL;
	insertLL(6);
	insertRear(7);
	printf("%d\n", head->value);	
	head = head->next;
	printf("%d\n", head->value);
	head = head->next;
	printf("%d\n", head->value);
	return 0;
}
