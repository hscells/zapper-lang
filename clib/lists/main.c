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
object_t* z_first(list_t* list) {
  return list->head->value;
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

object_t* z_length(list_t* list) {
  int length = 0;
  struct atom* atom = list->head;
  while(atom != NULL) {
    length++;
    atom = atom->next;
  }
  object_t* len = newObject();
  len->value->type = Int;
  len->value->value = (generic_value_t) length;
  return len;
}

void inilist_ts() {
  
}
