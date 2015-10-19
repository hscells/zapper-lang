#ifndef __GARBAGE_H
#define __GARBAGE_H

#include <stdlib.h>
#include "objects.h"
#include "system.h"

void collect(object_t* obj);
void colleclist_t(list_t* list);
void collecsymboltable_t(symboltable_t* s);

#endif
