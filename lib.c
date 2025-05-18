#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> //we are using a boolean type for contains()
#include "linkedlist.h" //*: declare to define pointer variable which stores memory address of variable of specific type
//also dereferences pointer which gives access to the value stored at memory address

// Initialize an empty list
void initList(List* list_pointer) { //parameter needed to be used to set the list that are in head and tail as NULL
    list_pointer->head = NULL;
    list_pointer->tail = NULL;
}

// Create node containing item, return reference of it.
Node* createNode(void* item) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    //allocate memory for the new node
    if (!newNode) {
        return NULL;
    }
    newNode->item = item;
    newNode->next = NULL;
    return newNode;
}

// Insert new item at the end of list.
int insertAtTail(List* list_pointer, void* item) {
    //same code as insertAtHead() with few minor details
    Node* newNode = createNode(item);
    if (!newNode) {
        return 1;
    }
    //if list empty, both head and tail set to newNode
    if (!list_pointer->tail) {
        list_pointer->head = newNode; //adds new node to the head
        list_pointer->tail = newNode; //adds new node to the tail
        // updates the next of the current tail to point to the new node.
    } else {
        list_pointer->tail->next = newNode;
        list_pointer->tail = newNode;
    }
    return 0;
}

// Insert item at start of the list.
int insertAtHead(List* list_pointer, void* item) {
    // Node* newNode = createNode(item);
    // newNode-> next = list_pointer;
    // list_pointer = newNode;

    //code above does not work through debugging due to list_pointer does not correctly access the head of the list
    //Must make sure if createNode returns NULL and handles error
    //need to be able to return 0 or 1 as success or failure
    Node* newNode = createNode(item);
    if (newNode == NULL) {
        return 1;
    }
    //adds new node to the head
    newNode->next = list_pointer->head;
    list_pointer->head = newNode;
    //if list empty, updates tail to newNode
    if (list_pointer->tail == NULL) {
        list_pointer->tail = newNode;
    }
    return 0;
}

// Insert item at a specified index.
int insertAtIndex(List* list_pointer, int index, void* item) {
    if (list_pointer == NULL || index < 0) {
        return 1;
    }
    Node* newNode = createNode(item);
    if (newNode == NULL) {
        return 1; // Memory allocation failure
    }
    if (index == 0) {
        return insertAtHead(list_pointer, item); //this is similar to when starting off the list with empty--adding the nodes will start from the head
    }
    Node* current = list_pointer->head;
    for (int i = 0; i < index - 1 && current != NULL; i++) {
        current = current->next;
    }

    if (current == NULL) {
        free(newNode); // Index out of bounds, free the allocated node
        return 1;       // Indicate failure
    }

    newNode->next = current->next;
    current->next = newNode;

    if (newNode->next == NULL) {
        list_pointer->tail = newNode;
    }
    //If the new node is at the end of the list, we update the tail pointer.
    return 0;
}

// Remove item from the end of list and return a reference to it
void* removeTail(List* list_pointer) {
    if (list_pointer->tail == NULL) {
        return NULL; //if the node is null at the tail end-- return NULL
    }
    if (list_pointer->head == list_pointer->tail) { //If head and tail are the same (only one node):Frees the node and sets both head and tail to NULL.
        void* item = (void*)list_pointer->tail->item; //Stores the item from the tail node to return later
        free(list_pointer->head);
        list_pointer->head = NULL;
        list_pointer->tail = NULL;
        return item;
    } //Traverses the list to find the second-to-last node. Frees the tail node. Updates the second-to-last node to be the new tail.
    Node* current = list_pointer->head;
    while (current->next && current->next != list_pointer->tail) { //it originally crashed, must add current->next twice since it might have been seen as NULL
        current = current->next;
    }
    void* item = (void*)list_pointer->tail->item;
    free(list_pointer->tail);
    list_pointer->tail = current;
    current->next = NULL;
    return item; //returns stored item
}

// Remove item from start of list and return a reference to it
void* removeHead(List* list_pointer) { //sample sources simply placed head = head-> next (not enough)
    if (list_pointer->head == NULL) { //same concept as tail, but instead with head
        return NULL;
    }
    void* item = (void*)list_pointer->head->item; //same way it is stored as tail with the item to be returned later
    Node* prevHead = list_pointer->head; //Saves the current head node to free it later.
    list_pointer->head = list_pointer->head->next; //Updates the head to point to the next node.
    free(prevHead); //Frees the old head node.
    if (list_pointer->head == NULL) { //If the list is now empty (head is NULL), sets tail to NULL as well.
        list_pointer->tail = NULL;
    }
    return item; //returns stored item
}

