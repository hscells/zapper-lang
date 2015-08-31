#include <stdio.h>
#include "objects.h"
#include "types.h"
#include "expressions.h"
#include "garbage.h"

#include "system.h"

#define INPUT_SIZE 512

int EXIT_STATUS = 0;

void repl_init();
t_symboltable *symboltable;

int main(int argc, char const *argv[]) {

  // first couple of lines initialise the system itself:

  // initiate the type system
  initTypes();
  // create a heap for the vm to run
  t_heap* heap = newHeap();
  // create a stack on the heap for the main class
  t_stack* mainStack = newStack();
  symboltable = newSymbolTable();

  t_ast* ast = newAst();

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

    //  if there is no input, test some things
    struct node* starting_node = ast->node;

    ast->node->next = newTokenNode(ast->system->ADD, ast);
    ast->node = ast->node->next;

    ast->node->next = newTokenNode(ast->tokens->LBRAC, ast);
    ast->node = ast->node->next;

    ast->node->next = newObjectNode(newInt(8, mainStack, heap), ast);
    ast->node = ast->node->next;

    ast->node->next = newTokenNode(ast->tokens->COMMA, ast);
    ast->node = ast->node->next;

    ast->node->next = newObjectNode(newInt(5, mainStack, heap), ast);
    ast->node = ast->node->next;

    ast->node->next = newTokenNode(ast->tokens->RBRAC, ast);
    ast->node = ast->node->next;

    ast->node->next = NULL;

    ast->node = starting_node;

    eval(ast);
  }

  // garbage function but resides in expressions
  collectAst(ast);

  free(heap);
  free(mainStack);
  free(Type);

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
