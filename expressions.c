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
  tokens->SYMBOL = 0x0019;
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
  // printf("%d\n", tokens->START);
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
  char tok[255];
  t_ast* ast = newAst();
  ast->node = newTokenNode(ast->tokens->START, ast);
  struct node* starting_node = ast->node;

  for(i = index; i < strlen(e) - 1; i++) {
    memset(tok,0,sizeof(tok));
    c = e[i];

    // add individual tokens to the ast
    if (c == '(') {
      ast->node->next = newTokenNode(ast->tokens->LBRAC, ast);
      ast->node = ast->node->next;
      c = e[i++];
    }

    if (c == ')') {
      ast->node->next = newTokenNode(ast->tokens->RBRAC, ast);
      ast->node = ast->node->next;
      c = e[i++];
    }

    if (c == '{') {
      ast->node->next = newTokenNode(ast->tokens->LCURL, ast);
      ast->node = ast->node->next;
      c = e[i++];
    }

    if (c == '}') {
      ast->node->next = newTokenNode(ast->tokens->RCURL, ast);
      ast->node = ast->node->next;
      c = e[i++];
    }

    if (c == '.') {
      ast->node->next = newTokenNode(ast->tokens->DOT, ast);
      ast->node = ast->node->next;
      c = e[i++];
    }

    if (c == ',') {
      ast->node->next = newTokenNode(ast->tokens->COMMA, ast);
      ast->node = ast->node->next;
      c = e[i++];
    }

    // add tokens which are >1 character in length
    while (c != '(' && i < strlen(e)) {
      strcat(tok, &c);
      i++;
      c = e[i];
    }
    if (strlen(tok) > 0) {
      // printf("\ni:%d - %s (%lu)\n",i,tok,strlen(tok));
      if (strcmp(tok,"if") == 0) {
        ast->node->next = newTokenNode(ast->tokens->IF, ast);
        ast->node = ast->node->next;
      } else if (strcmp(tok, "for") == 0) {
        ast->node->next = newTokenNode(ast->tokens->FOR, ast);
        ast->node = ast->node->next;
      } else if (strcmp(tok, "fn") == 0) {
        ast->node->next = newTokenNode(ast->tokens->FN, ast);
        ast->node = ast->node->next;
      } else if (strcmp(tok, "return") == 0) {
        ast->node->next = newTokenNode(ast->tokens->RETURN, ast);
        ast->node = ast->node->next;
      } else if (strcmp(tok, "int") == 0) {
        ast->node->next = newTokenNode(ast->tokens->INT, ast);
        ast->node = ast->node->next;
      } else if (strcmp(tok, "float") == 0) {
        ast->node->next = newTokenNode(ast->tokens->FLOAT, ast);
        ast->node = ast->node->next;
      } else if (strcmp(tok, "char") == 0) {
        ast->node->next = newTokenNode(ast->tokens->CHAR, ast);
        ast->node = ast->node->next;
      } else if (strcmp(tok, "string") == 0) {
        ast->node->next = newTokenNode(ast->tokens->STRING, ast);
        ast->node = ast->node->next;
      } else if (strcmp(tok, "list") == 0) {
        ast->node->next = newTokenNode(ast->tokens->LIST, ast);
        ast->node = ast->node->next;
      } else if (strcmp(tok, "class") == 0) {
        ast->node->next = newTokenNode(ast->tokens->CLASS, ast);
        ast->node = ast->node->next;
      } else if (strcmp(tok, "new") == 0) {
        ast->node->next = newTokenNode(ast->tokens->NEW, ast);
        ast->node = ast->node->next;
      } else {
        ast->node->next = newTokenNode(ast->tokens->OBJECT, ast);
        ast->node = ast->node->next;
        ast->node->object = newObject();
      }
    }
  }

  ast->node->next = NULL;
  ast->node = starting_node;

  return ast;
}

union generic eval(t_expression *expression) {
  union generic value;
  value.c = 0;

  // print each value of the linked list of nodes
  struct node* node = expression->ast->node;
  while(node != NULL){
    printf("%d\n", node->token);
    node = node->next;
  }
  return value;
}
