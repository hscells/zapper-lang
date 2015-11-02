#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>
#include "expressions.h"

#define EOL '\n'

#define NUMBERS "1234567890"
#define CHARACTERS "!?-+*/_=<>abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define PRECISION "."

int node_id = 0;

/**
 * given a char pointer, try to infer the type of the actual value
 * @param  token the token being looked at
 * @return       a type from types.h
 */
enum t_type inferType(char* token) {
  bool contains_number = false;
  bool contains_char = false;;
  bool contains_precision = false;
  char start_char;
  char end_char;
  char* c;

  c = token;
  start_char = token[0];
  if (start_char == '@') {
    return FunctionReference;
  }
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

  if ((contains_char || contains_number || contains_precision) && strlen(token) > 1 && start_char == '"' && end_char == '"') {
    return String;
  } else if ((contains_char || contains_number || contains_precision) && start_char == '\'' && end_char == '\'') {
    return Char;
  } else if (contains_number && !contains_precision && !contains_char) {
    return Int;
  } else if (contains_number && contains_precision && !contains_char) {
    return Float;
  } else if (contains_char && strlen(token) > 0) {
    return Symbol;
  }

  // wtf do I do now?
  return Exception;
}

void printast(list_t* ast, int t) {
  struct atom* a = ast->head;
  while (a != NULL) {
    if (a->value->value->type == List) {
      printast(a->value->value->value.l, t + 1);
    } else {
      for (int i = 0; i < t; i++) {
        printf("\t");
      }
      printf("parsed: %d\n", a->value->value->type);
    }
    a = a->next;
  }
}

object_t* parse(char* e) {
  int i;
  char c;
  enum t_type type;
  char tok[255];
  char cToStr[2];

  list_t* ast = malloc(sizeof(list_t*) * 256);

  list_t* expressions = z_list()->value->value.l;
  // list_t* parenlist_t = expressions;
  list_t* currenlist_t = expressions;
  object_t* root_node = newObject();
  root_node->value->value = (generic_value_t) expressions;
  root_node->value->type = List;
  int line_count = 0;

  int b_count = 0;

  for(i = 0; i < strlen(e) - 1; i++) {

    memset(tok,0,sizeof(tok));
    c = e[i];
    // skip over comments
    if (c == ';') {
      while (c != EOL) {
        c = e[++i];
      }
    }

    // continue to ignore spaces and tabs
    while (c == ' ' || c == '\t'){
      c = e[++i];
    }

    // maintain the line count
    while (c == EOL) {
      line_count++;
      c = e[++i];
    }

    if (c == '"') {
      c = e[++i];
      while (c != '"' && i < strlen(e)) {
        cToStr[0] = c;
        cToStr[1] = '\0';
        strcat(tok, cToStr);
        c = e[++i];
      }
      object_t* obj = newObject();
      char* copy = malloc(strlen(tok) + 1);
      strcpy(copy, tok);
      obj->value->type = String;
      obj->value->value.s = copy;
      z_conj(currenlist_t, obj);
    } else if (c == '(') {

      // create a new object to store the nested expression
      object_t* nested_expression = newObject();
      nested_expression->value->value = (generic_value_t) z_list()->value->value.l;
      nested_expression->value->type = List;

      // join the new nested expression onto the parent expression
      z_conj(currenlist_t, nested_expression);

      // maintain a reference to the parent list
      ast[++b_count] = *currenlist_t;

      // continue to parse the nested expression
      currenlist_t = nested_expression->value->value.l;
    }

    else if (c == ')') {
      currenlist_t = &ast[b_count--];
    }

    else {
      while (c != '(' && c != ')' && c != ' ' && c != '\t' && c != '\n' && i < strlen(e)) {
        cToStr[0] = c;
        cToStr[1] = '\0';
        strcat(tok, cToStr);
        c = e[++i];
      }
      c = e[--i];
      type = inferType(tok);
      if (type != Exception){
        // printf("this object is of type <%d>: %s\n", type, tok);
        object_t* obj = newObject();
        char* copy;
        switch(type) {
          case Int:
            obj->value->value = (generic_value_t) atoi(tok);
            obj->value->type = Int;
            z_conj(currenlist_t, obj);
            break;
          case Char:
            obj->value->value.c = tok[1];
            obj->value->type = Char;
            z_conj(currenlist_t, obj);
            break;
          case Float:
            obj->value->value = (generic_value_t) atof(tok);
            obj->value->type = Char;
            z_conj(currenlist_t, obj);
            break;
          // strings and symbols are a little more tricky, because of pointers
          // so we need to allocate space for the new char*
          case Symbol:
            copy = malloc(strlen(tok) + 1);
            strcpy(copy, tok);
            obj->value->type = Symbol;
            obj->value->value.s = copy;
            z_conj(currenlist_t, obj);
            break;
          case FunctionReference:
            copy = malloc(strlen(tok) + 1);
            strcpy(copy, tok);
            obj->value->type = FunctionReference;
            obj->value->value.s = copy;
            z_conj(currenlist_t, obj);
            break;
          default:
            exception("Could not determine the type of this object", line_count, tok);
        }
      }
    }
  }
  // printf("there are %d expression(s) in root s-expression\n", z_length(expressions)->value->value.i);
  // printast(expressions, -1);
  return root_node;
}

