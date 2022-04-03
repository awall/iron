#pragma once
#include <iostream>
#include <fstream>
#include <optional>
#include "lexer.h"


enum AstType {
  AST_NONE,

  AST_ATOM,
  AST_BLOCK,
  AST_LIT_INT,
  AST_OP_PLUS,
  AST_OP_MINUS,
  AST_OP_MULT,
  AST_OP_EQ,
};

struct Ast {
  int nkids;
  int span;  
  AstType type;  

  Ast* kid;
  Ast* next;

  union {
    Atom* atom;
    int int_value;
  };
};

class Parser {
private:

  Lexer _lexer = {};
  size_t _focus = 0;
  Ast* _nodes; size_t _nnodes = 0;
  
  Token* peek(int offset);
  Token* t_expect(TokenType);

  AstType op();
  Ast* a_lit_int();
  Ast* a_atom();
  Ast* a_bracketed();
  Ast* a_expr();
  Ast* a_block();
  Ast* a_statements();

public:

  Parser();
  ~Parser();

  Ast* parse_file(char const* path);

};
