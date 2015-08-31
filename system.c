#include "system.h"

void z_exception(char* e, int l, int c) {
  printf("Line: %d, Position: %d\n%s\n", l, c, e);
  exit(2);
}

int z_add(int a, int b) {
  return a + b;
}

int z_sub(int a, int b) {
  return a - b;
}

int z_mul(int a, int b) {
  return a * b;
}

int z_div(int a, int b) {
  return a / b;
}

void z_print(t_object* o) {
  switch(o->type) {
    case 0xff01:
      printf("%d", o->value.i);
      return;
    case 0xff03:
      printf("%c", o->value.c);
      return;
    case 0xff04:
      printf("%s", o->value.s);
      return;
  }
  printf("<object> @ %d", o->id);
}

void z_println(t_object* o) {
  z_print(o);
  printf("\n");
}

int z_typeof(t_object* o) {
  return o->type;
}

void z_exit() {
  exit(1);
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
  Int->value = (union generic)v;
  Int->type = Type->Int;
  return Int;
}

t_object* newChar(char c, t_stack* stack, t_heap* heap) {
  t_object* Char = newObject();
  Char->value = (union generic)c;
  Char->type = Type->Char;
  return Char;
}

t_object* newString(char* s, t_stack* stack, t_heap* heap) {
  t_object* String = newObject();
  String->value = (union generic)s;
  String->type = Type->String;
  return String;
}
