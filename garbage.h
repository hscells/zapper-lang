#ifndef __GARBAGE_H
#define __GARBAGE_H

#include <stdlib.h>
#include "objects.h"
#include "system.h"

void collect(t_object* obj);
void collect_list(t_list* list);
void collect_symboltable(t_symboltable* s);

#endif
