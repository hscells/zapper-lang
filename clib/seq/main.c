#include "../../system.h"

object_t* z_map(object_t* function, list_t* list) {
  struct atom* item = list->head;
  list_t* new_list = z_list()->value->value.l;
  while (item->next != NULL) {
    list_t* eval_list = z_list()->value->value.l;
    z_conj(eval_list, function);
    z_conj(eval_list, item->value);
    z_conj(new_list, eval(eval_list, globals));
    item = item->next;
  }
  return z_list_obj(list);
}

void init_seq() {
  object_t* (*map)(object_t*, list_t*) = &z_map;
  struct function* map_ref = newFunction(map, "map", 2);
  addFunctionToSymbolTable(clib_functions, map_ref);
}
