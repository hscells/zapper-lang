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

  // initiate the type system
  initTypes();
  // create a heap for the vm to run
  t_heap* heap = newHeap();
  // create a stack on the heap for the main class
  t_stack* mainStack = newStack();
  symboltable = newSymbolTable();

  // testing....
  t_object* shop = newObject();
  addFieldToFieldlist(shop->fields, "money", Type->Int, newInt(420, mainStack, heap));

  // z_println(shop->fields->field[0]->object);
  t_ast* ast = newAst();
  ast->node = newTokenNode(ast->tokens->START, ast);
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

  ast->node = starting_node;

  eval(ast);
  collectAst(ast);
  // repl_init();

  free(heap);
  free(mainStack);
  free(Type);

  collect(shop);

  return 0;
}

void repl_init() {
  char input[INPUT_SIZE] = "";
  printf("%s", ">>> ");
  while (EXIT_STATUS == 0 && fgets(input, INPUT_SIZE, stdin)) {
    t_expression *expression = (t_expression*) malloc(sizeof(t_expression));
    expression->ast = parse(input, 0);

    eval(expression->ast);
    free(expression->ast);
    free(expression);

    if (EXIT_STATUS == 0) {
      printf("%s", ">>> ");
    }
    EXIT_STATUS = 1;
  }
}
