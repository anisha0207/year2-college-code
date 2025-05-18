#include "testCase.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>//used for intptr_t: hold pointers, integer type such that any valid (void *)
       //value can be converted to this type and then converted back
#include <ctype.h>//used isDigit(): Checks whether a character is a decimal digit

//Comprehension: Helps to organize the code in terms of test case criterias

//Test 1: H5A5Z5S1X
/* H5: Insert 5 at the head
A5: Assert that the head is 5
Z5: Assert that the tail is 5
S1: Assert that the size is 1
X: End of test */

//Test 2: H5A5Z5S1N0,10A10Z5S2X
/* H5: Insert 5 at the head
A5: Assert that the head is 5
Z5: Assert that the tail is 5
S1: Assert that the size is 1
N0,10: Insert 10 at index 0 (new head)
A10: Assert that the head is now 10
Z5: Assert that the tail is still 5
S2: Assert that the size is now 2
X: End of test*/

//Test 3: H5T10A5Q0,5A5Z10S2X
/*H5: Insert 5 at the head
T10: Insert 10 at the tail
A5: Assert that the head is 5
Q0,5: Assert that the node at index 0 is 5
A5: Assert that the head is 5 (again)
Z10: Assert that the tail is 10
S2: Assert that the size is 2
X: End of test*/

//Test 4: H5H10H15DA10T5S3X
/*H5: Insert 5 at the head
H10: Insert 10 at the head
H15: Insert 15 at the head
D: Remove the head (15)
A10: Assert that the head is now 10
T5: Insert 5 at the tail
S3: Assert that the size is 3
X: End of test*/

//Test 5: H5H10H15CA15T10S3X
/*H5: Insert 5 at the head
H10: Insert 10 at the head
H15: Insert 15 at the head
C: Remove the tail (5)
A15: Assert that the head is 15
T10: Assert that the tail is 10
S3: Assert that the size is 3
X: End of test*/

//Test 6: H5H10H15E1A15Z5S2X
/*H5: Insert 5 at the head
H10: Insert 10 at the head
H15: Insert 15 at the head
E1: Remove node at index 1 (10)
A15: Assert that the head is 15
Z5: Assert that the tail is 5
S2: Assert that the size is 2
X: End of test*/

//Test 7: H5H10A10S2X
/*H5: Insert 5 at the head
H10: Insert 10 at the head
A10: Assert that the head is 10
S2: Assert that the size is 2
X: End of test*/

//Test 8: H5H10Z5A10S2X
/*H5: Insert 5 at the head
H10: Insert 10 at the head
Z5: Assert that the tail is 5
A10: Assert that the head is 10
S2: Assert that the size is 2
X: End of test*/

//Test 9: H5H10H15W10S3X
/*H5: Insert 5 at the head
H10: Insert 10 at the head
H15: Insert 15 at the head
W10: Assert that the list contains 10
S3: Assert that the size is 3
X: End of test*/

//Test 10: H5H10CZ10S1X
/*H5: Insert 5 at the head
H10: Insert 10 at the head
C: Remove the tail (5)
Z10: Assert that the tail is 10
S1: Assert that the size is 1
X: End of test*/

