#include <iostream>
#include <fstream>
#include <cstdio>
#include "parser.h"


using namespace std;

void print_ast(Ast const* ast) {
  if (!ast) {
    printf("");
  } else if (ast->type == AST_ATOM) {
    printf("%s", ast->atom->c_str());
  } else if (ast->type == AST_LIT_INT) {
    printf("%d", ast->int_value);
  } else if (ast->type == AST_OP_PLUS) {
    printf("(");
    print_ast(ast->left);
    printf("+");
    print_ast(ast->right);
    printf(")");
  } else if (ast->type == AST_OP_MINUS) {
    printf("(");
    print_ast(ast->left);
    printf("-");
    print_ast(ast->right);
    printf(")");
  } else if (ast->type == AST_OP_MULT) {
    printf("(");
    print_ast(ast->left);
    printf("*");
    print_ast(ast->right);
    printf(")");
  } else if (ast->type == AST_OP_EQ) {
    printf("(");
    print_ast(ast->left);
    printf("=");
    print_ast(ast->right);
    printf(")");
  }
}

char filename[1 << 8];

int main(int argc, char** argv) {
  auto parser = Parser();
  sprintf(filename, "sample/%s.fe", argv[1]);
  auto ast = parser.parse_file(filename);
  Ast* astp = ast ? &*ast : nullptr;
  print_ast(astp);
}