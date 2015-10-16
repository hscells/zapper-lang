#include <strings.h>

#include "../../system.h"

void exception(char* e, int line_number, char* token) {
  printf("An Exception was raised on line: %d, near: '%s'\n\t%s\n", line_number, token, e);
  exit(2);
}

void z_exception(char* e) {
  printf("An Exception was raised on line: %d, near: '%s'\n\t%s\n", LINE_NUMBER, CURRENT_TOKEN, e);
  exit(2);
}


t_object* z_add(t_list* args) {
  t_object* a = z_nth(args,0);
  t_object* b = z_nth(args,1);
  t_object* result = newObject();
  if(z_typeof(a) == z_typeof(b) && z_typeof(a) == Int) {
    result->value->value = (t_generic_value) (a->value->value.i + b->value->value.i);
    result->value->type = Int;
  } else {
    exception("Type mismatch", -1, NULL);
  }
  return result;
}

t_object* z_sub(t_object* a, t_object* b) {
  t_object* result = newObject();
  if(z_typeof(a) == z_typeof(b) && z_typeof(a) == Int) {
    result->value->value = (t_generic_value) (a->value->value.i - b->value->value.i);
    result->value->type = Int;
  } else {
    exception("Type mismatch", -1, NULL);
  }
  return result;
}

t_object* z_mul(t_object* a, t_object* b) {
  t_object* result = newObject();
  if(z_typeof(a) == z_typeof(b) && z_typeof(a) == Int) {
    result->value->value = (t_generic_value) (a->value->value.i * b->value->value.i);
    result->value->type = Int;
  } else {
    exception("Type mismatch", -1, NULL);
  }
  return result;
}

t_object* z_div(t_object* a, t_object* b) {
  t_object* result = newObject();
  if(z_typeof(a) == z_typeof(b) && z_typeof(a) == Int) {
    result->value->value = (t_generic_value) (a->value->value.i / b->value->value.i);
    result->value->type = Int;
  } else {
    exception("Type mismatch", -1, NULL);
  }
  return result;
}

void z_print(t_object* o, t_symboltable* s) {
  if(o == NULL) {
    printf("undefined");
    return;
  }

  switch(o->value->type) {
    case Int:
      printf("%d", o->value->value.i);
      return;
    case Float:
      printf("%f", o->value->value.f);
      return;
    case Char:
      printf("%c", o->value->value.c);
      return;
    case String:
      printf("%s",o->value->value.s);
      return;
    case Bool:
      printf("%d", o->value->value.b);
      return;
    case List:
      printf("<List Object>");
      return;
    case Exception:
      printf("<Exception Object>");
      return;
    case Function:
      printf("<Function Object>");
      return;
    case Symbol:
      z_print(getSymbolByName(s, o),s);
      return;
  }
  printf("<object> @ %d", o->id);
}

void z_println(t_object* o, t_symboltable* s) {
  z_print(o,s);
  printf("\n");
}

t_object* z_teq(t_object* a, t_object* b) {
  t_object* result = newObject();
  result->value->type = Bool;
  result->value->value = (t_generic_value) (z_typeof(a) == z_typeof(b));
  return result;
}

t_object* z_eq(t_object* a, t_object* b) {
  t_object* result = newObject();
  result->value->type = Bool;
  if(z_typeof(a) == z_typeof(b)) {
    switch(z_typeof(a)) {
      case Int:
        result->value->value = (t_generic_value) (a->value->value.i == b->value->value.i);
        break;
      case Float:
        result->value->value = (t_generic_value) (a->value->value.f == b->value->value.f);
        break;
      case Char:
        result->value->value = (t_generic_value) (a->value->value.c == b->value->value.c);
        break;
      case String:
        result->value->value = (t_generic_value) (a->value->value.s == b->value->value.s);
        break;
      case Bool:
        result->value->value = (t_generic_value) (a->value->value.b == b->value->value.b);
        break;
      case List:
        exception("List comparison not yet implemented.",-1,NULL);
        break;
      case Exception:
        exception("Exception comparison not yet implemented.",-1,NULL);
        break;
      case Function:
        exception("Function comparison not yet implemented.",-1,NULL);
        break;
      case Symbol:
        exception("Symbol comparison not yet implemented.",-1,NULL);
        break;
    }
  } else {
    exception("Type mismatch", -1, NULL);
  }
  return result;
}

