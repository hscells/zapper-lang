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
int CRASH_ON_EXCEPTION;
char* NAMESPACE;

char* ZAPPER_LIB_PATH;

struct symboltable_row_t {
  object_t* object;
  char* name;
  char* fqn;
  int id;
  struct symboltable_row_t* next;
};

typedef struct {
  struct symboltable_row_t* tail;
  struct symboltable_row_t* head;
  int current;
} symboltable_t;

symboltable_t *clib_functions;
symboltable_t *globals;
symboltable_t *imports;

generic_t* newGeneric();

object_t* z_print(list_t* arg);
object_t* z_println(list_t* arg);
object_t* z_read();

enum t_type z_typeof(object_t* o);
void exception(char* e, int line_num, char* token);
void z_exception(char* e);

object_t* z_add(list_t* args);
object_t* z_sub(list_t* args);
object_t* z_mul(list_t* args);
object_t* z_div(list_t* args);

object_t* z_teq(list_t* args);
object_t* z_eq(list_t* args);
object_t* z_lt(list_t* args);
object_t* z_gt(list_t* args);
object_t* z_lteq(list_t* args);
object_t* z_gteq(list_t* args);

object_t* z_list();
object_t* z_list_obj(list_t* list);
object_t* z_conj(list_t* list, object_t* o);
object_t* z_first(list_t* list);
object_t* z_rest(list_t* list);
object_t* z_nth(list_t* list, int index);
object_t* z_length(list_t* list);

object_t* z_int(int x);
object_t* z_string(char* x);

void init_system();
void init_core();
void inilist_ts();
void init_io();

symboltable_t* newSymbolTable();
void addFunctionToSymbolTable(symboltable_t* symboltable, struct function* func);
void addObjectToSymbolTable(symboltable_t* s, object_t* symbol, object_t* object);
struct function* getFunctionFromSymbolTable(symboltable_t* symboltable, char* name, int actual_parameter_count);
void printSymboltable(symboltable_t* symboltable);
bool inSymboltable(symboltable_t* s, char* name);
struct symboltable_row_t* symboltableRow(symboltable_t* s, char* name);
int getFunctionParamCount(symboltable_t* s, char* name);

object_t* getSymbolByName(symboltable_t* symboltable, char* name);
list_t* getFunctionParams(symboltable_t* symboltable, object_t* o);

object_t* eval(list_t* ast, symboltable_t* context);
object_t* call(struct function* func, list_t* args, symboltable_t* context);
#endif
