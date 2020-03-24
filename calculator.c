#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {FALSE, TRUE} BOOL;
typedef enum {CHAR, INT} TYPE;

typedef struct node
{
  void * token;
  struct node * next;
} Node;

typedef struct container
{
  Node *head;
  Node *tail;
  TYPE token_type;
} Container;

Container create_container(TYPE token_type)
{
  Container object;
  object.head = object.tail = NULL;
  object.token_type = token_type;
  return object;
}

void print_container(Container object)
{
  Node *temp = object.head;
  for(; temp; temp=temp->next)
    printf("%s ", temp->token);
  printf("\n");
}

char * get_top(Container object)
{
  return object.head->token;
}

int is_empty(Container object)
{
  return (object.head == NULL);
}

void push_head(Container *object, void * token)
{
  Node *node = (Node *)malloc(sizeof(Node));
  node->token = token;
  node->next = object->head;
  object->head = node;
  if(object->tail == NULL)
    object->tail = object->head;
}

void push_tail(Container *object, void * token)
{
  Node *node = (Node *)malloc(sizeof(Node));
  node->token = token;
  node->next = NULL;
  if (object->tail == NULL)
    object->head = node;
  else
    object->tail->next = node;
  object->tail = node;
}

void * pop_head(Container *object)
{
  if (!object || object->head == NULL)
    return NULL;
  void *token = object->head->token;
  free(object->head);
  object->head = object->head->next;
  if (object->head == NULL)
    object->tail = NULL;
  return token;
}

void * pop_tail(Container *object)
{
  if (!object || object->head == NULL)
    return NULL;
  void *token = object->head->token;

  Node *temp = object->head;    
  while(temp->next != object->tail || temp->next != NULL)
    temp = temp->next;
    
  free(object->tail);
  
  if(object->head == object->tail)
    object->head = object->tail = NULL;
  else
    object->tail = temp;
  return token;
}

int myatoi(char *str)
{
  int i = atoi(str);
  if (str[0] != '0' && i == 0)
    return -1;
  return i;
}

char** tokenizer(char *str, int *len)
{
  char *delim = " ", *token = "", **tokens;
  char *string = (char *)malloc(strlen(str) + 1);
  strcpy(string, str);

  int i;
  *len = 1;
  for(i=0; i<strlen(str); i++)
    if (str[i] == *delim)
      (*len)++;

  tokens = (char **)malloc((*len) * sizeof(char *));
  for (str=string, i=0; token && i < *len; str=NULL, i++)
  {
    token = strtok(str, delim);
    tokens[i] = (char *)malloc(strlen(token)+1);
    strcpy(tokens[i], token);
  }

  return tokens;
}

int is_op (char *str)
{
  char op = str[0];
  return (op == '+' || op == '-' || op == '*' || op == '/' || op == '^');
}

int prec(char * token) 
{ 
  switch (*token) 
  { 
    case '+': 
    case '-': 
        return 1; 
  
    case '*': 
    case '/': 
        return 2; 
  
    case '^': 
        return 3; 
  } 
  return -1; 
} 

int binary_eval(int x, int y, char *op)
{
  switch(*op)
  {
    case '+': return x + y;
    case '-': return x - y;
    case '*': return x * y;
    case '/': return x / y;
    default :
    {
      printf("ERROR: %s - Invalid binary operator\n", op);
      exit(-1);
    }
  }
}

Container infix_to_postfix(char *str)
{  
  int len;
  printf ("INFIX EXP: %s\n", str);
  char **tokens = tokenizer(str, &len);

  Container op_stack = create_container(CHAR);
  Container re_queue = create_container(CHAR);
  
  for(int i=0; i<len; i++)
  {
    if(myatoi(tokens[i]) != -1)
    {
      push_tail(&re_queue, tokens[i]);
    }
    else if (*tokens[i] == '(')
    {
      push_head(&op_stack, tokens[i]);
    }
    else if (*tokens[i] == ')')
    {
      while (!is_empty(op_stack) && *get_top(op_stack) != '(')
      {
        push_tail(&re_queue, pop_head(&op_stack));
      }
      if (is_empty(op_stack))
      {
        printf("ERROR: No matcing paranthesis for ) \n");
        exit (-1);
      } 
      pop_head(&op_stack);
    }
    else if (is_op(tokens[i]))
    {
      while (!is_empty(op_stack) && prec(tokens[i]) <= prec(get_top(op_stack)))
      {
        push_tail(&re_queue, pop_head(&op_stack));
      }
      push_head(&op_stack, tokens[i]);      
    }
  }
  while (!is_empty(op_stack))
  {
    if (*get_top(op_stack) == '(')
    {
      printf("ERROR: No matcing paranthesis for (\n");
      exit(-1);
    }
    push_tail(&re_queue, pop_head(&op_stack));
  }
  printf("POSTFIX EXP: ");
  print_container(re_queue);
  return re_queue;
}

int evaluvate(Container op_queue)
{
  Container op_values = create_container(INT);
  Node * node = op_queue.head;
  int token, *operand, *x_op, *y_op;

  for (; node; node=node->next)
  {
    token = myatoi(node->token);
    if(token != -1)
    {
      operand = (int *)malloc(sizeof(int));
      *operand = token;
      push_head(&op_values, operand);
    }
    else
    {
      y_op = pop_head(&op_values);
      x_op = pop_head(&op_values);
      operand = (int *)malloc(sizeof(int));
      *operand = binary_eval(*x_op, *y_op, node->token);
      push_head(&op_values, operand);
      free(x_op);
      free(y_op);
    }
  }
  operand = pop_head(&op_values);
  token = *operand;
  free(operand);
  return token;
}

int main(void)
{
  char *str = "2 + 3 * ( 4 + ( 2 - 7 ) * 5 )";
  Container op_queue = infix_to_postfix(str);
  int result = evaluvate(op_queue);
  printf("RESULT: %d\n", result);
  return 0;
}