#ifndef __TYPES_H
#define __TYPES_H

#include <stdlib.h>

typedef struct {
  int Int;
  int Float;
  int Char;
  int String;
  int List;
} t_type;

union generic {
  int i;
  char c;
  char* s;
  float f;
};

t_type* Type;

int getType(int type);
void initTypes();
#endif
