#include <stdio.h>
#include "types.h"
#include "objects.h"
#include "expressions.h"
#include "system.h"
#include "garbage.h"

#define INPUT_SIZE 512

int EXIT_STATUS = 0;

void repl_init();

int main(int argc, char const *argv[]) {

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
    init_system();
    t_list* expressions = parse(buffer)->value->value.l;
    t_object* value = eval(expressions);
    free(expressions);
    free(value);
    if (clib_functions->head != clib_functions->tail) {
      collect_symboltable(clib_functions);
    }
  } else {
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
