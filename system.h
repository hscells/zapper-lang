#include <stdlib.h>
#include <stdio.h>
#include "objects.h"
#include "types.h"

t_object* newInt(int v, t_stack* stack, t_heap* heap);
t_object* newChar(char c, t_stack* stack, t_heap* heap);
t_object* newString(char* s, t_stack* stack, t_heap* heap);

void z_println(t_object* o);
