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

t_object* z_sub(t_list* args) {
  t_object* a = z_nth(args,0);
  t_object* b = z_nth(args,1);
  t_object* result = newObject();
  if(z_typeof(a) == z_typeof(b) && z_typeof(a) == Int) {
    result->value->value = (t_generic_value) (a->value->value.i - b->value->value.i);
    result->value->type = Int;
  } else {
    exception("Type mismatch", -1, NULL);
  }
  return result;
}

t_object* z_mul(t_list* args) {
  t_object* a = z_nth(args,0);
  t_object* b = z_nth(args,1);
  t_object* result = newObject();
  if(z_typeof(a) == z_typeof(b) && z_typeof(a) == Int) {
    result->value->value = (t_generic_value) (a->value->value.i * b->value->value.i);
    result->value->type = Int;
  } else {
    exception("Type mismatch", -1, NULL);
  }
  return result;
}

t_object* z_div(t_list* args) {
  t_object* a = z_nth(args,0);
  t_object* b = z_nth(args,1);
  t_object* result = newObject();
  if(z_typeof(a) == z_typeof(b) && z_typeof(a) == Int) {
    result->value->value = (t_generic_value) (a->value->value.i / b->value->value.i);
    result->value->type = Int;
  } else {
    exception("Type mismatch", -1, NULL);
  }
  return result;
}

t_object* z_print(t_list* args) {
  t_object* obj = newObject();
  t_object* o = z_first(args);
  if(o == NULL) {
    printf("undefined");
    return obj;
  }
  printf("type: %d\n", o->value->type);

  switch(o->value->type) {
    case Int:
      printf("%d", o->value->value.i);
      return obj;
    case Float:
      printf("%f", o->value->value.f);
      return obj;
    case Char:
      printf("%c", o->value->value.c);
      return obj;
    case String:
      printf("%s",o->value->value.s);
      return obj;
    case Bool:
      printf("%d", o->value->value.i);
      printf("%d", o->value->value.b);
      return obj;
    case List:
      printf("<List Object>");
      return obj;
    case Exception:
      printf("<Exception Object>");
      return obj;
    case Function:
      printf("<Function Object>");
      return obj;
    case Symbol:
      // z_print(getSymbolByName(s, o),s);
      return obj;
  }
  printf("<object> @ %d", o->id);
}

t_object* z_println(t_list* args) {
  z_print(args);
  printf("\n");
  return newObject();
}

t_object* z_teq(t_list* args) {
  t_object* a = z_nth(args,0);
  t_object* b = z_nth(args,1);
  t_object* result = newObject();
  result->value->type = Bool;
  result->value->value = (t_generic_value) (z_typeof(a) == z_typeof(b));
  return result;
}

