#include "expressions.h"

union token initTokens() {
  union token tokens;
  tokens.LBRAC = 0;
  tokens.RBRAC = 1;
  tokens.LCURL = 2;
  tokens.RCURL = 3;
  tokens.EQUAL = 4;
  tokens.PLUS = 5;
  tokens.MINUS = 6;
  tokens.DIVIDE = 7;
  tokens.MULTIPLY = 8;
  tokens.IF = 9;
  tokens.FOR = 10;
  tokens.FN = 11;
  tokens.RETURN = 12;
  return tokens;
}

t_ast* newAst () {
  union token tokens = initTokens();
  t_ast* ast = (t_ast*) malloc(sizeof(t_ast));

  return ast;
}

union generic eval(t_expression *expression) {
  union generic value;
  return value;
}
