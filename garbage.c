#include "garbage.h"

void collect(t_object* obj) {
  free(obj->methods);
  free(obj->fields);
  free(obj);
}
