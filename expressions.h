#include <stdlib.h>
#include "objects.h"
#include "types.h"

union token {
  int START;
  int LBRAC;
  int LCURL;
  int RBRAC;
  int RCURL;
  int EQUAL;
  int PLUS;
  int MINUS;
  int DIVIDE;
  int MULTIPLY;
  int IF;
  int FOR;
  int FN;
  int RETURN;
  int OBJECT;
};

struct node {
  int id;
  int token;
  t_object* object;
  struct node *next;
  struct node *prev;
};

typedef struct {
  struct node *node;
  union token tokens;
} t_ast;

typedef struct {
  t_ast* ast;
} t_expression;

union token initTokens();
t_ast* parse (char* e);
union generic eval(t_expression *expression);