// Insert item at a specified index and return a reference to it
void* removeAtIndex(List* list_pointer, int index) {
    // Check if the list is NULL, the index is invalid (< 0), or the list is empty
    if (list_pointer == NULL || index < 0 || list_pointer->head == NULL) {
        return NULL;  // Return NULL if any of these conditions are true
    }

    // If the index is 0, use the removeHead() function to remove the first node
    if (index == 0) {
        return removeHead(list_pointer);  // Special case: removing the head of the list
    }

    /* Traverse the list to find the node before the one to be removed.
       The loop runs until it reaches the node just before the specified index.
       If the index is out of bounds (i.e., current->next becomes NULL before reaching index - 1),
       the function returns NULL. */
    Node* current = list_pointer->head;
    for (int i = 0; i < index - 1; i++) {
        if (current->next == NULL) {  // Check if we've reached the end of the list prematurely
            return NULL;  // Index is out of bounds, return NULL
        }
        current = current->next;  // Move to the next node in the list
    }

    // After traversing, check if current->next is NULL, which means index is out of bounds
    if (current->next == NULL) {
        return NULL;  // Index is invalid, return NULL
    }

    /* At this point, current points to the node just before the one to be removed.
       Save a pointer to the node that needs to be removed. */
    Node* nodeToRemove = current->next;

    /* Save a pointer to the item stored in the node being removed.
       This will be returned after freeing the node. */
    void* item = (void*)nodeToRemove->item;

    /* Update the next pointer of 'current' to skip over 'nodeToRemove',
       effectively removing it from the linked list. */
    current->next = nodeToRemove->next;

    // If we are removing the tail node, update the tail pointer of the list
    if (nodeToRemove == list_pointer->tail) {
        list_pointer->tail = current;  // Update tail to point to 'current'
    }

    // Free the memory allocated for 'nodeToRemove'
    free(nodeToRemove);

    // Return the item stored in the removed node
    return item;
}

// Return item at index
void* itemAtIndex(List* list_pointer, int index) {
    if (list_pointer == NULL || index < 0) { //This checks if the list pointer is NULL (invalid list) or if the index is negative.
        return NULL;
    }

    Node* current = list_pointer->head; //creates a current and sets it to the head of the list.
    for (int i = 0; i < index && current != NULL; i++) { //loops through the list until we've moved index number of times and we reach the end of the list where current becomes NULL
        current = current->next;
    }

    if (current != NULL) { //if current not NULL, found node at the index adn return that item
        return current->item;
    } else {
        return NULL;
    }
}

// Return true if the list contains the given item at least once, false otherwise.
bool contains(List* list_pointer, void* item) {
    if (list_pointer == NULL || list_pointer->head == NULL) { //if list does not exist or is empty, returns false (boolean)
        return false;
    }

    Node* current = list_pointer->head; //current is set as the head for the list_pointer
    while (current != NULL) { //go through the list when it's not a NULL
        if (current->item == item) { //checks if the item in the current node matches the item 
            return true;
        }
        current = current->next; //If the current item doesn't match, we move current to point to the next node in the list
    }

    return false; //gone through the list with no match
}

// Returns the size of the list, measured in nodes.
int size(List* list_pointer) {
    int count = 0;
    if (list_pointer == NULL) { //checks for empty list 
        return 0;
    }
    Node* current = list_pointer->head;
    while (current != NULL) { //counting nodes through the entire list
        count++;
        current = current->next;
    }

    return count; //total number of nodes
}

//helper method for main.c when freeing the list
void freeList(List* list_pointer) {
    Node* current = list_pointer->head;
    Node* next = NULL;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    list_pointer->head = NULL;
    list_pointer->tail = NULL;
}

void printList(List* list) {
    Node* node;

    // Handle an empty node. Just print a message.
    if (list->head == NULL) {
        printf("\nEmpty List");
        return;
    }

    // Start with the head.
    node = (Node*) list->head;

    printf("\nList: \n\n\t");
    while (node != NULL) {
        printf("[ %p ]", node->item);

        // Move to the next node
        node = (Node*) node->next;

        if (node != NULL) {
            printf("-->");
        }
    }
    printf("\n\n");
}
