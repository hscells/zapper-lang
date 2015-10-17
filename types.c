#include "types.h"

struct function* newFunction(t_object* (*f)(), char* name, int params) {
  struct function* function = (struct function*) malloc(sizeof(struct function));
  function->pointer = f;
  function->name = name;
  function->params = params;
  return function;
}

t_symbol* newSymbol(int pointer, char* name) {
  t_symbol* symbol = (t_symbol*) malloc(sizeof(t_symbol));
  symbol->pointer = pointer;
  symbol->name = name;
  return symbol;
}
