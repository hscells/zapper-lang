#ifndef __TYPES_H
#define __TYPES_H

#include <stdlib.h>

union generic {
  int i;
  char c;
  char* s;
  float f;
};

typedef struct {
  int Int;
  int Float;
  int Char;
  int String;
  int List;
} t_type;

t_type* Type;

int getType(int type);
void initTypes();
#endif
