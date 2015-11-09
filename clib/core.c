#include <strings.h>

#include "../system.h"

object_t* exception(char* e, int line_number, char* token) {
  printf("An Exception was raised on line: %d, near: '%s'\n\t%s\n", line_number, token, e);
  if (CRASH_ON_EXCEPTION) {
    exit(2);
  }
  object_t* o = newObject();
  o->value->value.s = token;
  o->value->type = Exception;
  return o;
}

object_t* z_exception(char* e) {
  printf("An Exception was raised on line: %d, near: '%s'\n\t%s\n", LINE_NUMBER, CURRENT_TOKEN, e);
  if (CRASH_ON_EXCEPTION) {
    exit(2);
  }
  object_t* o = newObject();
  o->value->value.s = CURRENT_TOKEN;
  o->value->type = Exception;
  return o;
}


object_t* z_add(list_t* args) {
  object_t* a = z_nth(args,0);
  object_t* b = z_nth(args,1);
  object_t* result = newObject();
  if(z_typeof(a) == z_typeof(b) && z_typeof(a) == Int) {
    result->value->value = (generic_value_t) (a->value->value.i + b->value->value.i);
    result->value->type = Int;
  } else if (z_typeof(a) == z_typeof(b) && z_typeof(a) == String) {
    char* str1 = a->value->value.s;
    char* str2 = b->value->value.s;
    char* newstr = (char*) malloc(1 + strlen(str1) + strlen(str2));
    strcpy(newstr, str1);
    strcat(newstr, str2);
    result->value->value.s = newstr;
    result->value->type = String;
  } else if (z_typeof(a) == z_typeof(b) && z_typeof(a) == Char) {
    char c1 = a->value->value.c;
    char c2 = b->value->value.c;
    char* newstr = (char*) malloc(1 + sizeof(char)*2);
    strncpy(newstr, &c1, 1);
    strncat(newstr, &c2, 1);
    result->value->value.s = newstr;
    result->value->type = String;
  } else if (z_typeof(a) == String && z_typeof(b) == Char) {
    char* str = a->value->value.s;
    char c = b->value->value.c;
    char* newstr = (char*) malloc(1 + sizeof(char) + strlen(str));
    strncpy(newstr, str, strlen(str));
    strncat(newstr, &c, 1);
    result->value->value.s = newstr;
    result->value->type = String;
  } else if (z_typeof(b) == String && z_typeof(a) == Char) {
    char* str = b->value->value.s;
    char c = a->value->value.c;
    char* newstr = (char*) malloc(1 + sizeof(char) + strlen(str));
    strncpy(newstr, &c, 1);
    strncat(newstr, str, strlen(str));
    result->value->value.s = newstr;
    result->value->type = String;
  } else {
    exception("Type mismatch", -1, NULL);
  }
  return result;
}

object_t* z_sub(list_t* args) {
  object_t* a = z_nth(args,0);
  object_t* b = z_nth(args,1);
  object_t* result = newObject();
  if(z_typeof(a) == z_typeof(b) && z_typeof(a) == Int) {
    result->value->value = (generic_value_t) (a->value->value.i - b->value->value.i);
    result->value->type = Int;
  } else {
    exception("Type mismatch", -1, NULL);
  }
  return result;
}

object_t* z_mul(list_t* args) {
  object_t* a = z_nth(args,0);
  object_t* b = z_nth(args,1);
  object_t* result = newObject();
  if(z_typeof(a) == z_typeof(b) && z_typeof(a) == Int) {
    result->value->value = (generic_value_t) (a->value->value.i * b->value->value.i);
    result->value->type = Int;
  } else {
    exception("Type mismatch", -1, NULL);
  }
  return result;
}

object_t* z_div(list_t* args) {
  object_t* a = z_nth(args,0);
  object_t* b = z_nth(args,1);
  object_t* result = newObject();
  if(z_typeof(a) == z_typeof(b) && z_typeof(a) == Int) {
    result->value->value = (generic_value_t) (a->value->value.i / b->value->value.i);
    result->value->type = Int;
  } else {
    exception("Type mismatch", -1, NULL);
  }
  return result;
}

