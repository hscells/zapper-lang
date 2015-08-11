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
  switch(o->type) {
    case 0xff01:
      printf("%d\n", o->value.i);
      return;
    case 0xff03:
      printf("%c\n", o->value.c);
      return;
    case 0xff04:
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