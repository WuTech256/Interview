// Find the middle element of singly linked list

/*
    I can find the middle element of a singly linked list using the slow and fast pointer technique
    I use two pointers: slow and fast. The slow pointer movers one node at at time, while the past pointer movers
    two nodes at a time.
    
    When the fast pointer reaches the end of the linked list, the slow pointer will be pointing to the middle node.
    This approach requires only on traversal and does not need extra memory
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

int findMiddle(Node* head) {
    Node* slow = head;
    Node* fast = head;
    
    while(fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    
    return slow->data;
}

void push(Node** head, int data) {
    Node* newNode = malloc(sizeof(Node));
    
    newNode->data = data;
    newNode->next = *head;
    *head = newNode;
}

int main()
{
    Node* head = NULL;
    push(&head, 5);
    push(&head, 4);
    push(&head, 3);
    push(&head, 2);
    push(&head, 1);
    
    printf("Middle element: %d\n", findMiddle(head));
    return 0;
}