object_t* z_print(list_t* args) {
  object_t* obj = newObject();
  object_t* o = z_first(args);
  struct atom *atom;
  if(o == NULL) {
    printf("undefined");
    return obj;
  }

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
      if(o->value->value.b) {
        printf("True");
      } else {
        printf("False");
      }
      return obj;
    case List:
      atom = o->value->value.l->head;
      printf("( ");
      while (atom != NULL) {
        object_t *l = z_list();
        z_conj(l->value->value.l, atom->value);
        z_print(l->value->value.l);
        printf(" ");
        atom = atom->next;
      }
      printf(")");
      return obj;
    case Exception:
      printf("<Exception Object>");
      return obj;
    case Function:
      if (o->value->value.function->body != NULL) {
        printf("<Function Object> ( %s ", o->value->value.function->name);
        atom = o->value->value.function->args->head;
        printf("( ");
        while (atom != NULL) {
          object_t *l = z_list();
          z_conj(l->value->value.l, atom->value);
          z_print(l->value->value.l);
          printf(" ");
          atom = atom->next;
        }
        printf(") ");
        atom = o->value->value.function->body->head;
        printf("( ");
        while (atom != NULL) {
          object_t *l = z_list();
          z_conj(l->value->value.l, atom->value);
          z_print(l->value->value.l);
          printf(" ");
          atom = atom->next;
        }
        printf(") )");
        printf("args: %d", z_length(o->value->value.function->args)->value->value.i);
      } else {
        printf("<Native Function Object>");
      }
      return obj;
    case FunctionReference:
      printf("%s",o->value->value.s);
      return obj;
    case Symbol:
      printf("%s", o->value->value.s);
      // z_print(getSymbolByName(s, o),s);
      return obj;
  }
  printf("<object> @ %d", o->id);
}

object_t* z_println(list_t* args) {
  z_print(args);
  printf("\n");
  return newObject();
}

object_t* z_teq(list_t* args) {
  object_t* a = z_nth(args,0);
  object_t* b = z_nth(args,1);
  object_t* result = newObject();
  result->value->type = Bool;
  result->value->value = (generic_value_t) (z_typeof(a) == z_typeof(b));
  return result;
}

object_t* z_eq(list_t* args) {
  object_t* a = z_nth(args,0);
  object_t* b = z_nth(args,1);
  object_t* result = newObject();
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
      case FunctionReference:
        exception("Function Reference comparison not yet implemented.",-1,NULL);
        break;
    }
  } else {
    exception("Type mismatch", -1, NULL);
  }
  return result;
}

object_t* z_lt(list_t* args) {
  object_t* a = z_nth(args,0);
  object_t* b = z_nth(args,1);
  object_t* result = newObject();
  result->value->type = Bool;
  if(z_typeof(a) == z_typeof(b)) {
    switch(z_typeof(a)) {
      case Int:
        result->value->value = (generic_value_t) (a->value->value.i < b->value->value.i);
        break;
      case Float:
        result->value->value = (generic_value_t) (a->value->value.f < b->value->value.f);
        break;
      case Char:
        result->value->value = (generic_value_t) (a->value->value.c < b->value->value.c);
        break;
      case String:
        result->value->value = (generic_value_t) (a->value->value.s < b->value->value.s);
        break;
      case Bool:
        result->value->value = (generic_value_t) (a->value->value.b < b->value->value.b);
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
      case FunctionReference:
        exception("Function Reference comparison not yet implemented.",-1,NULL);
        break;
    }
  } else {
    exception("Type mismatch", -1, NULL);
  }
  return result;
}

object_t* z_gt(list_t* args) {
  object_t* a = z_nth(args,0);
  object_t* b = z_nth(args,1);
  object_t* result = newObject();
  result->value->type = Bool;
  if(z_typeof(a) == z_typeof(b)) {
    switch(z_typeof(a)) {
      case Int:
        result->value->value = (generic_value_t) (a->value->value.i > b->value->value.i);
        break;
      case Float:
        result->value->value = (generic_value_t) (a->value->value.f > b->value->value.f);
        break;
      case Char:
        result->value->value = (generic_value_t) (a->value->value.c > b->value->value.c);
        break;
      case String:
        result->value->value = (generic_value_t) (a->value->value.s > b->value->value.s);
        break;
      case Bool:
        result->value->value = (generic_value_t) (a->value->value.b > b->value->value.b);
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
      case FunctionReference:
        exception("Function Reference comparison not yet implemented.",-1,NULL);
        break;
    }
  } else {
    exception("Type mismatch", -1, NULL);
  }
  return result;  return result;
}

