#ifndef __EXPRESSIONS_H
#define __EXPRESSIONS_H

#include <stdlib.h>
#include "types.h"
#include "objects.h"

typedef struct {
  int START;
  int END;
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

  int IMPORT;

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

// union token initTokens();
t_object* parse(char* e);
t_object* eval(t_list* ast);
#endif
