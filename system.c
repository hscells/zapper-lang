#include "system.h"
#include "expressions.h"
#include <string.h>

void exception(char* e, int line_number, char* token) {
  printf("An Exception was raised on line: %d, near: '%s'\n\t%s\n", line_number, token, e);
  exit(2);
}

void z_exception(char* e) {
  printf("An Exception was raised on line: %d, near: '%s'\n\t%s\n", LINE_NUMBER, CURRENT_TOKEN, e);
  exit(2);
}

t_generic* newGeneric() {
  return (t_generic*) malloc(sizeof(t_generic));
}

t_generic* z_add(t_generic* a, t_generic* b) {
  t_generic* result = newGeneric();
  result->value = (t_generic_value) (a->value.i + b->value.i);
  result->type = Int;
  return result;
}

t_generic* z_sub(t_generic* a, t_generic* b) {
  t_generic* result = newGeneric();
  result->value = (t_generic_value) (a->value.i - b->value.i);
  result->type = Int;
  return result;
}

t_generic* z_mul(t_generic* a, t_generic* b) {
  t_generic* result = newGeneric();
  result->value = (t_generic_value) (a->value.i * b->value.i);
  result->type = Int;
  return result;
}

t_generic* z_div(t_generic* a, t_generic* b) {
  t_generic* result = newGeneric();
  result->value = (t_generic_value) (a->value.i / b->value.i);
  result->type = Int;
  return result;
}

void z_print(t_object* o) {
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
      printf("%s", o->value->value.s);
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
      z_print(getSymbolByName(symboltable, o->value->value.s));
      return;
  }
  printf("<object> @ %d", o->id);
}

void z_println(t_object* o) {
  z_print(o);
  printf("\n");
}

enum t_type z_typeof(t_object* o) {
  return o->value->type;
}

t_generic* z_teq(t_object* a, t_object* b) {
  t_generic* result = newGeneric();
  result->type = Bool;
  result->value = (t_generic_value) (z_typeof(a) == z_typeof(b));
  return result;
}

