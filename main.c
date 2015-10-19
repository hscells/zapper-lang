#include <stdio.h>
#include "types.h"
#include "objects.h"
#include "expressions.h"
#include "system.h"

#define INPUT_SIZE 512

int EXIT_STATUS = 0;

void repl_init();

int main(int argc, char const *argv[]) {

  // t_symboltable* symboltable = newSymbolTable();
  init_system();
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
    // ast = parse(buffer, mainStack, heap, 0);
    // eval(ast, symboltable, 0, ast->head);
    t_list* expressions = parse(buffer)->value->value.l;
    t_object* value = eval(expressions);
    free(expressions);
    free(value);
    free(clib_functions);
  } else {
    free(clib_functions);
    exception("No input file was specified.",0 ,NULL);
  }
  return 0;
}

void repl_init() {
  char input[INPUT_SIZE] = "";
  printf("%s", ">>> ");
  while (EXIT_STATUS == 0 && fgets(input, INPUT_SIZE, stdin)) {

    // t_symboltable* symboltable = newSymbolTable();

    if (EXIT_STATUS == 0) {
      printf("%s", ">>> ");
    }
    EXIT_STATUS = 1;
  }
}
