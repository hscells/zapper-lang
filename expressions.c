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

#define MIN_BUILTIN 0xa000
#define MAX_BUILTIN 0xa012

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
  tokens->BOOL   = 0x0020;
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
  // builtins
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

  // list ops
  system->LIST    = 0xa013;
  system->CONJ    = 0xa014;
  system->FIRST   = 0xa015;
  system->REST    = 0xa016;
  system->LENGTH  = 0xa017;
  return system;
}

t_ast* newAst () {
  t_token* tokens = initTokens();
  t_system* system = initSystem();
  t_ast* ast = (t_ast*) malloc(sizeof(t_ast));
  ast->line_count = 1;
  ast->tokens = tokens;
  ast->system = system;
  ast->head = newTokenNode(ast->tokens->START, ast);
  ast->node = ast->head;
  ast->head->next = ast->node;
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
  node->object = newObject();
  node->object->value->value = object->value->value;
  node->object->value->type = object->value->type;
  return node;
}

void collect_ast(t_ast *ast) {
  struct node* curr;
  struct node* head = ast->head;
  while((curr = head) != NULL) {
    head = head->next;
    free(curr);
  }
  free(ast);
}

/**
 * clean up a list structure
 * @param l the list
 */
void collect_list(t_list* l) {
  struct atom* curr = l->head;
  struct atom* head = l->head;
  while((curr = head) != NULL) {
    head = head->next;
    free(curr);
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
  char start_char;
  char end_char;
  char* c;

  c = token;
  start_char = token[0];
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
    end_char = *c;
    c++;
  }

  if (contains_char && strlen(token) > 1 && start_char == '"' && end_char == '"') {
    return String;
  } else if (contains_char && strlen(token) == 1 && start_char == '\'' && end_char == '\'') {
    return Char;
  } else if (contains_number && !contains_precision && !contains_char) {
    return Int;
  } else if (contains_number && contains_precision && !contains_char) {
    return Float;
  }

  // wtf do I do now?
  return Symbol;
}

t_ast* parse(char* e, t_stack* stack, t_heap* heap, int index) {
  int i;
  char c;
  char cToStr[2];
  int type;
  char tok[255];
  t_ast* ast = newAst();

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

    // who the heck cares about spaces!?
    while (c == ' ' || c == '\t'){
      c = e[++i];
    }


    ast->node->line_num = ast->line_count;

    // parse string objects
    if (c == '"') {
      // need to do this twice.
      c = e[i++];
      c = e[i++];
      // for whatever c = e[i+=2] won't work...

      char* string = malloc(sizeof(char) * 255);
      int ch = 0;
      t_object* obj = newObject();

      obj->value->type = String;
      while(c != '"') {
        string[ch] = c;
        c = e[i++];
        ch++;
      }

      string[ch] = '\0';
      obj->value->value.s = string;
      ast->node->next = newObjectNode(obj, ast);
      ast->node = ast->node->next;
      c = e[i];
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
    while (c != '(' && c != ',' && c != ')' && c != '{' && c != '}' && i < strlen(e)) {
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
      } else if (strcmp(tok, "println") == 0) {
        ast->node->next = newTokenNode(ast->system->PRINTLN, ast);
        ast->node = ast->node->next;
      } else if (strcmp(tok, "read") == 0) {
        ast->node->next = newTokenNode(ast->system->READ, ast);
        ast->node = ast->node->next;
      } else if (strcmp(tok, "throw") == 0) {
        ast->node->next = newTokenNode(ast->system->EXCEPT, ast);
        ast->node = ast->node->next;
      } else {
        type = inferType(tok);
        // printf("this object is of type <%d>: %s\n", type, tok);
        t_object* obj = newObject();
        char* copy;
        switch(type) {
          case -1:
            exception("Could not determine the type of this object", ast->node->line_num, tok);
          case Int:
            obj->value->value = (t_generic_value) atoi(tok);
            obj->value->type = Int;
            ast->node->next = newObjectNode(obj, ast);
            break;
          case Char:
            obj->value->value = (t_generic_value) tok[0];
            obj->value->type = Char;
            ast->node->next = newObjectNode(obj, ast);
            break;
          case Float:
            obj->value->value = (t_generic_value) atof(tok);
            obj->value->type = Char;
            ast->node->next = newObjectNode(obj, ast);
            break;
          // strings and symbols are a little more tricky, because of pointers
          // so we need to allocate space for the new char*
          case Symbol:
            copy = malloc(strlen(tok) + 1);
            strcpy(copy, tok);
            obj->value->type = Symbol;
            obj->value->value.s = copy;
            ast->node->next = newObjectNode(obj, ast);
            break;
        }
        collect(obj);
        ast->node = ast->node->next;
      }
    }

  }
  ast->node->next = NULL;
  return ast;
}