object_t* z_lteq(list_t* args) {
  object_t* a = z_nth(args,0);
  object_t* b = z_nth(args,1);
  object_t* result = newObject();
  result->value->type = Bool;
  if(z_typeof(a) == z_typeof(b)) {
    switch(z_typeof(a)) {
      case Int:
        result->value->value = (generic_value_t) (a->value->value.i <= b->value->value.i);
        break;
      case Float:
        result->value->value = (generic_value_t) (a->value->value.f <= b->value->value.f);
        break;
      case Char:
        result->value->value = (generic_value_t) (a->value->value.c <= b->value->value.c);
        break;
      case String:
        result->value->value = (generic_value_t) (a->value->value.s <= b->value->value.s);
        break;
      case Bool:
        result->value->value = (generic_value_t) (a->value->value.b <= b->value->value.b);
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
      case FunctionReference:
        exception("Function Reference comparison not yet implemented.",-1,NULL);
        break;
    }
  } else {
    exception("Type mismatch", -1, NULL);
  }
  return result;
}

object_t* z_gteq(list_t* args){
  object_t* a = z_nth(args,0);
  object_t* b = z_nth(args,1);
  object_t* result = newObject();
  result->value->type = Bool;
  if(z_typeof(a) == z_typeof(b)) {
    switch(z_typeof(a)) {
      case Int:
        result->value->value = (generic_value_t) (a->value->value.i >= b->value->value.i);
        break;
      case Float:
        result->value->value = (generic_value_t) (a->value->value.f >= b->value->value.f);
        break;
      case Char:
        result->value->value = (generic_value_t) (a->value->value.c >= b->value->value.c);
        break;
      case String:
        result->value->value = (generic_value_t) (a->value->value.s >= b->value->value.s);
        break;
      case Bool:
        result->value->value = (generic_value_t) (a->value->value.b >= b->value->value.b);
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
      case FunctionReference:
        exception("Function Reference comparison not yet implemented.",-1,NULL);
        break;
    }
  } else {
    exception("Type mismatch", -1, NULL);
  }
  return result;
}

object_t* z_exit(list_t* args) {
  if (z_nth(args, 0)->value->type == Int) {
    exit(z_nth(args, 0)->value->value.i);
  } else {
    exit(2);
  }
}

object_t* z_fn(list_t* args) {
  char* name = z_nth(args, 0)->value->value.s;
  list_t* _args = z_nth(args, 1)->value->value.l;
  list_t* body = z_nth(args, 2)->value->value.l;
  int params = z_length(_args)->value->value.i;
  object_t* obj = newObject();
  obj->value->value = (generic_value_t) newZFunction(name, _args, body, params);
  addFunctionToSymbolTable(globals, obj->value->value.function);
  obj->value->type = Function;
  return obj;
}

object_t* z_lambda(list_t* args) {
  list_t* _args = z_nth(args, 0)->value->value.l;
  list_t* body = z_nth(args, 1)->value->value.l;
  int params = z_length(_args)->value->value.i;
  object_t* obj = newObject();
  obj->value->value = (generic_value_t) newZFunction(NULL, _args, body, params);
  obj->value->type = Function;
  return obj;
}

object_t* z_let(list_t* args) {
  object_t* symbol = z_nth(args, 0);
  object_t* obj = z_nth(args, 1);
  if (obj->value->type == List) {
    obj = eval(z_nth(args, 1)->value->value.l, NULL);
  }
  addObjectToSymbolTable(globals, symbol, obj);
  return obj;
}

object_t* z_return(list_t* args) {
  return z_nth(args, 0);
}

object_t* z_eval(list_t* args) {
  return eval(args, newSymbolTable());
}

object_t* z_ns(list_t* args) {
  NAMESPACE = z_first(args)->value->value.s;
  return NULL; // eventually this shouldn't be NULL.
}

object_t* z_cond_placeholder(list_t* args) {
  return newObject();
}

