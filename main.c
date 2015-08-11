#include <stdio.h>
#include "objects.h"
#include "types.h"
#include "expressions.h"
#include "garbage.h"

#include "system.h"

#define INPUT_SIZE 512

int EXIT_STATUS = 0;

void repl_init();

int main(int argc, char const *argv[]) {

  // initiate the type system
  initTypes();
  // create a heap for the vm to run
  t_heap* heap = newHeap();
  // create a stack on the heap for the main class
  t_stack* mainStack = newStack();


  // testing....
  t_object* shop = newObject();
  addFieldToFieldlist(shop->fields, "money", Type->Int, newInt(420, mainStack, heap));

  z_println(shop->fields->field[0]->object);

  repl_init();

  free(heap);
  free(mainStack);

  collect(shop);

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
