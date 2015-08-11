#include <stdio.h>
#include <stdlib.h>
#include "objects.h"
#include "types.h"

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

t_fieldlist* newFieldlist() {
  t_fieldlist *fields = (t_fieldlist*) malloc(sizeof(t_fieldlist));
  return fields;
}

void addMethodToMethodlist(t_methodlist* methodlist, char* name, int type) {
  methodlist->method[methodlist->index] = (t_symbol*) malloc(sizeof(t_symbol));
  methodlist->method[methodlist->index]->type = type;
  methodlist->method[methodlist->index]->object = NULL;
  methodlist->method[methodlist->index]->id = method_id++;
  methodlist->index++;
}

void addFieldToFieldlist(t_fieldlist* fieldlist, char* name, int type, t_object* object) {
  fieldlist->field[fieldlist->index] = (t_symbol*) malloc(sizeof(t_symbol));
  fieldlist->field[fieldlist->index]->type = type;
  fieldlist->field[fieldlist->index]->object = object;
  fieldlist->field[fieldlist->index]->id = field_id++;
  fieldlist->index++;
}

void addMethodlistToObject(t_object* object, t_methodlist* methodlist) {
  object->methods = methodlist;
}

void addFieldlistToObject(t_object* object, t_fieldlist* fieldlist) {
  object->fields = fieldlist;
}

t_object* newObject() {
  t_object *obj = (t_object*) malloc(sizeof(t_object));
  obj->id = newObjectId();
  obj->fields = newFieldlist();
  obj->methods = newMethodlist();
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
