#include <fstream>
#include "lexer.h"


const size_t MAX_FILE_SIZE = 1 << 20;
const size_t MAX_TOKENS = 1 << 15;

bool is_operator(char c) {
  switch (c) {
    case '+':
    case '-':
    case '*':
    case '(':
    case ')':
    case '=':
      return true;
  }
  return false;
}

bool is_whitespace(char c) {
  switch (c) {
    case '\n':
    case '\r':
    case '\t':
    case ' ':
      return true;
  }
  return false;
}

bool is_digit(char c) {
  return c <= '9' && c >= '0';
}

bool is_alpha(char c) {
  return (c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A');
}

bool is_alphanumeric(char c) {
  return is_digit(c) || is_alpha(c);
}

bool is_atomic(char c) {
  return is_alphanumeric(c) || c == '_';
}

bool ends_word(char c) {
  switch (c) {
    case '{':
    case '}':
    case ';':
      return true;
  }
  return !c || is_operator(c) || is_whitespace(c);
}

Lexer::Lexer() {
  _chars = new char[MAX_FILE_SIZE];
  _tokens = new Token[MAX_TOKENS];
}

Lexer::~Lexer() {
  delete[] _tokens;
  delete[] _chars;
}

void Lexer::lex_file(char const* path) {
  _here = _chars;
  _ntokens = 0;

  std::ifstream file(path);  
  file.read(_chars, MAX_FILE_SIZE);
  auto nchars = file.gcount();
  _chars[nchars] = 0;

  if (nchars == 0) {
    printf("ERROR: Can't read file %s.\n", path);
    return;
  }
  
  while (true) {
    while (is_whitespace(*_here)) _here++;
    if (!*_here) break;
    _tokens[_ntokens++] = next();
  }
}

Token Lexer::next() {
  char c = *_here;
  _here++;
  if (c == '(') return Token { .type = TK_PAREN_BEG };
  if (c == ')') return Token { .type = TK_PAREN_END };
  if (c == '{') return Token { .type = TK_SCOPE_BEG };
  if (c == '}') return Token { .type = TK_SCOPE_END };
  if (c == ';') return Token { .type = TK_STATEMENT_END };
  if (c == '+') return Token { .type = TK_OP_PLUS };
  if (c == '-') return Token { .type = TK_OP_MINUS };
  if (c == '*') return Token { .type = TK_OP_MULT };
  if (c == '=') return Token { .type = TK_OP_EQ };
  _here--;

  int number = 0;
  Atom* atom = new Atom;
  bool is_number = true;
  bool is_atom = true;
  while (true) {
    char c = *_here;

    if (ends_word(c)) {
      if (is_number) return Token { .type = TK_LIT_INT, .int_value = number };
      if (is_atom)   return Token { .type = TK_ATOM,    .atom      = atom   };

      printf("ERROR: %s is not an operator, an atom, or a number.\n", atom->c_str());
      return Token { .type = TK_NONE };
    }
    _here++;
    is_number &= is_digit(c);
    is_atom &= is_atomic(c); 

    number *= 10;
    number += c - '0';
    
    atom->push_back(c);
  }
}

Token* Lexer::token(int i) {
  return i < _ntokens ? _tokens + i : nullptr;
}