object_t* cond(list_t* conditions, symboltable_t* context) {
  struct atom* cond = conditions->head;
  while (cond != NULL) {
    if (cond->value->value->type == List) {
      if (z_nth(cond->value->value->value.l, 0)->value->type == List && z_nth(cond->value->value->value.l, 1)->value->type == List){
        object_t* result = eval(z_nth(cond->value->value->value.l, 0)->value->value.l, context);
        if (result->value->type == Bool){
          if (result->value->value.b) {
            return eval(z_nth(cond->value->value->value.l, 1)->value->value.l, context);
          }
        } else {
          exception("predicates in cond must return a bool",-1,NULL);
        }
      } else if (z_nth(cond->value->value->value.l, 0)->value->type == Symbol && z_nth(cond->value->value->value.l, 1)->value->type == List) {
        char* symbol = z_nth(cond->value->value->value.l, 0)->value->value.s;
        object_t *result;
        if (inSymboltable(globals, symbol)) {
          result = getSymbolByName(globals, symbol);
        } else if (inSymboltable(context, symbol)) {
          result = getSymbolByName(context, symbol);
        } else {
          result = z_nth(cond->value->value->value.l, 0);
        }
        if (result->value->type == Bool){
          if (result->value->value.b) {
            return eval(z_nth(cond->value->value->value.l, 1)->value->value.l, context);
          }
        } else {
          exception("predicates in cond must return a bool",-1,NULL);
        }
      } else{
        exception("cond requires ((predicate) (expression)) forms",-1,NULL);
      }
    } else {
      exception("cond requires a list to evaluate",-1,NULL);
    }
    cond = cond->next;
  }
  return NULL;
}

