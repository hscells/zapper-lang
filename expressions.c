#include <stdio.h>
#include <strings.h>
#include "expressions.h"

int node_id = 0;

union token initTokens() {
  union token tokens;
  tokens.START = -1;
  tokens.LBRAC = 0;
  tokens.RBRAC = 1;
  tokens.LCURL = 2;
  tokens.RCURL = 3;
  tokens.EQUAL = 4;
  tokens.PLUS = 5;
  tokens.MINUS = 6;
  tokens.DIVIDE = 7;
  tokens.MULTIPLY = 8;
  tokens.IF = 9;
  tokens.FOR = 10;
  tokens.FN = 11;
  tokens.RETURN = 12;
  return tokens;
}

t_ast* newAst () {
  union token tokens = initTokens();
  t_ast* ast = (t_ast*) malloc(sizeof(t_ast));
  ast->tokens = tokens;

  node_id++;
  return ast;
}

struct node* newNode() {
  struct node* node = (struct node*) malloc(sizeof(struct node));
  node->id = node_id++;
  return node;
}

struct node* newTokenNode(int token, t_ast* ast) {
  struct node* node = newNode();
  node->token = token;
  return node;
}

struct node* newObjectNode(t_object* object, t_ast* ast) {
  struct node* node = newNode();
  node->token = ast->tokens.OBJECT;
  node->object = object;
  return node;
}

t_ast* parse(char* e) {
  int i;
  char c;
  // char* tok;
  t_ast* ast = newAst();
  ast->node = newTokenNode(ast->tokens.START, ast);
  struct node* starting_node = ast->node;

  for(i = 0; i < strlen(e); i++) {
    c = e[i];
    if (c == '(') {
      ast->node->next = newTokenNode(ast->tokens.LBRAC, ast);
    } else if (c == ')') {
      ast->node->next = newTokenNode(ast->tokens.RBRAC, ast);
    }
  }

  ast->node = starting_node;

  return ast;
}

union generic eval(t_expression *expression) {
  union generic value;
  printf("%d\n", expression->ast->node->token);
  return value;
}
