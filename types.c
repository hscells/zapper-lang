#include "types.h"

struct function* newFunction(t_object* (*f)(), char* name, int params) {
  struct function *function = (struct function*) malloc(sizeof(struct function));
  function->native = true;
  function->pointer = f;
  function->name = (char* ) malloc(255);
  function->name = name;
  function->params = params;
  return function;
}

struct function* newZFunction(char* name, t_list* args, t_list* body, int params) {
  struct function *function = (struct function*) malloc(sizeof(struct function));
  function->native = false;
  function->name = (char* ) malloc(255);
  function->name = name;
  function->args = args;
  function->body = body;
  function->params = params;
  return function;
}

t_symbol* newSymbol(int pointer, char* name) {
  t_symbol *symbol = (t_symbol*) malloc(sizeof(t_symbol));
  symbol->pointer = pointer;
  symbol->name = name;
  return symbol;
}
