#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>
#include "expressions.h"
#include "system.h"
#include "types.h"

#define EOL '\n'

#define NUMBERS "1234567890"
#define CHARACTERS "!?-_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define PRECISION "."

int node_id = 0;


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
enum t_type inferType(char* token) {
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

void printast(t_list* ast, int t) {
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

t_object* parse(char* e) {
  int i;
  char c;
  enum t_type type;
  char tok[255];
  char cToStr[2];

  t_list* expressions = z_list()->value->value.l;
  t_list* parent_list = expressions;
  t_object* root_node = newObject();
  root_node->value->value = (t_generic_value) expressions;
  root_node->value->type = List;
  t_list* current_list = expressions;
  int line_count = 0;

  for(i = 0; i < strlen(e) - 1; i++) {

    memset(tok,0,sizeof(tok));
    c = e[i];
    // skip over comments
    if (c == ';') {
      while (c != EOL) {
        i++;
        c = e[i];
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

    if (c == '(') {
      t_object* nested_expression = newObject();
      nested_expression->value->value = (t_generic_value) z_list()->value->value.l;
      nested_expression->value->type = List;
      z_conj(current_list, nested_expression);
      parent_list = current_list;
      current_list = nested_expression->value->value.l;
    }

    else if (c == ')') {
      current_list = parent_list;
    }

    else {
      while (c != '(' && c != ')' && c != ' ' && c != '\t' && i < strlen(e)) {
        cToStr[0] = c;
        cToStr[1] = '\0';
        strcat(tok, cToStr);
        c = e[++i];
      }
      c = e[--i];
      type = inferType(tok);
      printf("this object is of type <%d>: %s\n", type, tok);
      t_object* obj = newObject();
      char* copy;
      switch(type) {
        case Int:
          obj->value->value = (t_generic_value) atoi(tok);
          obj->value->type = Int;
          z_conj(current_list, obj);
          break;
        case Char:
          obj->value->value = (t_generic_value) tok[0];
          obj->value->type = Char;
          z_conj(current_list, obj);
          break;
        case Float:
          obj->value->value = (t_generic_value) atof(tok);
          obj->value->type = Char;
          z_conj(current_list, obj);
          break;
        // strings and symbols are a little more tricky, because of pointers
        // so we need to allocate space for the new char*
        case Symbol:
          copy = malloc(strlen(tok) + 1);
          strcpy(copy, tok);
          obj->value->type = Symbol;
          obj->value->value.s = copy;
          z_conj(current_list, obj);
          break;
        case Function:
         break;
        case List:
          break;
        case String:
          break;
        case Bool:
          break;
        case Exception:
          break;
        default:
          exception("Could not determine the type of this object", line_count, tok);
      }
    }
  }
  printast(expressions, -1);
  return root_node;
}

t_object* call(struct function* function, t_list* args) {
  t_list* newargs = z_list()->value->value.l;
  struct atom* currentAtom = args->head;
  printf("old args: %d\n",z_length(args)->value->value.i);
  while (currentAtom != NULL) {
    if (currentAtom->value->value->type == List) {
      printf("evaluating nested list... \n");
      t_object* v = eval(currentAtom->value->value->value.l);
      printf("result: %d\n", v->value->value.i);
      z_conj(newargs, v);
    } else {
      z_conj(newargs, currentAtom->value);
    }
    currentAtom = currentAtom->next;
  }
  printf("new args: %d\n",z_length(newargs)->value->value.i);
  if (z_length(newargs)->value->value.i == function->params) {
    return (*function->pointer)(newargs);
  } else {
    z_exception("Not enough paramaters to function");
    return NULL;
  }
}

t_object* eval(t_list* ast) {
  struct atom* currentAtom = ast->head;
  t_object* value = newObject();
  while (currentAtom != NULL) {

    printf("The type of the current node: %d \n", currentAtom->value->value->type);

    if (currentAtom->value->value->type == Symbol && currentAtom == ast->head) { // is the symbol at the start of the list a builtin C function?
      if (inSymboltable(clib_functions, currentAtom->value->value->value.s)) {
        struct function* temp_func = getFunctionFromSymbolTable(clib_functions, currentAtom->value->value->value.s);
        return call(temp_func, z_rest(ast)->value->value.l);
      }
    } else if (currentAtom->value->value->type == List) { // is the symbol a nested list?
      printf("evaluating nested expression...\n");
      return eval(currentAtom->value->value->value.l);
    }

    currentAtom = currentAtom->next;
  }
  free(currentAtom);
  return value;
}
