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

  // first couple of lines initialise the system itself:

  // create a heap for the vm to run
  t_heap* heap = newHeap();
  // create a stack on the heap for the main class
  t_stack* mainStack = newStack();
  symboltable = newSymbolTable(mainStack);

  t_ast* ast = newAst();

  native_code = malloc(sizeof(struct node));

  // these next lines are used to read a file in for evaluation

  char *buffer = 0;
  long length;
  FILE *f;

  if (argc > 1) {
    f = fopen(argv[1], "rb");

    if (f) {
      fseek(f, 0, SEEK_END);
      length = ftell(f);
      fseek(f, 0, SEEK_SET);
      buffer = malloc(length);
      if (buffer) {
        fread(buffer, 1, length, f);
      }
      fclose(f);
    }

  }

  if (buffer) {
    ast = parse(buffer, mainStack, heap, 0);
    eval(ast);
  } else {
    exception("No input file was specified.",0 ,NULL);
  }

  // garbage function but resides in expressions
  free(heap);
  free(mainStack);

  return 0;
}

void repl_init(t_stack* stack, t_heap* heap) {
  char input[INPUT_SIZE] = "";
  printf("%s", ">>> ");
  while (EXIT_STATUS == 0 && fgets(input, INPUT_SIZE, stdin)) {
    t_expression *expression = (t_expression*) malloc(sizeof(t_expression));
    expression->ast = parse(input, stack, heap, 0);

    eval(expression->ast);
    free(expression->ast);
    free(expression);

    if (EXIT_STATUS == 0) {
      printf("%s", ">>> ");
    }
    EXIT_STATUS = 1;
  }
}