bool nodeIsFunction(struct node* node, t_ast* ast){
  if (node->token <= MAX_BUILTIN && node->token >= MIN_BUILTIN) {
    return true;
  } else if (node->token == ast->tokens->IF){
    return true;
  } else if (node->token == ast->tokens->RETURN){
    return true;
  } else if (node->object != NULL && node->object->value->type == Function) {
    return true;
  }
  return false;
}

/**
 * given an ast location, return a list object
 * @param  ast the ast object with the head node pointing at the start of the list
 * @return     a list object
 */
t_list* getList(t_ast* ast) {
  int depth = 0;

  struct node* node = ast->head;
  t_list* list = z_list();

  while ((node = node->next) != NULL) {
    if (node->token == ast->tokens->LBRAC) {
      depth++;
    } else if (node->token == ast->tokens->RBRAC) {
      depth--;
      if (depth == 0) {
        return list;
      }
    }

    else if (node->token == ast->tokens->OBJECT) {
      z_conj(list, node->object);
    }

    if (node->next == NULL) {
      exception("Unmatched parenthesis", node->line_num, ")");
    }
  }
  return list;
}

t_object* eval(t_ast *ast) {
  t_object* value;
  t_list* params;
  int paren_count = 0;

  // bool inside_function_definition = false;
  bool inside_actual_parameters = false;
  int current_function = 0;

  // print each value of the linked list of nodes
  struct node* node = ast->head;

  node = ast->head;
  printf("new eval stack\n");
  while(node != NULL){
    printf("token : %d\n", node->token);
    // printf("%d\n", node->token);
    // make sure there isn't a mismatched paren
    // also keep track of the current list structure
    if (node->token == ast->tokens->LBRAC) {
      paren_count++;
      if (current_function > 0) {
        params = z_list();
        inside_actual_parameters = true;
      }

    } else if (node->token == ast->tokens->RBRAC ) {
      paren_count--;
      if (paren_count == 0) {
        if (current_function == 0) {

        } else if(current_function == ast->system->ADD) { // otherwise evaluate a builtin
          if (z_length(params) == 2) {
            return z_add(params->head->value, params->head->next->value);
          } else {
            exception("Wrong number of arguments, function expects 2", node->line_num, "+");
          }
        } else if(current_function == ast->system->SUB) {
          if (z_length(params) == 2) {
            return z_sub(params->head->value, params->head->next->value);
          } else {
            exception("Wrong number of arguments, function expects 2", node->line_num, "-");
          }
        } else if(current_function == ast->system->MUL) {
          if (z_length(params) == 2) {
            return z_mul(params->head->value, params->head->next->value);
          } else {
            exception("Wrong number of arguments, function expects 2", node->line_num, "*");
          }
        } else if(current_function == ast->system->DIV) {
          if (z_length(params) == 2) {
            return z_div(params->head->value, params->head->next->value);
          } else {
            exception("Wrong number of arguments, function expects 2", node->line_num, "/");
          }
        } else if(current_function == ast->system->EQ) {
          if (z_length(params) == 2) {
            return z_eq(params->head->value, params->head->next->value);
          } else {
            exception("Wrong number of arguments, function expects 2", node->line_num, "=");
          }
        } else if(current_function == ast->system->LT) {
          if (z_length(params) == 2) {
            return z_lt(params->head->value, params->head->next->value);
          } else {
            exception("Wrong number of arguments, function expects 2", node->line_num, "<");
          }
        } else if(current_function == ast->system->GT) {
          if (z_length(params) == 2) {
            return z_gt(params->head->value, params->head->next->value);
          } else {
            exception("Wrong number of arguments, function expects 2", node->line_num, ">");
          }
        } else if(current_function == ast->system->LTEQ) {
          if (z_length(params) == 2) {
            return z_lteq(params->head->value, params->head->next->value);
          } else {
            exception("Wrong number of arguments, function expects 2", node->line_num, "<=");
          }
        } else if(current_function == ast->system->GTEQ) {
          if (z_length(params) == 2) {
            return z_gteq(params->head->value, params->head->next->value);
          } else {
            exception("Wrong number of arguments, function expects 2", node->line_num, ">=");
          }
        } else if(current_function == ast->system->PRINT) {
          if (z_length(params) == 1) {
            z_print(params->head->value);
            return value;
          } else {
            exception("Wrong number of arguments, function expects 1", node->line_num, "print");
          }
        } else if(current_function == ast->system->PRINTLN) {
          if (z_length(params) == 1) {
            z_println(params->head->value);
            return value;
          } else {
            exception("Wrong number of arguments, function expects 1", node->line_num, "println");
          }
        } else if(current_function == ast->system->READ) {
          return z_read();
        } else if(current_function == ast->system->EXIT) {
          node->next = NULL;
        } else {
          exception("Could not evaluate function.", node->line_num, "<Function Object>");
        }

        current_function = 0;
      }
    }

    // if we see a left curl, stop evaluating, if we see a right curl, also stop evaluating
    // stop on a left because we need something to separate the execution
    // stop on a right because we need to stop execution
    // braces act as an expression body
    if (node->token == ast->tokens->LCURL || node->token == ast->tokens->RCURL){
      return value;
    }

    /**
     * Logic for a return
     * return(EXPRESSION)
     * The value inside the return is evaluated and set to the current value context
     */
    if (node->token == ast->tokens->RETURN) {
      t_ast* newast = newAst();
      newast->head = node->next->next;
      value = eval(newast);
    }

    /**
     * Creates a new symbol->object pair and add the combo to a symbol table
     */
    if (node->token == ast->tokens->INT
    ||  node->token == ast->tokens->FLOAT
    ||  node->token == ast->tokens->CHAR
    ||  node->token == ast->tokens->STRING
    ||  node->token == ast->tokens->BOOL) {
      t_ast* tmp_ast = newAst();
      tmp_ast->head = node;
      t_list* args = getList(tmp_ast);
      // the parsing creates objects and infers the type ahead of execution, so the zapper typeof function can be used
      if (z_length(args) == 2) {
        switch(z_typeof(z_nth(args,1))){
          case Int:
            z_nth(args, 0)->value->type = Int;
            addObjectToSymbolTable(symboltable, z_nth(args, 0), z_nth(args, 1), node);
            break;
          case Float:
            z_nth(args, 0)->value->type = Float;
            addObjectToSymbolTable(symboltable, z_nth(args, 0), z_nth(args, 1), node);
            break;
          case Char:
            z_nth(args, 0)->value->type = Char;
            addObjectToSymbolTable(symboltable, z_nth(args, 0), z_nth(args, 1), node);
            break;
          case String:
            z_nth(args, 0)->value->type = String;
            addObjectToSymbolTable(symboltable, z_nth(args, 0), z_nth(args, 1), node);
            break;
          case Bool:
            z_nth(args, 0)->value->type = Bool;
            addObjectToSymbolTable(symboltable, z_nth(args, 0), z_nth(args, 1), node);
            break;
          default:
            exception("Could not reliably determine the type to create the object", node->line_num, z_nth(args, 0)->value->value.s);
        }
      } else if (z_length(args) == 1) {
        if(node->token == ast->tokens->IF) {
          z_nth(args, 0)->value->type = Int;
        } else if (node->token == ast->tokens->FLOAT) {
          z_nth(args, 0)->value->type = Float;
        } else if (node->token == ast->tokens->CHAR) {
          z_nth(args, 0)->value->type = Char;
        } else if (node->token == ast->tokens->STRING) {
          z_nth(args, 0)->value->type = String;
        } else if (node->token == ast->tokens->BOOL) {
          z_nth(args, 0)->value->type = Bool;
        }
        addObjectToSymbolTable(symboltable, z_nth(args, 0), NULL, node);
      } else {
        exception("Invalid number of arguments passed to assignment", node->line_num, "int");
      }
      while (node->token != ast->tokens->RBRAC) {
        node = node->next;
      }
    }

    /**
     * Logic for a function definition
     * A fn is defined as follows:
     * fn(add,(a,b),{return(+(a,b))})
     */
    if (node->token == ast->tokens->FN) {
      t_ast* tmp_ast = newAst();
      tmp_ast->head = node;
      t_list* args = getList(tmp_ast);
      addFunctionToSymbolTable(symboltable, z_nth(args, 0)->value->value.s, node, z_rest(args));
      while (node->token != ast->tokens->RBRAC) {
        node = node->next;
      }
    }

    /**
     * Logic for an if expression
     * An if function takes the following parameters:
     * if(predicate_function,function,optional function)
     * nest if's by placing another if inside the functions
     */
    if (node->token == ast->tokens->IF) {
      struct node* last_node;
      struct node* predicate;
      struct node* function1;
      struct node* function2;
      struct node* tmp = node;
      int num_functions = 0;
      int matching_brakets = 0;
      while(tmp != NULL) {
        if (tmp->token == ast->tokens->LBRAC) {
          matching_brakets++;
        }
        if (tmp->token == ast->tokens->RBRAC) {
          matching_brakets--;
        }
        if (num_functions == 0) {
          // we have our predicate_function
          if (nodeIsFunction(tmp, ast) && matching_brakets == 1) {
            predicate = tmp;
            num_functions++;
          }
        }
        else if (num_functions == 1) {
          // we have the first function evaluated if true
          if (nodeIsFunction(tmp, ast) && matching_brakets == 1) {
            function1 = tmp;
            num_functions++;
          }
        }
        else if (num_functions == 2) {
          // we have the first function evaluated if true
          if (nodeIsFunction(tmp, ast) && matching_brakets == 1) {
            function2 = tmp;
            num_functions++;
          }
        }
        else if (num_functions == 3 && matching_brakets == 0) {
          last_node = tmp;
          num_functions++;
        }
        tmp = tmp->next;
      }
      t_ast* newast = newAst();
      newast->head = predicate;
      t_object* result = eval(newast);
      if (result->value->value.b == true) {
        // we know the predicate evaluated to true, so skip to that node
        // evaluate everything inside that expression body
        newast->head = function1;
        value = eval(newast);
      } else {
        // the predicate evaluated to false, so skip to the second expression body
        // evaluate the second expression body
        newast->head = function2;
        value = eval(newast);
      }
      // skip to the end of the if body
      node = last_node;
    }

    if (node->token <= MAX_BUILTIN && node->token >= MIN_BUILTIN) {
      current_function = node->token;
    }

    if (node->next != NULL && node->next->token <= MAX_BUILTIN && node->next->token >= MIN_BUILTIN) {
      if (inside_actual_parameters) {
        // this will evaluate parameters if they are functions
        // parameters are evaludated and reduced before being passed to the calling function
        t_object* param = newObject();
        t_ast* newast = newAst();
        newast->head = node->next;
        param = eval(newast);
        z_conj(params, param);
        while (node->token != ast->tokens->RBRAC) {
          node = node->next;
        }
        // free(newast);
      } else {
        // this will evaluate a builtin function and record it's value
        t_ast* newast = newAst();
        newast->head = node->next;
        value = eval(newast);
        while (node->next->token != ast->tokens->RBRAC) {
          node = node->next;
        }
        current_function = 0;
        // free(newast);
      }
    } else if (node->token == ast->tokens->OBJECT && node->object->value->type == Function) {
      //do something with the symbol table!
    } else if (node->token == ast->tokens->OBJECT) {
      // we know that this must be just a plain old object now
      if (inside_actual_parameters) {
        // if the ast is inside a parameter list, add the object to that list
        if(z_typeof(node->object)== Symbol) {
          z_conj(params, getSymbolByName(symboltable, node->object->value->value.s));
        } else {
          z_conj(params, node->object);
        }
      }
    }

    node = node->next;
  }
  return value;
}