object_t* z_import(list_t* args) {
  struct atom *atom= args->head;
  while (atom != NULL) {
    char *buffer = 0;
    long length;
    FILE *f;
    char name[255];
    strcpy(name, ZAPPER_LIB_PATH);
    strcat(name, atom->value->value->value.s);
    strcat(name, ".zap");
    f = fopen(name, "rb");

    if (f) {
      fseek(f, 0, SEEK_END);
      length = ftell(f);
      fseek(f, 0, SEEK_SET);
      buffer = malloc(length);
      if (buffer) {
        fread(buffer, 1, length, f);
      }
      fclose(f);
    }

    if (buffer) {
      object_t* expressions = parse(buffer);
      eval(expressions->value->value.l, globals);
    } else {
      exception("Could not import package", -1, name);
    }
    atom = atom->next;
  }
  return newObject();
}

void init_core() {

  object_t* (*fn)(list_t* args) = &z_fn;
  struct function* fn_ref = newFunction(fn,"fn",3);
  addFunctionToSymbolTable(clib_functions, fn_ref);

  object_t* (*lambda)(list_t* args) = &z_lambda;
  struct function* lambda_ref = newFunction(lambda,"lambda",2);
  addFunctionToSymbolTable(clib_functions, lambda_ref);

  object_t* (*eval)(list_t* args) = &z_eval;
  struct function* eval_ref = newFunction(eval,"eval",1);
  addFunctionToSymbolTable(clib_functions, eval_ref);

  object_t* (*ns)(list_t* args) = &z_ns;
  struct function* ns_ref = newFunction(ns,"ns",1);
  addFunctionToSymbolTable(clib_functions, ns_ref);

  object_t* (*let)(list_t* args) = &z_let;
  struct function* let_ref = newFunction(let,"let",2);
  addFunctionToSymbolTable(clib_functions, let_ref);

  object_t* (*_return)(list_t* args) = &z_return;
  struct function* return_ref = newFunction(_return,"return",1);
  addFunctionToSymbolTable(clib_functions, return_ref);

  object_t* (*import)(list_t* args) = &z_import;
  struct function* import_ref = newFunction(import,"import",-1);
  addFunctionToSymbolTable(clib_functions, import_ref);

  object_t* (*cond)(list_t* args) = &z_cond_placeholder;
  struct function* cond_ref = newFunction(cond,"cond",-1);
  addFunctionToSymbolTable(clib_functions, cond_ref);

  object_t* (*add)(list_t* args) = &z_add;
  struct function* add_ref = newFunction(add,"+",2);
  addFunctionToSymbolTable(clib_functions, add_ref);

  object_t* (*sub)(list_t* args) = &z_sub;
  struct function* sub_ref = newFunction(sub,"-",2);
  addFunctionToSymbolTable(clib_functions, sub_ref);

  object_t* (*div)(list_t* args) = &z_div;
  struct function* div_ref = newFunction(div,"/",2);
  addFunctionToSymbolTable(clib_functions, div_ref);

  object_t* (*mul)(list_t* args) = &z_mul;
  struct function* mul_ref = newFunction(mul,"*",2);
  addFunctionToSymbolTable(clib_functions, mul_ref);

  object_t* (*eq)(list_t* args) = &z_eq;
  struct function* eq_ref = newFunction(eq,"=",2);
  addFunctionToSymbolTable(clib_functions, eq_ref);

  object_t* (*lt)(list_t* args) = &z_lt;
  struct function* lt_ref = newFunction(lt,"<",2);
  addFunctionToSymbolTable(clib_functions, lt_ref);

  object_t* (*gt)(list_t* args) = &z_gt;
  struct function* gt_ref = newFunction(gt,">",2);
  addFunctionToSymbolTable(clib_functions, gt_ref);

  object_t* (*lteq)(list_t* args) = &z_lteq;
  struct function* lteq_ref = newFunction(lteq,"<=",2);
  addFunctionToSymbolTable(clib_functions, lteq_ref);

  object_t* (*gteq)(list_t* args) = &z_gteq;
  struct function* gteq_ref = newFunction(gteq,">=",2);
  addFunctionToSymbolTable(clib_functions, gteq_ref);

  object_t* (*_exit)(list_t* args) = &z_exit;
  struct function* exit_ref = newFunction(_exit,"exit",1);
  addFunctionToSymbolTable(clib_functions, exit_ref);

  object_t* (*print)(list_t* args) = &z_print;
  struct function* print_ref = newFunction(print,"print",1);
  addFunctionToSymbolTable(clib_functions, print_ref);

  object_t* (*println)(list_t* args) = &z_println;
  struct function* println_ref = newFunction(println,"println",1);
  addFunctionToSymbolTable(clib_functions, println_ref);


}
