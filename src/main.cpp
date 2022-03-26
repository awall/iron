#include <iostream>
#include <fstream>
#include <cstdio>

#include "parser.h"

void print_ast(Ast* ast) {
  if (ast->type == AST_LIT_INT) {
      printf("%d", ast->int_value);
  }
  if (ast->type == AST_OP_PLUS) {
      printf("(");
      print_ast(ast->left);
      printf("+");
      print_ast(ast->right);
      printf(")");
  }
}

int main(int argc, char** argv) {
  auto parser = Parser();
  auto ast = parser.parse_file("sample/number.fe");
  print_ast(ast);
}