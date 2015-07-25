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
} t_token;

typedef struct {
  int ADD;
  int SUB;
  int MUL;
  int DIV;
  int EQ;
  int LT;
  int GT;
  int PRINT;
  int EXCEPT;
} t_system;

struct node {
  int id;
  int token;
  t_object* object;
  struct node *next;
  struct node *prev;
};

typedef struct {
  struct node *node;
  t_token* tokens;
  t_system* system;
} t_ast;

typedef struct {
  t_ast* ast;
} t_expression;

// union token initTokens();
t_ast* parse (char* e, int index);
union generic eval(t_expression *expression);
