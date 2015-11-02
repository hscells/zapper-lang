#ifndef __TYPES_H
#define __TYPES_H

#include <stdlib.h>
#include <stdbool.h>

enum t_type { Int, Float, Char, String, Bool, List, Exception, Function, FunctionReference, Symbol};

struct atom;
struct function;

typedef struct {
  struct atom* head;
  struct atom* tail;
} list_t;

typedef union {
  int         i;
  char        c;
  char*       s;
  double      f;
  bool        b;
  list_t*     l;
  struct function* function;
} generic_value_t;

typedef struct {
  generic_value_t value;
  enum t_type type;
} generic_t;

typedef struct {
  generic_t* value;
  int id;
} object_t;

struct function {
  // store if the function is C code or zapper code
  bool native;
  // if it's C code, the function pointer is stored here
  object_t* (*pointer)(list_t* args);
  // if it's zapper code, the body and arguments are stored here
  list_t* body;
  list_t* args;
  // both C and zapper code use these
  char* name;
  int params;
  char* namespace;
};

struct atom {
  object_t* value;
  struct atom* next;
};

struct function* newFunction(object_t* (*pointer)(), char* name, int params);
struct function* newZFunction(char* name, list_t* args, list_t* body, int params);

#endif
