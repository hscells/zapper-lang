#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>
#include "expressions.h"
#include "system.h"
#include "garbage.h"
#include "types.h"

#define EOL '\n'

#define NUMBERS "1234567890"
#define CHARACTERS "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define PRECISION "."

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
  system->ADD     = 0xa000;
  system->SUB     = 0xa001;
  system->MUL     = 0xa002;
  system->DIV     = 0xa003;
  system->EQ      = 0xa004;
  system->LT      = 0xa005;
  system->GT      = 0xa006;
  system->LTEQ    = 0xa007;
  system->GTEQ    = 0xa008;
  system->PRINT   = 0xa009;
  system->PRINTLN = 0xa012;
  system->READ    = 0xa010;
  system->EXCEPT  = 0xa011;
  // TODO: Add these symbols to the symbol table for function lookup as 'native code'
  // addFunctionToSymbolTable(symboltable, system->ADD, native_code, {Int, Int});
  return system;
}

t_ast* newAst () {
  t_token* tokens = initTokens();
  t_system* system = initSystem();
  t_ast* ast = (t_ast*) malloc(sizeof(t_ast));
  ast->tokens = tokens;
  ast->system = system;
  ast->node = newTokenNode(ast->tokens->START, ast);
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
  node->token = ast->tokens->OBJECT;
  node->object = object;
  return node;
}

void collectAst(t_ast *ast) {
  struct node* node = ast->node;
  while(node != NULL) {
    free(node);
    node = node->next;
  }
}

/**
 * clean up a list structure
 * @param l the list
 */
void collect_list(t_list* l) {
  struct atom* node = l->atom;
  while(node != NULL) {
    free(node);
    node = node->next;
  }
}

/**
 * given a char pointer, try to infer the type of the actual value
 * @param  token the token being looked at
 * @return       a type from types.h
 */
int inferType(char* token) {
  bool contains_number = false;
  bool contains_char = false;;
  bool contains_precision = false;
  char* c;

  c = token;
  while (*c) {
    if (strchr(NUMBERS, *c)){
      contains_number = true;
    }
    if (strchr(CHARACTERS, *c)) {
      contains_char = true;
    }
    if (strchr(PRECISION, *c)) {
      contains_precision = true;
    }
    c++;
  }

  if (contains_char && strlen(token) > 1) {
    return Type->String;
  } else if (contains_char && strlen(token) == 1) {
    return Type->Char;
  } else if (contains_number && !contains_precision && !contains_char) {
    return Type->Int;
  } else if (contains_number && contains_precision && !contains_char) {
    return Type->Float;
  }

  // wtf do I do now?
  return Type->Int;
}

t_ast* parse(char* e, t_stack* stack, t_heap* heap, int index) {
  int i;
  char c;
  char cToStr[2];
  int type;
  char tok[255];
  t_ast* ast = newAst();
  ast->node = newTokenNode(ast->tokens->START, ast);
  struct node* starting_node = ast->node;

  for(i = index; i < strlen(e) - 1; i++) {

    memset(tok,0,sizeof(tok));
    c = e[i];

    // skip over comments
    if (c == ';') {
      while (c != EOL) {
        i++;
        c = e[i];
      }
    }

    // maintain the line count
    while (c == EOL) {
      ast->line_count++;
      c = e[++i];
    }

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
    while (c != '(' && c != ',' && c != ')' && i < strlen(e)) {
      cToStr[0] = c;
      strcat(tok, cToStr);
      i++;
      c = e[i];
    }

    c = e[--i];

    // printf ("tok is: %s, c is: %c\n", tok, c);

    if (strlen(tok) > 0) {
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
      } else if (strcmp(tok, "+") == 0) {
        ast->node->next = newTokenNode(ast->system->ADD, ast);
        ast->node = ast->node->next;
      } else if (strcmp(tok, "-") == 0) {
        ast->node->next = newTokenNode(ast->system->SUB, ast);
        ast->node = ast->node->next;
      } else if (strcmp(tok, "*") == 0) {
        ast->node->next = newTokenNode(ast->system->MUL, ast);
        ast->node = ast->node->next;
      } else if (strcmp(tok, "/") == 0) {
        ast->node->next = newTokenNode(ast->system->DIV, ast);
        ast->node = ast->node->next;
      } else if (strcmp(tok, "=") == 0) {
        ast->node->next = newTokenNode(ast->system->EQ, ast);
        ast->node = ast->node->next;
      } else if (strcmp(tok, "<") == 0) {
        ast->node->next = newTokenNode(ast->system->LT, ast);
        ast->node = ast->node->next;
      } else if (strcmp(tok, ">") == 0) {
        ast->node->next = newTokenNode(ast->system->GT, ast);
        ast->node = ast->node->next;
      } else if (strcmp(tok, "<=") == 0) {
        ast->node->next = newTokenNode(ast->system->LTEQ, ast);
        ast->node = ast->node->next;
      } else if (strcmp(tok, ">=") == 0) {
        ast->node->next = newTokenNode(ast->system->GTEQ, ast);
        ast->node = ast->node->next;
      } else if (strcmp(tok, "print") == 0) {
        ast->node->next = newTokenNode(ast->system->PRINT, ast);
        ast->node = ast->node->next;
      } else if (strcmp(tok, "read") == 0) {
        ast->node->next = newTokenNode(ast->system->READ, ast);
        ast->node = ast->node->next;
      } else if (strcmp(tok, "throw") == 0) {
        ast->node->next = newTokenNode(ast->system->EXCEPT, ast);
        ast->node = ast->node->next;
      } else {
        // printf("this object is: %s\n", tok);
        type = inferType(tok);
        if (type == Type->Int) {
          ast->node->next = newObjectNode(newInt(atoi(tok), stack, heap), ast);
        }
        ast->node = ast->node->next;
      }
    }
  }

  ast->node->next = NULL;
  ast->node = starting_node;

  return ast;
}

