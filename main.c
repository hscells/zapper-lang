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

  ZLIB_PATH = getenv("ZLIB_PATH");
  if (ZLIB_PATH == NULL) {
    // printf("$ZLIB_PATH is not set. Using cwd for libraries and imports");
  }

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
    symboltable_t* context = newSymbolTable();
    list_t* expressions = parse(buffer)->value->value.l;
    object_t* value = eval(expressions, context);
    free(expressions);
    free(value);
    if (clib_functions->head != clib_functions->tail) {
      collecsymboltable_t(clib_functions);
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

    // symboltable_t* symboltable = newSymbolTable();

    if (EXIT_STATUS == 0) {
      printf("%s", ">>> ");
    }
    EXIT_STATUS = 1;
  }
}