object_t* call(struct function* function, list_t* args, symboltable_t* context) {
  list_t* newargs = z_list()->value->value.l;
  struct atom* currentAtom = args->head;

  // function definitions require a little trickery and for their parameters to not be evaluated
  if (strcmp(function->name, "fn") == 0
   || strcmp(function->name, "import") == 0
   || strcmp(function->name, "let") == 0
   || strcmp(function->name, "list") == 0) {
    if (z_length(args)->value->value.i == function->params || function->params == -1) {
      return (*function->pointer)(args);
    } else {
      exception("Parameter count mismatch.", -1, function->name);
      return NULL;
    }
  } else if (strcmp(function->name, "cond") == 0) {
    return cond(args, context);
  } else { // zapper and C functions get evaluated the same
    while (currentAtom != NULL) {
      // recursively evaluate and substitite the parameters
      if (currentAtom->value->value->type == List) {
        object_t* v = eval(currentAtom->value->value->value.l, context);
        z_conj(newargs, v);
      } else if (currentAtom->value->value->type == Symbol) {
        if (inSymboltable(globals, currentAtom->value->value->value.s)) {
          z_conj(newargs, getSymbolByName(globals, currentAtom->value->value->value.s));
        } else if (inSymboltable(context, currentAtom->value->value->value.s)) {
          z_conj(newargs, getSymbolByName(context, currentAtom->value->value->value.s));
        } else if (inSymboltable(clib_functions, currentAtom->value->value->value.s)) {
          z_conj(newargs, getSymbolByName(clib_functions, currentAtom->value->value->value.s));
        } else {
          exception("Symbol does not exist in local or global scope", -1, currentAtom->value->value->value.s);
          return NULL;
        }
      } else {
        z_conj(newargs, currentAtom->value);
      }
      currentAtom = currentAtom->next;
    }
    // if the parameter count matches
    if (z_length(newargs)->value->value.i == function->params) {
      // determine if the function is a C function or a zapper function and execute it
      if (function->native) {
        return (*function->pointer)(newargs);
      } else {
        symboltable_t* new_context = newSymbolTable();
        list_t* func_ast = function->body;
        for (int i = 0; i < function->params; i++){
            addObjectToSymbolTable(new_context, z_nth(function->args, i), z_nth(newargs, i));
        }
        return eval(func_ast, new_context);
      }
    } else {
      exception("Parameter count mismatch.", -1, function->name);
      return NULL;
    }
  }
}

object_t* eval(list_t* ast, symboltable_t* context) {
  int param_count;
  struct atom* currentAtom = ast->head;
  object_t* value = newObject();
  while (currentAtom != NULL) {

    // call function if there is a literal function object
    if (currentAtom->value->value->type == Function && currentAtom == ast->head) {
      return call(currentAtom->value->value->value.function, z_rest(ast)->value->value.l, context);
    }

    // call function if there is a symbol representing a function
    else if (currentAtom->value->value->type == Symbol && currentAtom == ast->head) { // is the symbol at the start of the list a function?
      param_count = z_length(z_rest(ast)->value->value.l)->value->value.i;
      if (inSymboltable(clib_functions, currentAtom->value->value->value.s) && getFunctionParamCount(clib_functions, currentAtom->value->value->value.s) == z_length(ast)->value->value.i - 1) {
        struct function* temp_func = getFunctionFromSymbolTable(clib_functions, currentAtom->value->value->value.s, param_count);
        return call(temp_func, z_rest(ast)->value->value.l, context);
      } else if (inSymboltable(globals, currentAtom->value->value->value.s) && getFunctionParamCount(globals, currentAtom->value->value->value.s) == z_length(ast)->value->value.i - 1) {
        struct function* temp_func = getFunctionFromSymbolTable(globals, currentAtom->value->value->value.s, param_count);
        return call(temp_func, z_rest(ast)->value->value.l, context);
      } else if (inSymboltable(context, currentAtom->value->value->value.s) && getFunctionParamCount(context, currentAtom->value->value->value.s) == z_length(ast)->value->value.i - 1) {
        struct function* temp_func = getFunctionFromSymbolTable(context, currentAtom->value->value->value.s, param_count);
        return call(temp_func, z_rest(ast)->value->value.l, context);
      } else {
        exception("Function does not exist in the local or global scope or the parameter count was incorrect.", -1, currentAtom->value->value->value.s);
        return newObject();
      }
    }

    // call a function over a nested list of arguments
    else if (currentAtom->value->value->type == FunctionReference && currentAtom == ast->head) { // is the symbol a nested list?
      param_count = z_length(z_rest(ast)->value->value.l)->value->value.i;
      value = eval(currentAtom->value->value->value.l, context);
      struct function* temp_func = getFunctionFromSymbolTable(clib_functions, currentAtom->value->value->value.s, param_count);
      return call(temp_func, z_rest(ast)->value->value.l, context);
    }

    // if the list is a nested list, evaluate down
    else if (currentAtom->value->value->type == List) { // is the symbol a nested list?
      value = eval(currentAtom->value->value->value.l, context);
    }

    currentAtom = currentAtom->next;
  }
  free(currentAtom);
  return value;
}
