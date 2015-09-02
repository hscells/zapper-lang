#include "system.h"
#include <string.h>

void exception(char* e, int line_number, char* token) {
  printf("An Exception was raised on line: %d, near: '%s'\n\t%s\n", line_number, token, e);
  exit(2);
}

void z_exception(char* e) {
  printf("An Exception was raised on line: %d, near: '%s'\n\t%s\n", LINE_NUMBER, CURRENT_TOKEN, e);
  exit(2);
}

t_generic z_add(t_generic a, t_generic b) {
  t_generic result;
  result.value = (t_generic_value) (a.value.i + b.value.i);
  result.type = Int;
  return result;
}

t_generic z_sub(t_generic a, t_generic b) {
  t_generic result;
  result.value = (t_generic_value) (a.value.i - b.value.i);
  result.type = Int;
  return result;
}

t_generic z_mul(t_generic a, t_generic b) {
  t_generic result;
  result.value = (t_generic_value) (a.value.i * b.value.i);
  result.type = Int;
  return result;
}

t_generic z_div(t_generic a, t_generic b) {
  t_generic result;
  result.value = (t_generic_value) (a.value.i / b.value.i);
  result.type = Int;
  return result;
}

void z_print(t_object* o) {
  switch(o->value.type) {
    case Int:
      printf("%d", o->value.value.i);
      return;
    case Float:
      printf("%f", o->value.value.f);
      return;
    case Char:
      printf("%c", o->value.value.c);
      return;
    case String:
      printf("%s", o->value.value.s);
      return;
    case Bool:
      break;
    case List:
      break;
  }
  printf("<object> @ %d", o->id);
}

void z_println(t_object* o) {
  z_print(o);
  printf("\n");
}

enum t_type z_typeof(t_object* o) {
  return o->value.type;
}

t_generic z_teq(t_object* a, t_object* b) {
  t_generic result;
  result.type = Bool;
  result.value = (t_generic_value) (z_typeof(a) == z_typeof(b));
  return result;
}

t_generic z_eq(t_object* a, t_object* b) {
  t_generic result;
  result.type = Bool;
  result.value = (t_generic_value) false;
  switch(a->value.type) {
    case Int:
      result.value = (t_generic_value) (a->value.value.i == b->value.value.i);
      break;
    case Float:
      result.value = (t_generic_value) (a->value.value.f == b->value.value.f);
      break;
    case Char:
      result.value = (t_generic_value) (a->value.value.c == b->value.value.c);
      break;
    case String:
      result.value = (t_generic_value) (strcmp(a->value.value.s,b->value.value.s) == 0);
      break;
    case Bool:
      break;
    case List:
      break;
  }
  return result;
}
t_generic z_lt(t_object* a, t_object* b) {
  t_generic result;
  result.type = Bool;
  result.value = (t_generic_value) false;
  switch(a->value.type) {
    case Int:
      result.value = (t_generic_value) (a->value.value.i < b->value.value.i);
      break;
    case Float:
      result.value = (t_generic_value) (a->value.value.f < b->value.value.f);
      break;
    case Char:
      result.value = (t_generic_value) (a->value.value.c < b->value.value.c);
      break;
    case String:
      result.value = (t_generic_value) (strcmp(a->value.value.s,b->value.value.s) < 0);
      break;
    case Bool:
      break;
    case List:
      break;
  }
  return result;
}
t_generic z_gt(t_object* a, t_object* b) {
  t_generic result;
  result.type = Bool;
  result.value = (t_generic_value) false;
  switch(a->value.type) {
    case Int:
      result.value = (t_generic_value) (a->value.value.i > b->value.value.i);
      break;
    case Float:
      result.value = (t_generic_value) (a->value.value.f > b->value.value.f);
      break;
    case Char:
      result.value = (t_generic_value) (a->value.value.c > b->value.value.c);
      break;
    case String:
      result.value = (t_generic_value) (strcmp(a->value.value.s,b->value.value.s) > 0);
      break;
    case Bool:
      break;
    case List:
      break;
  }
  return result;
}
t_generic z_lteq(t_object* a, t_object* b) {
  t_generic result;
  result.type = Bool;
  result.value = (t_generic_value) false;
  switch(a->value.type) {
    case Int:
      result.value = (t_generic_value) (a->value.value.i <= b->value.value.i);
      break;
    case Float:
      result.value = (t_generic_value) (a->value.value.f <= b->value.value.f);
      break;
    case Char:
      result.value = (t_generic_value) (a->value.value.c <= b->value.value.c);
      break;
    case String:
      result.value = (t_generic_value) (strcmp(a->value.value.s,b->value.value.s) <= 0);
      break;
    case Bool:
      break;
    case List:
      break;
  }
  return result;
}

