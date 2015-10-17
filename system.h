#ifndef __SYSTEM_H
#define __SYSTEM_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "objects.h"
#include "types.h"
#include "expressions.h"

int LINE_NUMBER;
char* CURRENT_TOKEN;

struct t_symboltable_row{
  t_object* object;
  char* name;
  int id;
  struct t_symboltable_row* next;
};

typedef struct {
  struct t_symboltable_row* tail;
  struct t_symboltable_row* head;
  int current;
} t_symboltable;

t_symboltable *clib_functions;

t_generic* newGeneric();

t_object* z_print(t_list* arg);
t_object* z_println(t_list* arg);
t_object* z_read();

enum t_type z_typeof(t_object* o);
void exception(char* e, int line_num, char* token);
void z_exception(char* e);

t_object* z_add(t_list* args);
t_object* z_sub(t_list* args);
t_object* z_mul(t_list* args);
t_object* z_div(t_list* args);

t_object* z_teq(t_list* args);
t_object* z_eq(t_list* args);
t_object* z_lt(t_list* args);
t_object* z_gt(t_list* args);
t_object* z_lteq(t_list* args);
t_object* z_gteq(t_list* args);

t_object* z_list();
t_object* z_list_obj(t_list* list);
t_object* z_conj(t_list* list, t_object* o);
t_object* z_first(t_list* list);
t_object* z_rest(t_list* list);
t_object* z_nth(t_list* list, int index);
t_object* z_length(t_list* list);

t_object* z_int(int x);
t_object* z_string(char* x);

void init_system();
void init_core();
void init_lists();
void init_seq();

t_symboltable* newSymbolTable();
void addFunctionToSymbolTable(t_symboltable* symboltable, struct function* func);
struct function* getFunctionFromSymbolTable(t_symboltable* symboltable, char* name);
void addObjectToSymbolTable(t_symboltable* symboltable, t_object* symbol, t_object* object, struct node *node);
void printSymboltable(t_symboltable* symboltable);
bool inSymboltable(t_symboltable* s, char* name);

t_object* getSymbolByName(t_symboltable* symboltable, t_object* o);
t_list* getFunctionParams(t_symboltable* symboltable, t_object* o);

// eval is defined here because it relis on the symboltable object
// t_object* eval(t_ast *ast, t_symboltable* symboltable, int depth, struct node* node);
#endif
