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

typedef struct {
  int id;
  char* token;
  struct node* node;
  enum t_type* formal_parameters[];
} t_symboltable_row;

typedef struct {
  t_symboltable_row* row[0xb00];
  t_stack* stack;
  int current;
} t_symboltable;

t_symboltable *symboltable;
struct node *native_code;

void z_print(t_object* o);
void z_println(t_object* o);
t_generic z_read();

enum t_type z_typeof(t_object* o);
void exception(char* e, int line_num, char* token);
void z_exception(char* e);

t_generic z_add(t_generic a, t_generic b);
t_generic z_sub(t_generic a, t_generic b);
t_generic z_mul(t_generic a, t_generic b);
t_generic z_div(t_generic a, t_generic b);

t_generic z_teq(t_object* a, t_object* b);
t_generic z_eq(t_object* a, t_object* b);
t_generic z_lt(t_object* a, t_object* b);
t_generic z_gt(t_object* a, t_object* b);
t_generic z_lteq(t_object* a, t_object* b);
t_generic z_gteq(t_object* a, t_object* b);

t_list* z_list();
void z_conj(t_list* list, t_object* o);
t_object* z_first(t_list* list);
t_list* z_rest(t_list* list);

t_symboltable* newSymbolTable(t_stack* stack);
void addFunctionToSymbolTable(t_symboltable* symboltable, int id, struct node *node, enum t_type formal_parameters[]);
