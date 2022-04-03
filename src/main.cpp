#include <iostream>
#include <fstream>
#include <cstdio>
#include "parser.h"


using namespace std;

void print_ast(Ast const* ast) {
  if (!ast) {
    printf("!NOTHING!");
    return;
  } 
  
  if (ast->nkids > 0) {
    auto is_block = ast->type == AST_BLOCK;
    printf(is_block ? "{\n" : "(");

    if (ast->type == AST_OP_PLUS)  printf("+");
    if (ast->type == AST_OP_MINUS) printf("-");
    if (ast->type == AST_OP_MULT)  printf("*");
    if (ast->type == AST_OP_EQ)    printf("=");
    
    for (auto kid = ast->kid; kid; kid = kid->next) {
      printf(" ");
      print_ast(kid);
      if (is_block) printf(";\n");
    }

    printf(is_block ? "}\n" : ")");
  } else if (ast->type == AST_ATOM) {
    printf("%s", ast->atom->c_str());
  } else if (ast->type == AST_LIT_INT) {
    printf("%d", ast->int_value);
  }
}

char filename[1 << 8];

int main(int argc, char** argv) {
  auto parser = Parser();
  sprintf(filename, "sample/%s.fe", argv[1]);
  auto ast = parser.parse_file(filename);
  print_ast(ast);
}