#include <string.h>
#include "system.h"

generic_t* newGeneric() {
  return (generic_t*) malloc(sizeof(generic_t));
}

enum t_type z_typeof(object_t* o) {
  return o->value->type;
}

object_t* z_read() {
  object_t* result = newObject();
  return result;
}

object_t* z_int(int x) {
  object_t* obj = newObject();
  obj->value->type = Int;
  obj->value->value.i = x;
  return obj;
}

object_t* z_string(char* x) {
  object_t* obj = newObject();
  obj->value->type = String;
  obj->value->value.s = x;
  return obj;
}

symboltable_t* newSymbolTable() {
  symboltable_t *s = (symboltable_t*) malloc(sizeof(symboltable_t));
  s->head = NULL;
  s->tail = NULL;
  return s;
}

void addFunctionToSymbolTable(symboltable_t* s, struct function* func) {
  struct symboltable_row_t* row = (struct symboltable_row_t*) malloc(sizeof(struct symboltable_row_t));
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

struct function* getFunctionFromSymbolTable(symboltable_t* s, char* name, int actual_parameter_count) {
  struct symboltable_row_t* r = s->head;
  while (r != NULL) {
    if (strcmp(r->name, name) == 0 && (r->object->value->value.function->params == -1 || r->object->value->value.function->params == actual_parameter_count)) {
      return r->object->value->value.function;
    }
    r = r->next;
  }
  exception("Function not in this symboltable",-1,name);
  return NULL;
}

void addObjectToSymbolTable(symboltable_t* s, object_t* symbol, object_t* object) {
  struct symboltable_row_t* row = (struct symboltable_row_t*) malloc(sizeof(struct symboltable_row_t));
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

bool inSymboltable(symboltable_t* s, char* name) {
  struct symboltable_row_t* r = s->head;
  while(r != NULL) {
    if (strcmp(r->name, name) == 0) {
      return true;
    }
    r = r->next;
  }
  return false;
}

void printSymboltable(symboltable_t* s) {
  struct symboltable_row_t* r = s->head;
  printf("Current Symboltable:\n");
  while(r != NULL) {
    printf("\tname: %s\n", r->name);
    r = r->next;
  }
}

object_t* getSymbolByName(symboltable_t* s, char* name) {
  struct symboltable_row_t* r = s->head;
  while (r != NULL) {
    if (strcmp(r->name, name) == 0) {
      return r->object;
    }
    r = r->next;
  }
  exception("Object has no value",-1,name);
  return NULL;
}

void init_system() {
  clib_functions = newSymbolTable();
  globals = newSymbolTable();
  imports = newSymbolTable();

  object_t* z_True = newObject();
  object_t* z_True_symbol = newObject();
  object_t* z_False = newObject();
  object_t* z_False_symbol = newObject();

  z_True->value->value.b = true;
  z_True->value->type = Bool;
  z_True_symbol->value->value.s = "True";
  z_True_symbol->value->type = Symbol;

  z_False->value->value.b = false;
  z_False->value->type = Bool;
  z_False_symbol->value->value.s = "False";
  z_False_symbol->value->type = Symbol;

  addObjectToSymbolTable(globals, z_True_symbol, z_True);
  addObjectToSymbolTable(globals, z_False_symbol, z_False);

  init_core();
  inilist_ts();
  // init_seq();
}
