#include <cstdio>
#include "parser.h"


Ast* Parser::parse_file(const char* path) {
  _lexer.lex_file(path);
  return a_num_expr();
}

Token* Parser::peek(int offset) {
  return _lexer.token(_focus + offset);
}

Token* Parser::t_expect(TokenType type) {
  auto token = peek(0);
  if (token && token->type == type) {
    _focus++;
    return token;
  }
  return nullptr;
}

Ast* Parser::a_lit_int() {
  auto token = t_expect(TK_LIT_INT);
  if (token) {
    return new Ast { .type = AST_LIT_INT, .int_value = token->int_value };
  }
  return nullptr;
}

Ast* Parser::a_num_expr() {
  auto left = a_lit_int();
  if (!left) return nullptr;

  auto plus = t_expect(TK_OP_PLUS);
  if (!plus) return left;

  auto right = a_num_expr();
  if (!right) return nullptr;

  return new Ast { 
    .type = AST_OP_PLUS,
    .left = left,
    .right = right,
  };
}
