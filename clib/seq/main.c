#include "../../system.h"

t_object* z_map(t_object* function, t_list* list) {
  struct atom* item = list->head;
  t_list* new_list = z_list()->value->value.l;
  while (item->next != NULL) {
    t_list* eval_list = z_list()->value->value.l;
    z_conj(eval_list, function);
    z_conj(eval_list, item->value);
    z_conj(new_list, eval(eval_list));
    item = item->next;
  }
  return z_list_obj(list);
}

void init_seq() {
  t_object* (*map)(t_object*, t_list*) = &z_map;
  addFunctionToSymbolTable(clib_functions, map, 2, "map");
}