t_generic* z_eq(t_object* a, t_object* b) {
  t_generic* result = newGeneric();
  result->type = Bool;
  result->value = (t_generic_value) false;

  t_generic* teq;
  teq = z_teq(a,b);
  if (!teq->value.b) {
    result->type = Exception;
    result->value = (t_generic_value) "Parameter types do not match.";
    return result;
  }
  switch(a->value->type) {
    case Int:
      result->value = (t_generic_value) (a->value->value.i == b->value->value.i);
      break;
    case Float:
      result->value = (t_generic_value) (a->value->value.f == b->value->value.f);
      break;
    case Char:
      result->value = (t_generic_value) (a->value->value.c == b->value->value.c);
      break;
    case String:
      result->value = (t_generic_value) (strcmp(a->value->value.s, b->value->value.s) == 0);
      break;
    case Bool:
      result->value = (t_generic_value) (a->value->value.b == b->value->value.b);
    case List:
      result->type = Exception;
      result->value = (t_generic_value) "Could not compare Lists.";
      break;
    case Exception:
      result->type = Exception;
      result->value = (t_generic_value) "Could not compare Exceptions.";
      break;
    case Function:
      result->type = Exception;
      result->value = (t_generic_value) "Could not compare Functions.";
      break;
    case Symbol:
      result->value = (t_generic_value) (strcmp(a->value->value.s, b->value->value.s) == 0);
      break;
  }
  return result;
}
t_generic* z_lt(t_object* a, t_object* b) {
  t_generic* result = newGeneric();
  result->type = Bool;
  result->value = (t_generic_value) false;

  t_generic* teq;
  teq = z_teq(a,b);
  if (!teq->value.b) {
    result->type = Exception;
    result->value = (t_generic_value) "Parameter types do not match.";
    return result;
  }

  switch(a->value->type) {
    case Int:
      result->value = (t_generic_value) (a->value->value.i < b->value->value.i);
      break;
    case Float:
      result->value = (t_generic_value) (a->value->value.f < b->value->value.f);
      break;
    case Char:
      result->value = (t_generic_value) (a->value->value.c < b->value->value.c);
      break;
    case String:
      result->value = (t_generic_value) (strcmp(a->value->value.s,b->value->value.s) < 0);
      break;
    case Bool:
      result->type = Exception;
      result->value = (t_generic_value) "Could not compare Bools.";
      break;
    case List:
      result->type = Exception;
      result->value = (t_generic_value) "Could not compare Lists.";
      break;
    case Exception:
      result->type = Exception;
      result->value = (t_generic_value) "Could not compare Exceptions.";
      break;
    case Function:
      result->type = Exception;
      result->value = (t_generic_value) "Could not compare Functions.";
      break;
    case Symbol:
      result->type = Exception;
      result->value = (t_generic_value) "Could not compare Symbols.";
      break;
  }
  return result;
}
t_generic* z_gt(t_object* a, t_object* b) {
  t_generic* result = newGeneric();
  result->type = Bool;
  result->value = (t_generic_value) false;

  t_generic* teq;
  teq = z_teq(a,b);
  if (!teq->value.b) {
    result->type = Exception;
    result->value = (t_generic_value) "Parameter types do not match.";
    return result;
  }

  switch(a->value->type) {
    case Int:
      result->value = (t_generic_value) (a->value->value.i > b->value->value.i);
      break;
    case Float:
      result->value = (t_generic_value) (a->value->value.f > b->value->value.f);
      break;
    case Char:
      result->value = (t_generic_value) (a->value->value.c > b->value->value.c);
      break;
    case String:
      result->value = (t_generic_value) (strcmp(a->value->value.s,b->value->value.s) > 0);
      break;
    case Bool:
      result->type = Exception;
      result->value = (t_generic_value) "Could not compare Bools.";
      break;
    case List:
      result->type = Exception;
      result->value = (t_generic_value) "Could not compare Lists.";
      break;
    case Exception:
      result->type = Exception;
      result->value = (t_generic_value) "Could not compare Exceptions.";
      break;
    case Function:
      result->type = Exception;
      result->value = (t_generic_value) "Could not compare Functions.";
      break;
    case Symbol:
      result->type = Exception;
      result->value = (t_generic_value) "Could not compare Symbols.";
      break;
  }
  return result;
}
t_generic* z_lteq(t_object* a, t_object* b) {
  t_generic* result = newGeneric();
  result->type = Bool;
  result->value = (t_generic_value) false;

  t_generic* teq;
  teq = z_teq(a,b);
  if (!teq->value.b) {
    result->type = Exception;
    result->value = (t_generic_value) "Parameter types do not match.";
    return result;
  }

  switch(a->value->type) {
    case Int:
      result->value = (t_generic_value) (a->value->value.i <= b->value->value.i);
      break;
    case Float:
      result->value = (t_generic_value) (a->value->value.f <= b->value->value.f);
      break;
    case Char:
      result->value = (t_generic_value) (a->value->value.c <= b->value->value.c);
      break;
    case String:
      result->value = (t_generic_value) (strcmp(a->value->value.s,b->value->value.s) <= 0);
      break;
    case Bool:
      result->type = Exception;
      result->value = (t_generic_value) "Could not compare Bools.";
      break;
    case List:
      result->type = Exception;
      result->value = (t_generic_value) "Could not compare Lists.";
      break;
    case Exception:
      result->type = Exception;
      result->value = (t_generic_value) "Could not compare Exceptions.";
      break;
    case Function:
      result->type = Exception;
      result->value = (t_generic_value) "Could not compare Functions.";
      break;
    case Symbol:
      result->type = Exception;
      result->value = (t_generic_value) "Could not compare Symbols.";
      break;
  }
  return result;
}

