/* all lines before functions are only to explain the background */

#include <stdlib.h>;
#include <pthread.h>;
#include "linkedlist.h";

#define SYS_ERR -1

struct node {
    int data;
    struct node *next;
} node_t;

struct linked_list{
	node *head;
	pthread_mutex_t mutex;
} list; 

list * list_init(){
	list *list_ptr = malloc(sizeof(list));
	list_ptr->head = NULL;
	pthread_mutex_init(&(list_ptr->mutex), NULL);
	return list_ptr;
}

// add a node to end of linked-list
int add(int num){
    node *temp,*right;
	// create temp
    temp = malloc(sizeof(struct node));
    temp->data=num;
	temp->next=NULL;
	// make right the last node
	// Start of Critical Section
	pthread_mutex_lock(&cs_mutex);
    right=head;
    while(right->next != NULL){
		right = right->next;
	}
	// link right to temp
    right->next = temp;
	pthread_mutex_unlock(&cs_mutex);
	return 0;
}

// remove first node
void remove(){
	// Start of Critical Section
	pthread_mutex_lock(&cs_mutex);
	if (head == NULL){
		return;
	}
	head = head->next;
	pthread_mutex_unlock(&cs_mutex);
}