#include "types.h"

int getType(t_type *type) {
  return type->type;
}

t_type* Int(){
  t_type* type_int = (t_type*) malloc(sizeof(t_type));
  type_int->type = 0;
  return type_int;
}
