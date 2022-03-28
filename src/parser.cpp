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

AstType Parser::op() {
  if (t_expect(TK_OP_PLUS)) return AST_OP_PLUS;
  if (t_expect(TK_OP_MULT)) return AST_OP_MULT;
  return AST_NONE;
}

Ast* Parser::a_num_expr() {
  auto left = a_bracketed();
  if (!left) left = a_lit_int();
  if (!left) return nullptr;

  auto mid = op();
  if (!mid) return left;

  auto right = a_num_expr();
  if (!right) return nullptr;

  return new Ast { .type = mid, .left = left, .right = right };
}

Ast* Parser::a_bracketed() {
  auto open = t_expect(TK_PAREN_OPEN);
  if (!open) {
    return nullptr;
  }

  auto raw = a_num_expr();
  auto close = t_expect(TK_PAREN_CLOSE);
  if (close) {
    return raw;
  } else {
    printf("'(' with no matching ')'");
    return nullptr;
  }
}
