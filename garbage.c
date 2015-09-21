#include "garbage.h"

void collect(t_object* obj) {
  free(obj);
}
