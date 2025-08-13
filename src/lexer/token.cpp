#include "lexer/token.hpp"
#include "common/span.hpp"

Token::Token(Kind kind, Span span) : kind(kind), span(span) {}

TokenCollect::TokenCollect() : items({}) {
  items.reserve(INIT_TOKEN_RESERVE_SIZE);
}

const std::vector<Token> &TokenCollect::data() const { return items; }
auto TokenCollect::begin() const { return items.begin(); }
auto TokenCollect::end() const { return items.end(); }
void TokenCollect::push(const Token &token) { items.push_back(token); }

void TokenCollect::print_all() const {
  for (auto &t : *this) {
    std::cout << t << "\n";
  }
  std::cout << std::endl;
}

std::ostream &operator<<(std::ostream &os, const Token &token) {
  os << token.span << " [";

#define X(name, repr)                                                          \
  case Token::Kind::name:                                                      \
    os << repr;                                                                \
    break;
  switch (token.kind) {
    TOKEN_LIST
  default:
    os << "<unknown kind>";
  }
#undef X

  os << "]";

  if (token.kind == Token::Kind::Identifier ||
      token.kind == Token::Kind::Decimal ||
      token.kind == Token::Kind::Integer || token.kind == Token::Kind::String) {
    const auto lexeme = token.span.lexeme();
    os << " '" << lexeme << "'";
  }
  return os;
}