t_object* z_eq(t_list* args) {
  t_object* a = z_nth(args,0);
  t_object* b = z_nth(args,1);
  t_object* result = newObject();
  result->value->type = Bool;
  if(z_typeof(a) == z_typeof(b)) {
    switch(z_typeof(a)) {
      case Int:
        result->value->value.b = (a->value->value.i == b->value->value.i);
        break;
      case Float:
        result->value->value.b = (a->value->value.f == b->value->value.f);
        break;
      case Char:
        result->value->value.b = (a->value->value.c == b->value->value.c);
        break;
      case String:
        result->value->value.b = strcmp(a->value->value.s, b->value->value.s);
        break;
      case Bool:
        result->value->value.b = (a->value->value.b == b->value->value.b);
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

t_object* z_lt(t_list* args) {
  t_object* a = z_nth(args,0);
  t_object* b = z_nth(args,1);
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

t_object* z_gt(t_list* args) {
  t_object* a = z_nth(args,0);
  t_object* b = z_nth(args,1);
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

t_object* z_lteq(t_list* args) {
  t_object* a = z_nth(args,0);
  t_object* b = z_nth(args,1);
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

t_object* z_gteq(t_list* args){
  t_object* a = z_nth(args,0);
  t_object* b = z_nth(args,1);
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

t_object* z_exit(t_list* args) {
  if (z_nth(args, 0)->value->type == Int) {
    exit(z_nth(args, 0)->value->value.i);
  } else {
    exit(2);
  }
}

t_object* z_fn(t_list* args) {
  char* name = z_nth(args, 0)->value->value.s;
  t_list* _args = z_nth(args, 1)->value->value.l;
  t_list* body = z_nth(args, 2)->value->value.l;
  int params = z_length(_args)->value->value.i;
  t_object* obj = newObject();
  obj->value->value = (t_generic_value) newZFunction(name, _args, body, params);
  addFunctionToSymbolTable(globals, obj->value->value.function);
  obj->value->type = Function;
  return obj;
}

t_object* z_let(t_list* args) {
  t_object* symbol = z_nth(args, 0);
  t_object* obj = z_nth(args, 1);
  addObjectToSymbolTable(globals, symbol, obj);
  return obj;
}

t_object* z_return(t_list* args) {
  return z_nth(args, 0);
}

t_object* z_cond_placeholder(t_list* args) {
  return newObject();
}

void init_core() {

  t_object* (*fn)(t_list* args) = &z_fn;
  struct function* fn_ref = newFunction(fn,"fn",3);
  addFunctionToSymbolTable(clib_functions, fn_ref);

  t_object* (*let)(t_list* args) = &z_let;
  struct function* let_ref = newFunction(let,"let",2);
  addFunctionToSymbolTable(clib_functions, let_ref);

  t_object* (*_return)(t_list* args) = &z_return;
  struct function* return_ref = newFunction(_return,"cond",0);
  addFunctionToSymbolTable(clib_functions, return_ref);

  t_object* (*cond)(t_list* args) = &z_cond_placeholder;
  struct function* cond_ref = newFunction(cond,"return",1);
  addFunctionToSymbolTable(clib_functions, cond_ref);

  t_object* (*add)(t_list* args) = &z_add;
  struct function* add_ref = newFunction(add,"+",2);
  addFunctionToSymbolTable(clib_functions, add_ref);

  t_object* (*sub)(t_list* args) = &z_sub;
  struct function* sub_ref = newFunction(sub,"-",2);
  addFunctionToSymbolTable(clib_functions, sub_ref);

  t_object* (*div)(t_list* args) = &z_div;
  struct function* div_ref = newFunction(div,"/",2);
  addFunctionToSymbolTable(clib_functions, div_ref);

  t_object* (*mul)(t_list* args) = &z_mul;
  struct function* mul_ref = newFunction(mul,"*",2);
  addFunctionToSymbolTable(clib_functions, mul_ref);

  t_object* (*eq)(t_list* args) = &z_eq;
  struct function* eq_ref = newFunction(eq,"=",2);
  addFunctionToSymbolTable(clib_functions, eq_ref);

  t_object* (*lt)(t_list* args) = &z_lt;
  struct function* lt_ref = newFunction(lt,"<",2);
  addFunctionToSymbolTable(clib_functions, lt_ref);

  t_object* (*gt)(t_list* args) = &z_gt;
  struct function* gt_ref = newFunction(gt,">",2);
  addFunctionToSymbolTable(clib_functions, gt_ref);

  t_object* (*lteq)(t_list* args) = &z_lteq;
  struct function* lteq_ref = newFunction(lteq,"<=",2);
  addFunctionToSymbolTable(clib_functions, lteq_ref);

  t_object* (*gteq)(t_list* args) = &z_gteq;
  struct function* gteq_ref = newFunction(gteq,">=",2);
  addFunctionToSymbolTable(clib_functions, gteq_ref);

  t_object* (*_exit)(t_list* args) = &z_exit;
  struct function* exit_ref = newFunction(_exit,"exit",1);
  addFunctionToSymbolTable(clib_functions, exit_ref);

  t_object* (*print)(t_list* args) = &z_print;
  struct function* print_ref = newFunction(print,"print",1);
  addFunctionToSymbolTable(clib_functions, print_ref);

  t_object* (*println)(t_list* args) = &z_println;
  struct function* println_ref = newFunction(println,"println",1);
  addFunctionToSymbolTable(clib_functions, println_ref);


}
