#include <stdio.h>
#include "objects.h"
#include "types.h"
#include "expressions.h"

#include "system.h"

#define INPUT_SIZE 512

int EXIT_STATUS = 0;

void repl_init();

int main(int argc, char const *argv[]) {

  t_heap* heap = newHeap();
  t_stack* mainStack = newStack();


  t_fieldlist* fields = newFieldlist();
  t_methodlist* methods = newMethodlist();
  addFieldToFieldlist(fields, "money", Int(), newInt(420, mainStack, heap));
  t_object* shop = newObject(methods, fields);

  z_println(shop->fields->field[0]->object);

  repl_init();

  return 0;
}

void repl_init() {
  char input[INPUT_SIZE] = "";
  printf("%s", ">>> ");
  while (EXIT_STATUS == 0 && fgets(input, INPUT_SIZE, stdin)) {
    t_expression *expression = (t_expression*) malloc(sizeof(t_expression));
    expression->ast = parse(input, 0);
    eval(expression);
    free(expression);

    if (EXIT_STATUS == 0) {
      printf("%s", ">>> ");
    }
    EXIT_STATUS = 1;
  }
}
