#include <string.h>
#include "system.h"

t_generic* newGeneric() {
  return (t_generic*) malloc(sizeof(t_generic));
}

enum t_type z_typeof(t_object* o) {
  return o->value->type;
}

t_object* z_read() {
  t_object* result = newObject();
  return result;
}

t_object* z_int(int x) {
  t_object* obj = newObject();
  obj->value->type = Int;
  obj->value->value.i = x;
  return obj;
}

t_object* z_string(char* x) {
  t_object* obj = newObject();
  obj->value->type = String;
  obj->value->value.s = x;
  return obj;
}

t_symboltable* newSymbolTable() {
  t_symboltable *s = (t_symboltable*) malloc(sizeof(t_symboltable));
  s->head = NULL;
  s->tail = NULL;
  return s;
}

void addFunctionToSymbolTable(t_symboltable* s, struct function* func) {
  struct t_symboltable_row* row = (struct t_symboltable_row*) malloc(sizeof(struct t_symboltable_row));
  row->name = func->name;
  row->object = newObject();

  row->object->value->type = Function;
  row->object->value->value.function = func;
  row->id = row->object->id;

  row->next = NULL;
  if (s->head == NULL) {
    s->head = row;
    s->tail = row;
  } else {
    s->tail->next = row;
    s->tail = row;
  }

}

struct function* getFunctionFromSymbolTable(t_symboltable* s, char* name) {
  struct t_symboltable_row* r = s->head;
  while (r != NULL) {
    if (strcmp(r->name, name) == 0) {
      return r->object->value->value.function;
    }
    r = r->next;
  }
  exception("Object has no value",-1,name);
  return NULL;
}

void addObjectToSymbolTable(t_symboltable* s, t_object* symbol, t_object* object, struct node *node) {
  struct t_symboltable_row* row = (struct t_symboltable_row*) malloc(sizeof(struct t_symboltable_row));
  row->name = symbol->value->value.s;
  if (object != NULL) {
    row->object = object;
    row->id = object->id;
  }

  row->next = NULL;
  if (s->head == NULL) {
    s->head = row;
    s->tail = row;
  } else {
    s->tail->next = row;
    s->tail = row;
  }
}

bool inSymboltable(t_symboltable* s, char* name) {
  struct t_symboltable_row* r = s->head;
  while(r != NULL) {
    if (strcmp(r->name, name) == 0) {
      return true;
    }
    r = r->next;
  }
  return false;
}

void printSymboltable(t_symboltable* s) {
  struct t_symboltable_row* r = s->head;
  printf("Current Symboltable:\n");
  while(r != NULL) {
    printf("\tname: %s\n", r->name);
    r = r->next;
  }
}

t_object* getSymbolByName(t_symboltable* s, t_object* o) {
  struct t_symboltable_row* r = s->head;
  while (r != NULL) {
    if (strcmp(r->name, o->value->value.s) == 0) {
      return r->object;
    }
    r = r->next;
  }
  exception("Object has no value",-1,o->value->value.s);
  return NULL;
}

void init_system() {
  clib_functions = newSymbolTable();
  init_core();
  init_lists();
  init_seq();
}
