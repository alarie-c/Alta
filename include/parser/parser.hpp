#ifndef PARSER_H
#define PARSER_H
#include "common/diagnostic.hpp"
#include "common/span.hpp"
#include "lexer/token.hpp"
#include "parser/ast.hpp"
#include <optional>

class Parser {
  const Source &source;
  const TokenCollect &tokens;
  DiagCollect &diagnostics;
  size_t cursor;

public:
  Parser(const Source &source, const TokenCollect &tokens,
         DiagCollect &diagnostics);
  void parse();

private:
  bool is_at_end(unsigned k = 0);
  Token peek(unsigned k = 1);
  Token current();

public:
  std::optional<ast::Node> parse_primary();
  std::optional<ast::Node> parse_expr();
};

#endif