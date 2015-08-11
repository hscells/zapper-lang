#include "types.h"

int getType(int type) {
  return 0;
}

void initTypes() {
  Type = (t_type*) malloc(sizeof(t_type));
  Type->Int     = 0xff01;
  Type->Float   = 0xff02;
  Type->Char    = 0xff03;
  Type->String  = 0xff04;
  Type->List    = 0xff05;
}
