#include <fstream>
#include "lexer.h"


const std::size_t MAX_FILE_SIZE = 1 << 20;

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

bool is_ok_after_number(char c) {
  switch (c) {
    case '+':
    case ')':
      return true;
  }
  return is_whitespace(c);
}


Lexer::Lexer() {
  _chars = new char[MAX_FILE_SIZE];
  _tokens = new Token[MAX_FILE_SIZE / 16];
}

Lexer::~Lexer() {
  delete[] _chars;
  delete[] _tokens;
}

void Lexer::lex_file(char const* path) {
  std::ifstream file(path);

  _cursor = 0;
  file.read(_chars, MAX_FILE_SIZE);
  _nchars = file.gcount();  
  _chars[_nchars] = 0;

  if (_chars == 0) {
    printf("ERROR: Can't read file %s.\n", path);
  }

  _ntokens = 0;
  for (; next(); _ntokens++);
}

char Lexer::peek(int offset) {
  if ((_cursor + offset) < _nchars) {
    return _chars[_cursor + offset];
  }
  return 0;
}

char Lexer::expect(char expected) {
  if (peek(0) == expected) {
    _cursor++;
    return expected;
  }
  return 0;
}

bool Lexer::number(int* number) {
  auto cursor = _cursor;
  *number = 0;
  int digits = 0;
  while (true) {
    char c = peek(0);
    
    if (c >= '0' && c <= '9') {
      *number *= 10;
      *number += c - '0';
      digits++;
    } else if (!c || is_ok_after_number(c)) {      
      return digits > 0;
    } else if (digits > 0) {
      printf("ERROR: expected digit, whitespace or operator.\n");
      _cursor = cursor;
      return false;
    } else {
      _cursor = cursor;
      return false;
    }

    _cursor++;
  }
}

void Lexer::skip_whitespace() {
  for (; is_whitespace(peek(0)); _cursor++);
}

Token* Lexer::next() {
  skip_whitespace();

  if (!peek(0)) return nullptr;

  if (expect('(')) return new (_tokens + _ntokens) Token { .type = TK_PAREN_OPEN };
  if (expect(')')) return new (_tokens + _ntokens) Token { .type = TK_PAREN_CLOSE };
  if (expect('+')) return new (_tokens + _ntokens) Token { .type = TK_OP_PLUS };
  
  int n;
  if (number(&n)) return new (_tokens + _ntokens) Token { .type = TK_LIT_INT, .int_value = n };

  // error, expected op or number
  printf("ERROR: expected + or number.\n");
  return nullptr;
}

Token* Lexer::token(int i) {
  if (i < _ntokens) {
    return _tokens + i;
  }
  return nullptr;
}
