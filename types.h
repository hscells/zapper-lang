#ifndef __TYPES_H
#define __TYPES_H

#include <stdlib.h>
#include <stdbool.h>

enum t_type { Int, Float, Char, String, Bool, List, Exception, Function, Symbol};

typedef union {
  int     i;
  char    c;
  char*   s;
  double  f;
  bool    b;
} t_generic_value;

typedef struct {
  t_generic_value value;
  enum t_type type;
} t_generic;

#endif
