#ifndef LEXER_H
#define LEXER_H
#include "common/diagnostic.hpp"
#include "common/span.hpp"
#include "lexer/token.hpp"
#include <expected>
#include <vector>

class Lexer {
  const Source &source;
  DiagCollect &diagnostics;
  TokenCollect &tokens;
  size_t cursor;

private:
  bool is_at_end(unsigned k = 0) const;

  char current() const;
  char peek(unsigned k = 1) const;
  void eat(unsigned k = 1);

  void push(Token token);

  void skip_whitespace();
  Token lex_identifier();
  Token lex_number();
  std::expected<Token, Diagnostic> lex_once();
  // std::expected<Token, Diagnostic> lex_string();

public:
  Lexer(const Source &source, TokenCollect &tokens, DiagCollect &diagnostics);
  void lex();
};

#endif