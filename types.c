#include "types.h"

struct function* newFunction(object_t* (*f)(), char* name, int params) {
  struct function *function = (struct function*) malloc(sizeof(struct function));
  function->native = true;
  function->pointer = f;
  function->name = (char* ) malloc(255);
  function->name = name;
  function->params = params;
  return function;
}

struct function* newZFunction(char* name, list_t* args, list_t* body, int params) {
  struct function *function = (struct function*) malloc(sizeof(struct function));
  function->native = false;
  function->name = (char* ) malloc(255);
  function->name = name;
  function->args = args;
  function->body = body;
  function->params = params;
  return function;
}
