#pragma once
#include <iostream>
#include <fstream>
#include "lexer.h"


enum AstType {
  AST_LIT_INT,
  AST_OP_PLUS,
};

struct Ast {
  AstType type;

  union {
    int int_value;
    struct {
      Ast* left;
      Ast* right;
    };
  };
};

class Parser {
private:

  Lexer _lexer = {};
  int _focus = 0;
  
  Token* peek(int offset);

  Token* t_expect(TokenType);
  Ast* a_lit_int();
  Ast* a_num_expr();
  Ast* a_op_plus();

public:

  Ast* parse_file(char const* path);

};
