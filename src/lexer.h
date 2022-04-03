#pragma once
#include <string>
#include <unordered_set>


enum TokenType {
  TK_NONE = 0,

  TK_ATOM,
  TK_LIT_INT,
  TK_OP_PLUS,
  TK_OP_MINUS,
  TK_OP_MULT,
  TK_OP_EQ,
  TK_PAREN_BEG,
  TK_PAREN_END,
  TK_SCOPE_BEG,
  TK_SCOPE_END,
  TK_STATEMENT_END,
};

typedef std::string Atom;

struct Token {
  TokenType type;
  union {
    int int_value;
    Atom* atom;
  };
};


class Lexer {
private:

  char* _chars; char* _here;
  Token* _tokens; size_t _ntokens = 0;

  Token next();

public:

  Lexer();
  ~Lexer();

  void lex_file(char const* path);
  Token* token(int i);

};
