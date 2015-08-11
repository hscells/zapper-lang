#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "expressions.h"
#include "system.h"
#include "types.h"

int node_id = 0;

t_token* initTokens() {
  t_token* tokens = (t_token*) malloc(sizeof(t_token));
  tokens->START  = 0x0000;
  tokens->LBRAC  = 0x0001;
  tokens->RBRAC  = 0x0002;
  tokens->LCURL  = 0x0003;
  tokens->RCURL  = 0x0004;
  tokens->IF     = 0x0005;
  tokens->FOR    = 0x0006;
  tokens->FN     = 0x0007;
  tokens->RETURN = 0x0008;
  tokens->OBJECT = 0x0009;
  tokens->INT    = 0x0010;
  tokens->FLOAT  = 0x0011;
  tokens->CHAR   = 0x0012;
  tokens->STRING = 0x0013;
  tokens->LIST   = 0x0014;
  tokens->COMMA  = 0x0015;
  tokens->DOT    = 0x0016;
  tokens->CLASS  = 0x0017;
  tokens->NEW    = 0x0018;
  return tokens;
}

t_system* initSystem() {
  t_system* system = (t_system*) malloc(sizeof(t_system));
  system->ADD    = 0xa000;
  system->SUB    = 0xa001;
  system->MUL    = 0xa002;
  system->DIV    = 0xa003;
  system->EQ     = 0xa004;
  system->LT     = 0xa005;
  system->GT     = 0xa006;
  system->LTEQ   = 0xa007;
  system->GTEQ   = 0xa008;
  system->PRINT  = 0xa009;
  system->READ   = 0xa010;
  system->EXCEPT = 0xa011;
  return system;
}

t_ast* newAst () {
  t_token* tokens = initTokens();
  printf("%d\n", tokens->START);
  t_system* system = initSystem();
  t_ast* ast = (t_ast*) malloc(sizeof(t_ast));
  ast->tokens = tokens;
  ast->system = system;
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
  node->token = ast->tokens->RBRAC;
  node->object = object;
  return node;
}

t_ast* parse(char* e, int index) {
  int i;
  char c;
  char* tok;
  t_ast* ast = newAst();
  ast->node = newTokenNode(ast->tokens->START, ast);
  struct node* starting_node = ast->node;

  for(i = index; i < strlen(e) - 1; i++) {
    c = e[i];
    if (c == '(') {
      ast->node->next = newTokenNode(ast->tokens->LBRAC, ast);
    } else if (c == ')') {
      ast->node->next = newTokenNode(ast->tokens->RBRAC, ast);
    } else if (c == '{') {
      ast->node->next = newTokenNode(ast->tokens->LCURL, ast);
    } else if (c == '}') {
      ast->node->next = newTokenNode(ast->tokens->RCURL, ast);
    } else {
      while (c != '(' && c != ')' && c != '{' && c != '}') {
        tok = strcat(tok, (char*)c);
        i++;
      }
      z_exception("Invalid token",0,i);
    }
  }

  ast->node = starting_node;

  return ast;
}

union generic eval(t_expression *expression) {
  union generic value;
  printf("%d\n", expression->ast->node->id);
  return value;
}