t_generic z_gteq(t_object* a, t_object* b){
  t_generic result;
  result.type = Bool;
  result.value = (t_generic_value) false;
  switch(a->value.type) {
    case Int:
      result.value = (t_generic_value) (a->value.value.i >= b->value.value.i);
      break;
    case Float:
      result.value = (t_generic_value) (a->value.value.f >= b->value.value.f);
      break;
    case Char:
      result.value = (t_generic_value) (a->value.value.c >= b->value.value.c);
      break;
    case String:
      result.value = (t_generic_value) (strcmp(a->value.value.s,b->value.value.s) >= 0);
      break;
    case Bool:
      break;
    case List:
      break;
  }
  return result;
}

void z_exit() {
  exit(1);
}

t_generic z_read() {
  t_generic result;
  return result;
}

// below are list operations

/**
 * creates a new list
 * @return a new list
 */
t_list* z_list() {
  t_list* list = (t_list*) malloc(sizeof(t_list));
  list->head = NULL;
  return list;
}

/**
 * (conj)-oin a list with another object
 * @param list the list being conjoined onto
 * @param o    the object which is being conjoined
 */
void z_conj(t_list* list, t_object* o) {
  struct atom* atom = (struct atom*) malloc(sizeof(struct atom));
  atom->value = o;
  if (list->head == NULL) {
    list->head = atom;
    list->atom = atom;
  } else {
    list->atom->next = atom;
  }
}

/**
 * gets the first item from a list
 * @param  list the list being looked at
 * @return      the object at the very start of the list
 */
t_object* z_first(t_list* list) {
  return list->head->value;
}

/**
 * gets everything but the first element from a list
 * @param  list the list being looked at
 * @return      everything except the first item in the list
 */
t_list* z_rest(t_list* list) {
  struct atom* atom = list->head->next;
  t_list* new_list = z_list();
  new_list->head = atom;
  return new_list;
}


t_object* newSystem(t_stack* stack, t_heap* heap) {
  t_fieldlist* fields = newFieldlist();
  t_methodlist* methods = newMethodlist();
  t_object* System = newObject(methods, fields);
  return System;
}

t_object* newInt(int v, t_stack* stack, t_heap* heap) {
  t_object* Int = newObject();
  Int->value.value = (t_generic_value) v;
  Int->value.type = (enum t_type) Int;
  return Int;
}

t_object* newChar(char c, t_stack* stack, t_heap* heap) {
  t_object* Char = newObject();
  Char->value.value = (t_generic_value) c;
  Char->value.type = (enum t_type) Char;
  return Char;
}

t_object* newString(char* s, t_stack* stack, t_heap* heap) {
  t_object* String = newObject();
  String->value.value = (t_generic_value) s;
  String->value.type = (enum t_type) String;
  return String;
}

t_symboltable* newSymbolTable(t_stack* stack) {
  t_symboltable *s = (t_symboltable*) malloc(sizeof(t_symboltable));
  s->stack = stack;
  return s;
}

void addFunctionToSymbolTable(t_symboltable* symboltable, int id, struct node *node, enum t_type* formal_parameters[]) {
  t_symboltable_row* row = (t_symboltable_row*) malloc(sizeof(t_symboltable_row));
  row->id = id;
  row->node = node;
  symboltable->row[symboltable->current++] = row;
}
