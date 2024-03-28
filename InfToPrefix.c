#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h> // Include limits.h for INT_MIN

#define SIZE 1000 // The limit of expression length
/////////////////////////////////////////////////

typedef struct _listnode{
  char item;
  struct _listnode *next;
} ListNode;

typedef struct _linkedlist{
  int size;
  ListNode *head;
} LinkedList;

////////////////////////////////// stack  ///////////////////////

typedef struct stack{
   LinkedList ll;
} Stack;

//////////////////////// function prototypes /////////////////////////////

void infixtoPrefix(char* infix, char* prefix); // You should not change the prototypes of these functions

int precedence(char op); // You may use this utility function or rewrite your own precedence () function

////////////////////////////////////////////////////////////////////////////
void push(Stack *s, char item);
int pop(Stack *s);
int peek(Stack *s);
int isEmptyStack(Stack *s);

////////////////////////////////////////////////////////////////////////////
void printList(LinkedList *ll);
ListNode * findNode(LinkedList *ll, int index);
int insertNode(LinkedList *ll, int index, char item);
int removeNode(LinkedList *ll, int index);
void removeAllItems(LinkedList *ll);

////////////////////////////////////////////////////////////////////////////

int main()
{
   char infix[SIZE];
   char prefix[SIZE];

   printf("Enter an infix expression:\n");
   scanf("%[^\n]%*c",infix);

   infixtoPrefix(infix, prefix);
   printf("The prefix expression is \n");
   printf("%s\n",prefix);
   return 0;
}

void infixtoPrefix(char* infix, char* prefix)
{
  //add your code here
  // Initialize a stack for operators
    Stack s;
    s.ll.head = NULL;
    s.ll.size = 0;

    // Initialize the prefix expression
  

    // Get the length of the infix expression
    int len = strlen(infix);
    
    // reverse infix string
    for (int i = 0; i < len / 2; i++) {
        char temp = infix[i];
        infix[i] = infix[len - 1 - i];
        infix[len - 1 - i] = temp;
    }
    // exchange character ( with ) in infix
     for (int i = 0; i < len; i++) {
 
        if (infix[i] == '(') {
            infix[i] = ')';
        }
        else if (infix[i] == ')') {
            infix[i] = '(';
        }
    }
    
    
    // variables to be used in loop
    int j = 0, i;
    // Scan all characters of the infix expression
    for (i = 0; infix[i]; ++i) {
        // If the character is an operand, add it to the postfix expression
        if ((infix[i] >= 'a' && infix[i] <= 'z') || (infix[i] >= 'A' && infix[i] <= 'Z') || (infix[i] >= '0' && infix[i] <= '9')) {
       
            prefix[j++] = infix[i];
         
          
       
        }
        // If the character is an operator
        else if (infix[i] == '+' || infix[i] == '-' || infix[i] == '*' || infix[i] == '/') {
            // Pop operators with higher or equal precedence from the stack and add them to the postfix expression
            while (!isEmptyStack(&s) && precedence(infix[i])  < precedence(peek(&s)) &&  peek(&s) != '(') {
                prefix[j++] = pop(&s);
            }
            // Push the current operator onto the stack
            push(&s, infix[i]);
            
        }
        // If the character is a left parenthesis, push it onto the stack
        else if (infix[i] == '(') {
            push(&s, infix[i]);
            
        }
        // If the character is a right parenthesis, pop operators from the stack and add them to the postfix expression until a left parenthesis is encountered
        else if (infix[i] == ')') {
            while (!isEmptyStack(&s) && peek(&s) != '(') {
                prefix[j++] = pop(&s);
              
            }
            // Pop the left parenthesis from the stack
            pop(&s);
           
        }
    }

  



    // Pop any remaining operators from the stack and add them to the postfix expression
    while (!isEmptyStack(&s)) {
        prefix[j++] = pop(&s);
       
    }
    

    // Null-terminate the postfix expression
    prefix[j] = '\0';
    
    len = strlen(prefix);
    for (int i = 0; i < len / 2; i++) {
        char temp = prefix[i];
        prefix[i] = prefix[len - 1 - i];
        prefix[len - 1 - i] = temp;
    }
}


int precedence(char op) {
   switch (op) {
       case '+':
       case '-':
           return 1;
       case '*':
       case '/':
           return 2;
       default:
           return -1;
   }
}
/////////////////////////////////////////////////////////////////////

void push(Stack *s, char item){
  insertNode(&(s->ll), 0, item);
}

int pop(Stack *s){
  char item;
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

////////////////////////////////////////////////////////////////

void printList(LinkedList *ll){

   ListNode *cur;
   if (ll == NULL)
       return;
   cur = ll->head;

   if (cur == NULL)
       printf("Empty");
   while (cur != NULL)
   {
       printf("%c ", cur->item);
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

int insertNode(LinkedList *ll, int index, char value){

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


