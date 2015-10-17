#ifndef __TYPES_H
#define __TYPES_H

#include <stdlib.h>
#include <stdbool.h>

enum t_type { Int, Float, Char, String, Bool, List, Exception, Function, Symbol};

struct atom;
struct function;

typedef struct {
  struct atom* head;
  struct atom* tail;
} t_list;

typedef struct {
  int pointer;
  char* name;
} t_symbol;

typedef union {
  int         i;
  char        c;
  char*       s;
  double      f;
  bool        b;
  t_list*     l;
  t_symbol*   symbol;
  struct function* function;
} t_generic_value;

typedef struct {
  t_generic_value value;
  enum t_type type;
} t_generic;

typedef struct {
  t_generic* value;
  int id;
} t_object;

struct function {
  t_object* (*pointer)(t_list* args);
  char* name;
  int params;
};

struct atom {
  t_object* value;
  struct atom* next;
};

struct function* newFunction(t_object* (*pointer)(), char* name, int params);
t_symbol* newSymbol(int pointer, char* name);

#endif
