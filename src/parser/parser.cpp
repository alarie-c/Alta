#include "parser/parser.hpp"
#include "common/diagnostic.hpp"
#include "common/span.hpp"
#include "lexer/token.hpp"
#include "parser/ast.hpp"
#include <cstdint>
#include <expected>
#include <optional>
#include <string_view>

/* -------------------------------------------------------------------------- */
/* PARSER HELPERS */
/* -------------------------------------------------------------------------- */

std::expected<int32_t, Diagnostic> parse_int32(const std::string_view &str,
                                               Span span) {
  try {
    int32_t num = std::stoi(std::string(str));
    return num;
  } catch (...) {
    std::unexpected(Diagnostic(Diagnostic::Issue::InternalError, span,
                               "Invalid integer literal."));
  }
  return 0;
}

std::expected<double, Diagnostic> parse_float(const std::string_view &str,
                                              Span span) {
  try {
    double num = std::stod(std::string(str));
    return num;
  } catch (...) {
    std::unexpected(Diagnostic(Diagnostic::Issue::InternalError, span,
                               "Invalid floating point literal."));
  }
  return 0;
}

/* -------------------------------------------------------------------------- */
/* PARSER IMPLEMENTATION */
/* -------------------------------------------------------------------------- */

Parser::Parser(const Source &source, const TokenCollect &tokens,
               DiagCollect &diagnostics)
    : source(source), tokens(tokens), diagnostics(diagnostics), cursor(0) {}

bool Parser::is_at_end(const unsigned k) {
  if (cursor + k >= tokens.size()) {
    return true;
  }
  return false;
}
Token Parser::peek(const unsigned k) {
  if (is_at_end(k)) {
    return tokens.eof();
  }
  return tokens.data()[cursor + k];
}
Token Parser::current() { return peek(0); }

void Parser::parse() { return; }

/* -------------------------------------------------------------------------- */
/* EXPRESSION PARSERS */
/* -------------------------------------------------------------------------- */

// Top level parse_expr() method points to the highest level precedence
// expression parser.
std::optional<ast::Node> Parser::parse_expr() { return parse_primary(); }

std::optional<ast::Node> Parser::parse_primary() {
  using enum Token::Kind;
  const auto token = current();
  // std::cout << token << std::endl;

  switch (token.kind) {

  // Integer parsing and conversion
  case Integer: {
    const auto value = parse_int32(token.span.lexeme(), token.span);
    if (value.has_value()) {
      return ast::Node(ast::Kind::Int, ast::Data{.node_int = {value.value()}},
                       token.span);
    }
    diagnostics.push(value.error());
    return std::nullopt;
  }

  // Floating point parsing and conversion
  case Decimal: {
    const auto value = parse_float(token.span.lexeme(), token.span);
    if (value.has_value()) {
      return ast::Node(ast::Kind::Float,
                       ast::Data{.node_float = {value.value()}}, token.span);
    }
    diagnostics.push(value.error());
    return std::nullopt;
  }

  // If none of those matched then throw the infamous 'expected expression'
  // error
  default: {
    const Diagnostic diag(Diagnostic::Issue::ExpectedExpression, token.span,
                          "Expected an expression here.");
    diagnostics.push(diag);
    return std::nullopt;
  }
  }
}
