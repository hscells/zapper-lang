#ifndef __OBJECTS_H
#define __OBJECTS_H

#include "types.h"

typedef struct {
  int index;
} t_methodlist;

typedef struct {
  int index;
} t_fieldlist;

typedef struct {
  int index;
  t_object* object[1024];
} t_stack;

typedef struct {
  int index;
  t_stack* stack[1024];
} t_heap;

int newObjectId();
t_object* newObject();

t_heap* newHeap();
t_stack* newStack();

#endif
