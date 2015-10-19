#include "garbage.h"

void collect(object_t* obj) {
  free(obj);
}

void colleclist_t(list_t* list) {
  struct atom* node = list->head;
  while (node != NULL) {
    struct atom* temp = node;
    node = node->next;
    free(temp);
  }
  list->head = NULL;
}

void collecsymboltable_t(symboltable_t* s) {
  struct symboltable_row_t* curr = s->head;
  while ((curr = s->head) != NULL) {
    s->head = s->head->next;
    free(curr->object);
    free(curr);
  }
  free(s);
}
