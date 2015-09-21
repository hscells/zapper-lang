#include <stdio.h>
#include <stdlib.h>
#include "objects.h"
#include "system.h"

#define STACK_SIZE 1024;
#define HEAP_SIZE 1024;
#define METHOD_LIMIT 512;
#define FIELD_LIMIT 512;

int object_id = 0;
int method_id = 0;
int field_id = 0;

int newObjectId() {
  return object_id++;
}

t_methodlist* newMethodlist() {
  t_methodlist *methods = (t_methodlist*) malloc(sizeof(t_methodlist));
  return methods;
}
t_object* newObject() {
  t_object *obj = (t_object*) malloc(sizeof(t_object));
  obj->id = newObjectId();
  obj->value = newGeneric();
  return obj;
}

t_heap* newHeap() {
  t_heap* heap = (t_heap*) malloc(sizeof(t_heap));
  return heap;
}

t_stack* newStack() {
  t_stack* stack = (t_stack*) malloc(sizeof(t_stack));
  return stack;
}
