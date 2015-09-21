#ifndef __TYPES_H
#define __TYPES_H

#include <stdlib.h>
#include <stdbool.h>

enum t_type { Int, Float, Char, String, Bool, List, Exception, Function, Symbol};

struct atom;

typedef struct {
  struct atom* head;
  struct atom* atom;
} t_list;

typedef union {
  int     i;
  char    c;
  char*   s;
  double  f;
  bool    b;
  t_list* l;
} t_generic_value;

typedef struct {
  t_generic_value value;
  enum t_type type;
} t_generic;

typedef struct {
  t_generic* value;
  int id;
} t_object;

struct atom {
  t_object* value;
  struct atom* next;
};

#endif
