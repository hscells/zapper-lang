#include <strings.h>

#include "../../system.h"

/**
 * creates a new list
 * @return a new list
 */
t_object* z_list() {
  t_list* list = (t_list*) malloc(sizeof(t_list));
  list->head = NULL;
  t_object* list_obj = newObject();
  list_obj->value->type = List;
  list_obj->value->value.l = list;
  return list_obj;
}

/**
 * Creates a new list encapsulated in an object
 * @return a new list in an object
 */
t_object* z_list_obj(t_list* list) {
  t_object* list_obj = newObject();
  list_obj->value->type = List;
  list_obj->value->value.l = list;
  return list_obj;
}

/**
 * (conj)-oin a list with another object
 * @param list the list being conjoined onto
 * @param o    the object which is being conjoined
 */
t_object* z_conj(t_list* list, t_object* o) {
  struct atom* atom = (struct atom*) malloc(sizeof(struct atom));
  atom->value = o;
  if (list->head == NULL) {
    list->head = atom;
    list->tail = atom;
  } else {
    list->tail->next = atom;
    list->tail = atom;
  }
  return z_list_obj(list);
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
t_object* z_rest(t_list* list) {
  struct atom* atom = list->head->next;
  t_list* new_list = z_list()->value->value.l;
  new_list->head = atom;
  return z_list_obj(new_list);
}

t_object* z_nth(t_list* list, int index) {
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

t_object* z_length(t_list* list) {
  int length = 0;
  struct atom* atom = list->head;
  while(atom != NULL) {
    length++;
    atom = atom->next;
  }
  t_object* len = newObject();
  len->value->type = Int;
  len->value->value = (t_generic_value) length;
  return len;
}

void init_lists() {
  
}
