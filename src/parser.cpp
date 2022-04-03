#include <cstdio>
#include "parser.h"


const size_t MAX_NODES = 1 << 20;

Parser::Parser() {
  _nodes = new Ast[MAX_NODES];
}

Parser::~Parser() {
  delete[] _nodes;
}

Ast* Parser::parse_file(const char* path) {
  _lexer.lex_file(path);
  return a_naked_block();
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
    return new (_nodes + _nnodes++) Ast { .span = 1, .type = AST_LIT_INT, .int_value = token->int_value };
  }
  return nullptr;
}

Ast* Parser::a_atom() {
  auto token = t_expect(TK_ATOM);
  if (token) {
    return new (_nodes + _nnodes++) Ast { .span = 1, .type = AST_ATOM, .atom = token->atom };
  }
  return nullptr;
}

AstType Parser::op() {
  if (t_expect(TK_OP_EQ))    return AST_OP_EQ;
  if (t_expect(TK_OP_PLUS))  return AST_OP_PLUS;
  if (t_expect(TK_OP_MINUS)) return AST_OP_MINUS;
  if (t_expect(TK_OP_MULT))  return AST_OP_MULT;
  return AST_NONE;
}

Ast* Parser::a_expr() {
  auto pre = op(); 
  auto left = a_bracketed();
  if (!left) left = a_block();
  if (!left) left = a_lit_int();
  if (!left) left = a_atom();
  if (!left) return nullptr;
  if (pre) left = new (_nodes + _nnodes++) Ast { .nkids = 1, .span = 1 + left->span, .type = pre, .kid = left };

  auto mid = op();
  if (!mid) return left;

  auto right = a_expr();
  if (!right) {
    _nnodes -= left->span;
    printf("ERROR: operator has no right side");
    return nullptr;
  }
  
  left->next = right;
  return new (_nodes + _nnodes++) Ast { .nkids = 2, .span = 1 + left->span + right->span, .type = mid, .kid = left };
}

Ast* Parser::a_bracketed() {
  auto open = t_expect(TK_PAREN_BEG);
  if (!open) return nullptr;

  auto raw = a_expr();
  auto close = t_expect(TK_PAREN_END);
  if (!close) {
    if (raw) _nnodes -= raw->span;
    printf("ERROR: '(' with no matching ')'");
    return nullptr;
  }

  return raw;
}

Ast* Parser::a_block() {
  auto open = t_expect(TK_SCOPE_BEG);
  if (!open) return nullptr;

  auto statements = a_statements();

  auto close = t_expect(TK_SCOPE_END);
  if (!close) {
    printf("ERROR: '{' with no matching '}'");
    return nullptr;
  }

  return statements;
}

Ast* Parser::a_statements() {
  int nkids = 0;
  int span = 1;
  Ast* first = nullptr;
  Ast* last = nullptr;
  while (true) {
    auto expr = a_expr();
    if (!expr) break;
    if (!t_expect(TK_STATEMENT_END)) {
      printf("ERROR: statements must end with ';'");  
      return nullptr;
    }

    if (!first) first = expr;
    if (last) last->next = expr;
    last = expr;
    span += expr->span;
    nkids++;    
  }

  return new (_nodes + _nnodes++) Ast { .nkids = nkids, .span = span, .type = AST_BLOCK, .kid = first };
}