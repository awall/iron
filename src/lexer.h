#pragma once
#include <iostream>
#include <fstream>


enum TokenType {
  TK_LIT_INT,
  TK_OP_PLUS,
  TK_PAREN_OPEN,
  TK_PAREN_CLOSE,
};

struct Token {
  TokenType type;
  union {
    int int_value;
  };
};

class Lexer {
private:

  int _cursor;
  char* _chars; int _nchars;
  Token* _tokens; int _ntokens;

  char peek(int offset);
  char expect(char expected);
  bool number(int*);
  void skip_whitespace();

  Token* next();

public:

  Lexer();
  ~Lexer();

  void lex_file(char const* path);
  Token* token(int i);

};
