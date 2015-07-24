#include <stdlib.h>
#include "objects.h"
#include "types.h"

union token {
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
}

struct node {
  union token token;
  int id;
  struct node childNode;
  struct node parentNode;
};

typedef struct {
  struct node root;
  union token tokens;
  struct node* branches;
} t_ast;

union token initTokens();
t_ast* parse (char* e);
union generic eval(t_expression *expression);
