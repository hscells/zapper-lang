#ifndef __OBJECTS_H
#define __OBJECTS_H

#include "types.h"

typedef struct symbol t_symbol;

typedef struct {
  t_symbol *method[512];
  int index;
} t_methodlist;

typedef struct {
  t_symbol *field[512];
  int index;
} t_fieldlist;

typedef struct {
  t_methodlist* methods;
  t_fieldlist* fields;
  int type;
  union generic value;
  int id;
} t_object;

struct symbol {
  int type;
  t_object* object;
  int id;
};

typedef struct {
  t_symbol symbols[1024];
} t_symboltable;

typedef struct {
  int index;
  t_object* object[1024];
} t_stack;

typedef struct {
  int index;
  t_stack* stack[1024];
} t_heap;

typedef struct {
  int method_id;
  int* signature;
  int return_type;
  char* ast;
} t_method;

int newObjectId();
t_methodlist* newMethodlist();
t_fieldlist* newFieldlist();
void addMethodToMethodlist(t_methodlist* methodlist, char* name, int type);
void addFieldToFieldlist(t_fieldlist* fieldlist, char* name, int type, t_object* value);
void addMethodlistToObject(t_object* object, t_methodlist* methodlist);
void addFieldlistToObject(t_object* object, t_fieldlist* fieldlist);
t_object* newObject();

t_symboltable* newSymbolTable();

void addSymbolToSymbolTable(t_symboltable symboltable, t_stack *stack, t_object *object);

t_heap* newHeap();
t_stack* newStack();

#endif