t_object* z_lt(t_object* a, t_object* b) {
  t_object* result = newObject();
  result->value->type = Bool;
  if(z_typeof(a) == z_typeof(b)) {
    switch(z_typeof(a)) {
      case Int:
        result->value->value = (t_generic_value) (a->value->value.i < b->value->value.i);
        break;
      case Float:
        result->value->value = (t_generic_value) (a->value->value.f < b->value->value.f);
        break;
      case Char:
        result->value->value = (t_generic_value) (a->value->value.c < b->value->value.c);
        break;
      case String:
        result->value->value = (t_generic_value) (a->value->value.s < b->value->value.s);
        break;
      case Bool:
        result->value->value = (t_generic_value) (a->value->value.b < b->value->value.b);
        break;
      case List:
        exception("List comparison not yet implemented.",-1,NULL);
        break;
      case Exception:
        exception("Exception comparison not yet implemented.",-1,NULL);
        break;
      case Function:
        exception("Function comparison not yet implemented.",-1,NULL);
        break;
      case Symbol:
        exception("Symbol comparison not yet implemented.",-1,NULL);
        break;
    }
  } else {
    exception("Type mismatch", -1, NULL);
  }
  return result;
}

t_object* z_gt(t_object* a, t_object* b) {
  t_object* result = newObject();
  result->value->type = Bool;
  if(z_typeof(a) == z_typeof(b)) {
    switch(z_typeof(a)) {
      case Int:
        result->value->value = (t_generic_value) (a->value->value.i > b->value->value.i);
        break;
      case Float:
        result->value->value = (t_generic_value) (a->value->value.f > b->value->value.f);
        break;
      case Char:
        result->value->value = (t_generic_value) (a->value->value.c > b->value->value.c);
        break;
      case String:
        result->value->value = (t_generic_value) (a->value->value.s > b->value->value.s);
        break;
      case Bool:
        result->value->value = (t_generic_value) (a->value->value.b > b->value->value.b);
        break;
      case List:
        exception("List comparison not yet implemented.",-1,NULL);
        break;
      case Exception:
        exception("Exception comparison not yet implemented.",-1,NULL);
        break;
      case Function:
        exception("Function comparison not yet implemented.",-1,NULL);
        break;
      case Symbol:
        exception("Symbol comparison not yet implemented.",-1,NULL);
        break;
    }
  } else {
    exception("Type mismatch", -1, NULL);
  }
  return result;  return result;
}

t_object* z_lteq(t_object* a, t_object* b) {
  t_object* result = newObject();
  result->value->type = Bool;
  if(z_typeof(a) == z_typeof(b)) {
    switch(z_typeof(a)) {
      case Int:
        result->value->value = (t_generic_value) (a->value->value.i <= b->value->value.i);
        break;
      case Float:
        result->value->value = (t_generic_value) (a->value->value.f <= b->value->value.f);
        break;
      case Char:
        result->value->value = (t_generic_value) (a->value->value.c <= b->value->value.c);
        break;
      case String:
        result->value->value = (t_generic_value) (a->value->value.s <= b->value->value.s);
        break;
      case Bool:
        result->value->value = (t_generic_value) (a->value->value.b <= b->value->value.b);
        break;
      case List:
        exception("List comparison not yet implemented.",-1,NULL);
        break;
      case Exception:
        exception("Exception comparison not yet implemented.",-1,NULL);
        break;
      case Function:
        exception("Function comparison not yet implemented.",-1,NULL);
        break;
      case Symbol:
        exception("Symbol comparison not yet implemented.",-1,NULL);
        break;
    }
  } else {
    exception("Type mismatch", -1, NULL);
  }
  return result;
}

t_object* z_gteq(t_object* a, t_object* b){
  t_object* result = newObject();
  result->value->type = Bool;
  if(z_typeof(a) == z_typeof(b)) {
    switch(z_typeof(a)) {
      case Int:
        result->value->value = (t_generic_value) (a->value->value.i >= b->value->value.i);
        break;
      case Float:
        result->value->value = (t_generic_value) (a->value->value.f >= b->value->value.f);
        break;
      case Char:
        result->value->value = (t_generic_value) (a->value->value.c >= b->value->value.c);
        break;
      case String:
        result->value->value = (t_generic_value) (a->value->value.s >= b->value->value.s);
        break;
      case Bool:
        result->value->value = (t_generic_value) (a->value->value.b >= b->value->value.b);
        break;
      case List:
        exception("List comparison not yet implemented.",-1,NULL);
        break;
      case Exception:
        exception("Exception comparison not yet implemented.",-1,NULL);
        break;
      case Function:
        exception("Function comparison not yet implemented.",-1,NULL);
        break;
      case Symbol:
        exception("Symbol comparison not yet implemented.",-1,NULL);
        break;
    }
  } else {
    exception("Type mismatch", -1, NULL);
  }
  return result;
}

void z_exit() {
  exit(1);
}

void init_core() {
  t_object* (*add)(t_list* args) = &z_add;
  addFunctionToSymbolTable(clib_functions, add, 2, "+");
}