t_generic* z_gteq(t_object* a, t_object* b){
  t_generic* result = newGeneric();
  result->type = Bool;
  result->value = (t_generic_value) false;

  t_generic* teq;
  teq = z_teq(a,b);
  if (!teq->value.b) {
    result->type = Exception;
    result->value = (t_generic_value) "Parameter types do not match.";
    return result;
  }

  switch(a->value->type) {
    case Int:
      result->value = (t_generic_value) (a->value->value.i >= b->value->value.i);
      break;
    case Float:
      result->value = (t_generic_value) (a->value->value.f >= b->value->value.f);
      break;
    case Char:
      result->value = (t_generic_value) (a->value->value.c >= b->value->value.c);
      break;
    case String:
      result->value = (t_generic_value) (strcmp(a->value->value.s,b->value->value.s) >= 0);
      break;
    case Bool:
      result->type = Exception;
      result->value = (t_generic_value) "Could not compare Bools.";
      break;
    case List:
      result->type = Exception;
      result->value = (t_generic_value) "Could not compare Lists.";
      break;
    case Exception:
      result->type = Exception;
      result->value = (t_generic_value) "Could not compare Exceptions.";
      break;
    case Function:
      result->type = Exception;
      result->value = (t_generic_value) "Could not compare Functions.";
      break;
    case Symbol:
      result->type = Exception;
      result->value = (t_generic_value) "Could not compare Symbols.";
      break;
  }
  return result;
}

void z_exit() {
  exit(1);
}

t_generic* z_read() {
  t_generic* result = newGeneric();
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

t_object* z_nth(t_list* list, int index) {
  if (index > z_length(list) - 1 || index < 0) {
    z_exception("Index out of bounds.");
    return NULL;
  }
  struct atom* a = list->head;
  for (int i = 0; i < z_length(list); i++) {
    if (i == index) {
      return a->value;
    }
    a = a->next;
  }
  return NULL;
}

int z_length(t_list* list) {
  int length = 0;
  list->atom = list->head;
  while(list->atom != NULL) {
    length++;
    list->atom = list->atom->next;
  }
  return length;
}

t_generic* z_eval(char* expressions, t_stack* stack, t_heap* heap) {
  t_ast* ast = newAst();
  ast = parse(expressions, stack, heap, 0);
  return eval(ast);
}

t_object* z_int(int x) {
  t_object* obj = newObject();
  obj->value->type = Int;
  obj->value->value = (t_generic_value) x;
  return obj;
}

t_object* newSystem(t_stack* stack, t_heap* heap) {
  t_fieldlist* fields = newFieldlist();
  t_methodlist* methods = newMethodlist();
  t_object* System = newObject(methods, fields);
  return System;
}

t_symboltable* newSymbolTable() {
  t_symboltable *s = (t_symboltable*) malloc(sizeof(t_symboltable));
  s->head = NULL;
  return s;
}

void addFunctionToSymbolTable(t_symboltable* s, char* name, struct node *node, t_list* formal_parameters) {
  struct t_symboltable_row* row = (struct t_symboltable_row*) malloc(sizeof(struct t_symboltable_row));
  row->object = newObject();
  row->id = row->object->id;
  row->node = node;
  row->formal_parameters = formal_parameters;
  if (s->head == NULL) {
    s->head = row;
  } else {
    s->tail->next = row;
    s->tail = row;
  }
}

void addObjectToSymbolTable(t_symboltable* s, char* name, t_object* object, struct node *node) {
  struct t_symboltable_row* row = (struct t_symboltable_row*) malloc(sizeof(struct t_symboltable_row));
  row->name = name;
  row->node = node;
  if (object != NULL) {
    row->object = object;
    row->id = object->id;
  }
  row->next = NULL;

  if (s->head == NULL) {
    s->head = row;
    s->tail = row;
  } else {
    s->tail->next = row;
    s->tail = row;
  }
}

void printSymboltable(t_symboltable* s) {
  struct t_symboltable_row* r = s->head;
  printf("Current Symboltable:\n");
  while(r != NULL) {
    printf("\tobject name: %s\n", r->name);
    r = r->next;
  }
}

t_object* getSymbolByName(t_symboltable* s, char* name) {
  struct t_symboltable_row* r = s->head;
  while (r != NULL) {
    if (strcmp(r->name, name) == 0) {
      return r->object;
    }
    r = r->next;
  }
  exception("Object has no value",-1, name);
  return NULL;
}
