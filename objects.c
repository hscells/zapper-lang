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

t_object* newObject() {
  t_object *obj = (t_object*) malloc(sizeof(t_object));
  obj->id = newObjectId();
  obj->value = newGeneric();
  return obj;
}

t_object* newList(t_list* list) {
  t_object *obj = (t_object*) malloc(sizeof(t_object));
  obj->id = newObjectId();
  obj->value->value = (t_generic_value) list;
  obj->value->type = List;
  return obj;
}
