#ifndef __TYPES_H
#define __TYPES_H

#include <stdlib.h>

typedef struct {
  int type;
} t_type;

t_type* Int();
t_type* t_char();
t_type* t_string();

union generic {
  int i;
  char c;
  char* s;
};

int getType(t_type *type);

#endif
