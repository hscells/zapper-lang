#include "system.h"

void z_exception(char* e, int l, int c) {

  printf("Line: %d, Position: %d\n%s\n", l, c, e);
  exit(1);
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

void z_println(t_object* o) {
  switch(o->type->type) {
    case 0:
      printf("%d\n", o->value.i);
      return;
    case 1:
      printf("%c\n", o->value.c);
      return;
    case 2:
      printf("%s\n", o->value.s);
      return;
  }
  z_exception("Could not print this object.",0,0);
}

t_object* newSystem(t_stack* stack, t_heap* heap) {
  t_fieldlist* fields = newFieldlist();
  t_methodlist* methods = newMethodlist();
  t_object* System = newObject(methods, fields);
  return System;
}

t_object* newInt(int v, t_stack* stack, t_heap* heap) {
  t_fieldlist* fields = newFieldlist();
  t_methodlist* methods = newMethodlist();
  t_object* Int = newObject(methods, fields);
  Int->value = (union generic)v;
  Int->type->type = 0;
  return Int;
}

t_object* newChar(char c, t_stack* stack, t_heap* heap) {
  t_fieldlist* fields = newFieldlist();
  t_methodlist* methods = newMethodlist();
  t_object* Char = newObject(methods, fields);
  Char->value = (union generic)c;
  Char->type->type = 1;
  return Char;
}

t_object* newString(char* s, t_stack* stack, t_heap* heap) {
  t_fieldlist* fields = newFieldlist();
  t_methodlist* methods = newMethodlist();
  t_object* String = newObject(methods, fields);
  String->value = (union generic)s;
  String->type->type = 2;
  return String;
}
