#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "objects.h"
#include "types.h"

int LINE_NUMBER;
char* CURRENT_TOKEN;

struct atom {
  t_object* value;
  struct atom* next;
};

typedef struct {
  struct atom* head;
  struct atom* atom;
} t_list;

struct t_symboltable_row{
  t_object* object;
  char* name;
  int id;
  struct node* node;
  t_list* formal_parameters;
  struct t_symboltable_row* next;
} ;

typedef struct {
  struct t_symboltable_row* tail;
  struct t_symboltable_row* head;
  t_stack* stack;
  int current;
} t_symboltable;

t_symboltable *symboltable;
struct node *native_code;

t_generic* newGeneric();

void z_print(t_object* o);
void z_println(t_object* o);
t_generic* z_read();

enum t_type z_typeof(t_object* o);
void exception(char* e, int line_num, char* token);
void z_exception(char* e);

t_generic* z_add(t_generic* a, t_generic* b);
t_generic* z_sub(t_generic* a, t_generic* b);
t_generic* z_mul(t_generic* a, t_generic* b);
t_generic* z_div(t_generic* a, t_generic* b);

t_generic* z_teq(t_object* a, t_object* b);
t_generic* z_eq(t_object* a, t_object* b);
t_generic* z_lt(t_object* a, t_object* b);
t_generic* z_gt(t_object* a, t_object* b);
t_generic* z_lteq(t_object* a, t_object* b);
t_generic* z_gteq(t_object* a, t_object* b);

t_list* z_list();
void z_conj(t_list* list, t_object* o);
t_object* z_first(t_list* list);
t_list* z_rest(t_list* list);
t_object* z_nth(t_list* list, int index);
int z_length(t_list* list);

t_object* z_int(int x);

t_generic* z_eval(char* expressions, t_stack* stack, t_heap* heap);

t_symboltable* newSymbolTable();
void addFunctionToSymbolTable(t_symboltable* symboltable, char*, struct node *node, t_list* formal_parameters);
void addObjectToSymbolTable(t_symboltable* symboltable, char* name, t_object* object, struct node *node);
void printSymboltable(t_symboltable* symboltable);
t_object* getSymbolByName(t_symboltable* symboltable, char* name);
