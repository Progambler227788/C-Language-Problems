#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

//////////////////inked list ///////////

typedef struct _listnode{
  int item;
  struct _listnode *next;
} ListNode;

typedef struct _linkedlist{
  int size;
  ListNode *head;
  ListNode *tail;
} LinkedList;

///////////////// stack    /////////////////

typedef struct stack{
   LinkedList ll;
} Stack;

/////////////// queue /////////////////////

typedef struct _queue{
   LinkedList ll;
} Queue;

////////// function prototypes /////////////

// You should not change the prototypes of these functions

void interleaveQueue(Queue *q);

////////////////////////////////////////////

void push(Stack *s, int item);
int pop(Stack *s);
int peek(Stack *s);
int isEmptyStack(Stack *s);

void enqueue(Queue *q, int item);
int dequeue(Queue *q);
int isEmptyQueue(Queue *s);

///////////////////////////////////////////////////////
void printList(LinkedList *ll);
ListNode * findNode(LinkedList *ll, int index);
int insertNode(LinkedList *ll, int index, int value);
int removeNode(LinkedList *ll, int index);
void removeAllItems(LinkedList *ll);

////////////////////////////////////////////////////////


int main()
{
   int c, value;

   Queue q;

   //initialize the queue
   q.ll.head =NULL;
   q.ll.tail =NULL;
   q.ll.size =0;


   c =1;

   printf("1: Insert an integer into the queue;\n");
   printf("2: Interleaving the first half of the queue with the second half of the queue;\n");
   printf("0: Quit;\n");

   while (c != 0)
   {
       printf("Please input your choice(1/2/0): ");
       scanf("%d", &c);

       switch (c)
       {
       case 1:
           printf("Input an integer that you want to insert into the queue: ");
           scanf("%d", &value);
           enqueue(&q, value);
           printf("The resulting queue is: ");
           printList(&(q.ll));
           break;
       case 2:
           interleaveQueue(&q); // You need to code this function
           printf("The resulting queue after interleaving the first half of the queue with the second half of the queue is: ");
           printList(&(q.ll));
           removeAllItems(&(q.ll));
           break;
       case 0:
           removeAllItems(&(q.ll));
           break;
       default:
           printf("Choice unknown;\n");
           break;
       }
   }

   return 0;
}

////////////////////////////////////////////////////////////
void interleaveQueue(Queue *q) {
    int half_size = q->ll.size / 2;
    Stack s;
    s.ll.head = NULL;
    s.ll.tail = NULL;
    s.ll.size = 0;

  
    // Push first half elements into the stack
    for (int i = 0; i < half_size; i++) {
        int item = dequeue(q);
       
        push(&s, item);
    }
   
    
    // Pop top elements of stack till its empty stage and then enque them in que
    while (!isEmptyStack(&s) ) {
    int stack_item = pop(&s);
    enqueue(q, stack_item);
}
 

    // dequeue first que elements and enqueue the first half elements of queue at end
    for (int i = 0; i < half_size; i++) {
        int que_item = dequeue(q);
        enqueue(q, que_item);
    }

   
    // Again push the first half elements into the stack
    for (int i = 0; i < half_size; i++) {
        int item = dequeue(q);
        push(&s, item);
    }

    
    // Interleave the elements of queue and stack
    while (!isEmptyStack(&s) ) {
        int stack_item = pop(&s);
        enqueue(q, stack_item);
        int queue_item = dequeue(q);
      
        enqueue(q, queue_item);
    }
}



//////////////////////////////////////////////////////////

void push(Stack *s, int item){
  insertNode(&(s->ll), 0, item);
}

int pop(Stack *s){
  int item;
  if(!isEmptyStack(s)){
   item = ((s->ll).head)->item;
   removeNode(&(s->ll), 0);
   return item;
  }
   return INT_MIN;
}

int peek(Stack *s){
  return ((s->ll).head)->item;
}

int isEmptyStack(Stack *s){
  if ((s->ll).size == 0)
     return 1;
  return 0;
}

void enqueue(Queue *q, int item){
  insertNode(&(q->ll), q->ll.size, item);
}

int dequeue(Queue *q){
  int item;
  item = ((q->ll).head)->item;
  removeNode(&(q->ll), 0);
  return item;
}

int isEmptyQueue(Queue *q){
  if ((q->ll).size == 0)
     return 1;
  return 0;
}

/////////////////////////////////////////////////////

void printList(LinkedList *ll){

   ListNode *cur;
   if (ll == NULL)
       return;
   cur = ll->head;

   if (cur == NULL)
       printf("Empty");
   while (cur != NULL)
   {
       printf("%d ", cur->item);
       cur = cur->next;
   }
   printf("\n");
}

ListNode * findNode(LinkedList *ll, int index){

   ListNode *temp;

   if (ll == NULL || index < 0 || index >= ll->size)
       return NULL;

   temp = ll->head;

   if (temp == NULL || index < 0)
       return NULL;

   while (index > 0){
       temp = temp->next;
       if (temp == NULL)
           return NULL;
       index--;
   }

   return temp;
}

int insertNode(LinkedList *ll, int index, int value){

   ListNode *pre, *cur;

   if (ll == NULL || index < 0 || index > ll->size + 1)
       return -1;

   // If empty list or inserting first node, need to update head pointer
   if (ll->head == NULL || index == 0){
       cur = ll->head;
       ll->head = malloc(sizeof(ListNode));
       ll->head->item = value;
       ll->head->next = cur;
       ll->size++;
       return 0;
   }


   // Find the nodes before and at the target position
   // Create a new node and reconnect the links
   if ((pre = findNode(ll, index - 1)) != NULL){
       cur = pre->next;
       pre->next = malloc(sizeof(ListNode));
       pre->next->item = value;
       pre->next->next = cur;
       ll->size++;
       return 0;
   }

   return -1;
}


int removeNode(LinkedList *ll, int index){

   ListNode *pre, *cur;

   // Highest index we can remove is size-1
   if (ll == NULL || index < 0 || index >= ll->size)
       return -1;

   // If removing first node, need to update head pointer
   if (index == 0){
       cur = ll->head->next;
       free(ll->head);
       ll->head = cur;
       ll->size--;

       return 0;
   }

   // Find the nodes before and after the target position
   // Free the target node and reconnect the links
   if ((pre = findNode(ll, index - 1)) != NULL){

       if (pre->next == NULL)
           return -1;

       cur = pre->next;
       pre->next = cur->next;
       free(cur);
       ll->size--;
       return 0;
   }

   return -1;
}

void removeAllItems(LinkedList *ll)
{
   ListNode *cur = ll->head;
   ListNode *tmp;

   while (cur != NULL){
       tmp = cur->next;
       free(cur);
       cur = tmp;
   }
   ll->head = NULL;
   ll->size = 0;
}

