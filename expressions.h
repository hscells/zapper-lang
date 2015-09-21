#include <stdlib.h>
#include "objects.h"
#include "types.h"

typedef struct {
  int START;
  int LBRAC;
  int LCURL;
  int RBRAC;
  int RCURL;
  int IF;
  int FOR;
  int FN;
  int RETURN;
  int OBJECT;
  int INT;
  int FLOAT;
  int CHAR;
  int STRING;
  int LIST;
  int BOOL;
  int COMMA;
  int DOT;
  int CLASS;
  int NEW;
  int SYMBOL;
} t_token;

typedef struct {
  int ADD;
  int SUB;
  int MUL;
  int DIV;
  int EQ;
  int LT;
  int GT;
  int LTEQ;
  int GTEQ;
  int PRINT;
  int PRINTLN;
  int READ;
  int EXCEPT;
  int EXIT;

  int LIST;
  int CONJ;
  int FIRST;
  int REST;
  int LENGTH;

} t_system;

// the node structure is a linked list
struct node {
  int id;
  int token;
  t_object* object;
  struct node *next;
  struct node *prev; // prev actually isn't used
  int line_num;
};

typedef struct {
  struct node *node;
  struct node *head;
  t_token* tokens;
  t_system* system;
  int line_count;
  t_stack* stack;
} t_ast;

typedef struct {
  t_ast* ast;
} t_expression;

// union token initTokens();
t_ast* parse(char* e, t_stack* stack, t_heap* heap, int index);
t_ast* newAst();
struct node* newNode();
struct node* newTokenNode(int token, t_ast* ast);
struct node* newObjectNode(t_object* object, t_ast* ast);
t_object* eval(t_ast *ast);
void collect_ast(t_ast *ast);
