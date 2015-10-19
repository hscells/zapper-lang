#include "garbage.h"

void collect(t_object* obj) {
  free(obj);
}

void collect_list(t_list* list) {
  struct atom* node = list->head;
  while (node != NULL) {
    struct atom* temp = node;
    node = node->next;
    free(temp);
  }
  list->head = NULL;
}

void collect_symboltable(t_symboltable* s) {
  struct t_symboltable_row* curr = s->head;
  while ((curr = s->head) != NULL) {
    s->head = s->head->next;
    free(curr->object);
    free(curr);
  }
  free(s);
}
