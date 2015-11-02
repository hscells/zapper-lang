#include <strings.h>

#include "../../system.h"

/**
 * creates a new list
 * @return a new list
 */
object_t* z_list() {
  list_t* list = (list_t*) malloc(sizeof(list_t));
  list->head = NULL;
  object_t* list_obj = newObject();
  list_obj->value->type = List;
  list_obj->value->value.l = list;
  return list_obj;
}

object_t* zz_list(list_t *args) {
  object_t* list = z_list();
  // list->value->value.l = args;
  list->value->value.l->head = args->head;
  list->value->value.l->tail = args->head;
  struct atom* temp = args->head;
  while(temp != NULL) {
    list->value->value.l->tail = temp;
    list->value->value.l->tail->next = temp->next;
    temp = temp->next;
  }
  return list;
}

/**
 * Creates a new list encapsulated in an object
 * @return a new list in an object
 */
object_t* z_list_obj(list_t* list) {
  object_t* list_obj = newObject();
  list_obj->value->type = List;
  list_obj->value->value.l = list;
  return list_obj;
}

/**
 * (conj)-oin a list with another object
 * @param list the list being conjoined onto
 * @param o    the object which is being conjoined
 */
object_t* z_conj(list_t* list, object_t* o) {
  struct atom* atom = (struct atom*) malloc(sizeof(struct atom));
  atom->value = o;
  atom->next = NULL;
  if (list->head == NULL) {
    list->head = atom;
    list->tail = atom;
  } else {
    list->tail->next = atom;
    list->tail = atom;
  }
  return z_list_obj(list);
}


//z_conj is used so prolifically in the internals, something needs to go on top of it
object_t* zz_conj(list_t* list) {
  list_t* l = z_nth(list, 0)->value->value.l;
  object_t* o = z_nth(list, 1);
  return z_conj(l, o);
}


/**
 * gets the first item from a list
 * @param  list the list being looked at
 * @return      the object at the very start of the list
 */
object_t* z_first(list_t* list) {
  return list->head->value;
}

object_t* zz_first(list_t* args) {
  if (z_typeof(z_first(args)) == String) {
    char* s = z_first(args)->value->value.s;
    char c = *s;
    object_t* o = newObject();
    o->value->value.c = c;
    o->value->type = Char;
    return o;
  } else {
    return z_first(z_nth(args, 0)->value->value.l);
  }
}



/**
 * gets everything but the first element from a list
 * @param  list the list being looked at
 * @return      everything except the first item in the list
 */
object_t* z_rest(list_t* list) {
  struct atom* atom = list->head->next;
  list_t* new_list = z_list()->value->value.l;
  new_list->head = atom;
  return z_list_obj(new_list);
}

object_t* zz_rest(list_t* args) {
  if (z_typeof(z_first(args)) == String) {
    char* s = z_first(args)->value->value.s;
    char* new_s = ++s;
    object_t* o = newObject();
    o->value->value.s = new_s;
    o->value->type = String;
    return o;
  } else {
    return z_rest(z_nth(args, 0)->value->value.l);
  }
}

object_t* z_nth(list_t* list, int index) {
  if (index > z_length(list)->value->value.i - 1 || index < 0) {
    z_exception("Index out of bounds.");
    return NULL;
  }
  struct atom* a = list->head;
  for (int i = 0; i < z_length(list)->value->value.i; i++) {
    if (i == index) {
      return a->value;
    }
    a = a->next;
  }
  return NULL;
}

//z_nth is used so prolifically in the internals, something needs to go on top of it
object_t* zz_nth(list_t* list) {
  if (z_typeof(z_nth(list, 0)) == String) {
    int index = z_nth(list, 1)->value->value.i;
    char* s = z_first(list)->value->value.s;
    char c;
    for (int i = 0; i <= index; i++) {
      c = *s;
      ++s;
    }
    object_t* o = newObject();
    o->value->value.c = c;
    o->value->type = Char;
    return o;
  } else {
    list_t* l = z_nth(list, 0)->value->value.l;
    int index = z_nth(list, 1)->value->value.i;
    return z_nth(l, index);
  }
}

object_t* z_length(list_t* list) {
  int length = 0;
  struct atom* atom = list->head;
  while(atom != NULL) {
    length++;
    atom = atom->next;
  }
  object_t* len = newObject();
  len->value->type = Int;
  len->value->value.i = length;
  return len;
}

object_t* zz_length(list_t* args) {
  if (z_typeof(z_first(args)) == String) {
    char* s = z_first(args)->value->value.s;
    object_t* o = newObject();
    o->value->value.i = strlen(s);
    o->value->type = Int;
    return o;
  } else {
    return z_length(z_nth(args, 0)->value->value.l);
  }
}



void inilist_ts() {
  object_t* (*length)(list_t* args) = &zz_length;
  struct function* length_ref = newFunction(length,"length",1);
  addFunctionToSymbolTable(clib_functions, length_ref);

  object_t* (*nth)(list_t* args) = &zz_nth;
  struct function* nth_ref = newFunction(nth,"nth",2);
  addFunctionToSymbolTable(clib_functions, nth_ref);

  object_t* (*first)(list_t* args) = &zz_first;
  struct function* first_ref = newFunction(first,"first",1);
  addFunctionToSymbolTable(clib_functions, first_ref);

  object_t* (*rest)(list_t* args) = &zz_rest;
  struct function* rest_ref = newFunction(rest,"rest",1);
  addFunctionToSymbolTable(clib_functions, rest_ref);

  object_t* (*conj)(list_t* args) = &zz_conj;
  struct function* conj_ref = newFunction(conj,"conj",2);
  addFunctionToSymbolTable(clib_functions, conj_ref);

  object_t* (*list)(list_t* args) = &zz_list;
  struct function* list_ref = newFunction(list,"list",-1);
  addFunctionToSymbolTable(clib_functions, list_ref);
}
