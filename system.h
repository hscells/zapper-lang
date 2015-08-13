#include <stdlib.h>
#include <stdio.h>
#include "objects.h"
#include "types.h"

t_object* newInt(int v, t_stack* stack, t_heap* heap);
t_object* newChar(char c, t_stack* stack, t_heap* heap);
t_object* newString(char* s, t_stack* stack, t_heap* heap);

void z_print(t_object* o);
void z_typeof(t_object* o);
void z_exception(char* e, int l, int c);

int z_add(int a, int b);
int z_sub(int a, int b);
int z_mul(int a, int b);
int z_div(int a, int b);