union generic eval(t_ast *ast) {
  union generic value;
  value.c = 0;
  t_list* params;
  int paren_count = 0;

  // bool inside_function_definition = false;
  bool inside_actual_parameters = false;
  int current_function;

  // print each value of the linked list of nodes
  struct node* node = ast->node;
  struct node* starting_node = node;
  while(node != NULL){

    // make sure there isn't a mismatched paren
    // also keep track of the current list structure
    if (node->token == ast->tokens->LBRAC) {
      paren_count++;
      params = z_list();
    } else if (node->token == ast->tokens->RBRAC ) {
      paren_count--;
      if (paren_count == 0) {
        if(current_function == ast->system->ADD) {
          return z_add(params->head->value->value, params->head->next->value->value);
        } else if(current_function == ast->system->SUB) {
          return z_sub(params->head->value->value, params->head->next->value->value);
        } else if(current_function == ast->system->MUL) {
          return z_mul(params->head->value->value, params->head->next->value->value);
        } else if(current_function == ast->system->DIV) {
          return z_div(params->head->value->value, params->head->next->value->value);
        } else if(current_function == ast->system->EQ) {
          return z_eq(params->head->value, params->head->next->value);
        } else if(current_function == ast->system->LT) {
          return z_lt(params->head->value, params->head->next->value);
        } else if(current_function == ast->system->GT) {
          return z_gt(params->head->value, params->head->next->value);
        } else if(current_function == ast->system->LTEQ) {
          return z_lteq(params->head->value, params->head->next->value);
        } else if(current_function == ast->system->GTEQ) {
          return z_gteq(params->head->value, params->head->next->value);
        } else if(current_function == ast->system->PRINT) {
          z_print(params->head->value);
        } else if(current_function == ast->system->PRINTLN) {
          printf("hello\n");
          z_println(params->head->value);
        } else if(current_function == ast->system->READ) {
          return z_read();
        } else if(current_function == ast->system->EXIT) {
          node->next = NULL;
        }
        collect_list(params);
      }
    }

    if (node->token == ast->tokens->OBJECT) {
      printf("next token: %d\n", node->next->token);
      if (inside_actual_parameters) {
        // t_object* param_obj;
        // // need to check if the object is in the functions symbol table
        // if (node->next->token == ast->tokens->LBRAC) {
        //   // printf("going to recurse into this function\n");
        //   // t_ast* recur_ast = newAst();
        //   // param_obj = newObject();
        //   // recur_ast->head = node;
        //   // value = eval(recur_ast);
        //   // param_obj->value = value;
        //   // while (node->next != NULL && node->next->token != ast->tokens->RBRAC);
        //   param_obj = node->object;
        // } else {
        //   param_obj = node->object;
        // }
        z_conj(params, node->object);
      } else {
        current_function = node->token;
      }

    }
    node = node->next;
  }
  ast->node = starting_node;
  printf("done eval!\n");
  collectAst(ast);
  return value;
}