#include "linkedlist.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

 int test(List* list, char* testC) {
    int index = 0;
    while (testC[index] != '\0') {
        // Skip all non-command characters

        while (testC[index] != '\0' && !strchr("HTNDCEAQZSWX", testC[index])) {
            index++;
        }

        if (testC[index] == '\0'){
            break;
        } 

        char command = testC[index];
        index++;// Move past the command character

        int value, node_index;
        //Only parse arguments IF needed
        switch (command) {
            //first block: parses input and gets rid of necessary values
            //(H,T,A,Z,S,W): require singe value
            //(N,Q): require index and value
            //(E): requie index
            //(D,C): with no parameter

            case 'H': case 'T': case 'A': case 'Z': case 'S': case 'W': {
                if (sscanf(&testC[index], "%d", &value) != 1) {
                    printf("Failed to parse value for command %c\n", command);
                    return 1;
                }
                //Advance index to after the parsed number
                while(isdigit(testC[index]) || testC[index] == '-') {//isDigit() is important here to ignore other characters that aren't digits in order to execute
                    index++;//will move on if '-'
                }
                break;
            }
            case 'N': case 'Q': {
                if (sscanf(&testC[index], "%d,%d", &node_index, &value) != 2) {
                    printf("Failed to parse index and value for command %c\n", command);
                    return 1;
                }
                //Advance index to after the parsed numbers, comma, and negative sign
                while(isdigit(testC[index]) || testC[index] == ',' || testC[index] == '-') {
                    index++;//will move on when it reaches to ',' and '-'
                }
                break;
            }
            case 'E': {
                if (sscanf(&testC[index], "%d", &node_index) != 1) {
                    printf("Failed to parse index for command E\n");
                    return 1;
                }
                //Advance index to after the parsed number and negative sign
                while(isdigit(testC[index]) || testC[index] == '-') {
                    index++;//same concept continues
                }
                break;
            }
            case 'X':
                // End of test command
                return 0;  // End of test
                break;
            default:
                break;
        }

        // Execute the command
        switch (command) {
            case 'H'://insert at head
                printf("Inserting %d at head\n", value);
                if (insertAtHead(list, (void*)(intptr_t)value) != 0) {
                    printf("Failed to insert %d at head\n", value);
                    return 1;
                }
                break;
            case 'T'://insert at tail
                printf("Inserting %d at tail\n", value);
                if (insertAtTail(list, (void*)(intptr_t)value) != 0) {
                    printf("Failed to insert %d at tail\n", value);
                    return 1;
                }
                break;
            case 'N'://insert at specific index
                printf("Inserting %d at index %d\n", value, node_index);
                if (insertAtIndex(list, node_index, (void*)(intptr_t)value) != 0) {
                    printf("Failed to insert %d at index %d (out of bounds?)\n", value, node_index);
                    return 1;
                }
                break;
            case 'D'://remove head
                printf("Removing head\n");
                if (removeHead(list) == NULL) {
                    printf("Failed to remove head (list empty?)\n");
                    return 1;
                }
                break;
            case 'C'://remove tail
                printf("Removing tail\n");
                if (removeTail(list) == NULL) {
                    printf("Failed to remove tail (list empty?)\n");
                    return 1;
                }
                break;
            case 'E'://remove at specific index
                printf("Removing node at index %d\n", node_index);
                if (removeAtIndex(list, node_index) == NULL) {
                    printf("Failed to remove node at index %d\n", node_index);
                    return 1;
                }
                break;
            case 'A'://assert head value
                printf("Asserting head value is %d\n", value);
                if (list->head == NULL || (intptr_t)(list->head->item) != value) {
                    printf("Assertion failed: Head value is not %d\n", value);
                    return 1;
                }
                break;
            case 'Z'://assert tail value
                printf("Asserting tail value is %d\n", value);
                if (list->tail == NULL || (intptr_t)(list->tail->item) != value) {
                    printf("Assertion failed: Tail value is not %d\n", value);
                    return 1;
                }
                break;
            case 'Q'://assert value at specific index
                printf("Asserting value at index %d is %d\n", node_index, value);
                if ((intptr_t)itemAtIndex(list, node_index) != value) {
                    printf("Assertion failed: Value at index %d is not %d\n", node_index, value);
                    return 1;
                }
                break;
            case 'S'://size
                printf("Asserting size is %d\n", value);
                if (size(list) != value) {
                    printf("Assertion failed: Size is not %d (actual size: %d)\n", value, size(list));
                    return 1;
                }
                break;
            case 'W'://assert list contains value
                printf("Asserting list contains %d\n", value); 
                if (!contains(list, (void*)(intptr_t)value)) {
                    printf("List does not contain %d\n", value);
                    return 1;
                }
                break;
            case 'X':
                printf("End of test.\n");
                return 0;
            default:
                printf("Unknown command: %c\n", command);
                return 1;
        }

        // Move to next potential command
        while (testC[index] != '\0' && !strchr("HTNDCEAQZSWX", testC[index])) {
            index++;
        }
    }
    return 0;//test passed
}

