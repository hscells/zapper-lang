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
  z_exception("Could not print this object.",0,0);
}

void z_typeof(t_object* o) {
  printf("%d", o->type);
}

void z_exit() {
  exit(1);
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
