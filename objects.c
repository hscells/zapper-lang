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

object_t* newObject() {
  object_t *obj = (object_t*) malloc(sizeof(object_t));
  obj->id = newObjectId();
  obj->value = newGeneric();
  obj->line_num = -1;
  return obj;
}

object_t* newList(list_t* list) {
  object_t *obj = (object_t*) malloc(sizeof(object_t));
  obj->id = newObjectId();
  obj->value->value = (generic_value_t) list;
  obj->value->type = List;
  return obj;
}
