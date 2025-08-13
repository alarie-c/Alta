#include "lexer/lexer.hpp"
#include "common/diagnostic.hpp"
#include "common/span.hpp"
#include "lexer/token.hpp"
#include <cctype>
#include <optional>
#include <string_view>

/* -------------------------------------------------------------------------- */
/* NON-ASSOCIATED HELPERS */
/* -------------------------------------------------------------------------- */

Token::Kind keyword_or_identifier(const std::string_view &sv) {
  using enum Token::Kind;

  if (sv == "if")
    return If;
  if (sv == "else")
    return Else;
  if (sv == "function")
    return Function;
  if (sv == "for")
    return For;
  if (sv == "break")
    return Break;
  if (sv == "continue")
    return Continue;

  // default to identifier
  return Identifier;
}

bool is_ident_start(const char &ch) { return std::isalpha(ch) || ch == '_'; }
bool is_ident_cont(const char &ch) { return std::isalnum(ch) || ch == '_'; }
bool is_number_start(const char &ch) { return std::isdigit(ch); }
bool is_number_cont(const char &ch) { return std::isdigit(ch) || ch == '.'; }
bool is_whitespace(const char &ch) {
  return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\v' || ch == '\b';
}

/* -------------------------------------------------------------------------- */
/* ASSOCIATED HELPERS */
/* -------------------------------------------------------------------------- */

Lexer::Lexer(const Source &source, TokenCollect &tokens,
             DiagCollect &diagnostics)
    : source(source), diagnostics(diagnostics), tokens(tokens), cursor(0) {}

bool Lexer::is_at_end(unsigned k) const { return cursor + k >= source.size; }
void Lexer::skip_whitespace() {
  while (is_whitespace(current()))
    eat();
}

char Lexer::current() const { return peek(0); }
char Lexer::peek(unsigned k) const {
  if (is_at_end(k))
    return '\0';
  return source.content.at(cursor + k);
}

void Lexer::eat(unsigned k) {
  if (is_at_end(k))
    cursor = source.size;
  cursor += k;
}

/* -------------------------------------------------------------------------- */
/* LEXER METHODS */
/* -------------------------------------------------------------------------- */

Token Lexer::lex_identifier() {
  const auto start = cursor;

  // Keep consuming while the NEXT character is a valid ident
  while (is_ident_cont(peek())) {
    eat();
  }

  // Now the next character is invalid, push a token and the next cycle will
  // handle the invalid
  const Span span(source, start, cursor - start + 1);
  const auto sv = span.lexeme();
  const auto kind = keyword_or_identifier(sv);
  return Token(kind, span);
}

Token Lexer::lex_number() {
  const auto start = cursor;
  auto kind = Token::Kind::Integer;

  while (is_number_cont(peek())) {
    if (peek() == '.') {
      if (kind == Token::Kind::Integer && is_number_start(peek(2))) {
        // This implies the dot is part of this token and the thing after the
        // dot is also a valid number
        kind = Token::Kind::Decimal;
      } else {
        // This implies the dot is not part of this token, so break here
        break;
      }
    }
    eat();
  }

  // Now the next character is invalid, push a token and the next cycle will
  // handle the invalid
  const Span span(source, start, cursor - start + 1);
  return Token(kind, span);
}

// std::expected<Token, Diagnostic> Lexer::lex_string();
std::optional<Token> Lexer::lex_once() {
  using enum Token::Kind;

  // Skip so the current thing is meaningful character
  skip_whitespace();
  const auto start = cursor;
  const auto ch = peek(0);

  // Early return on the EOF case
  if (is_at_end())
    return Token(Eof, Span(source, cursor, 1));
  if (is_ident_start(ch))
    return lex_identifier();
  if (is_number_start(ch))
    return lex_number();

  const auto peek1 = peek(1);
  const auto peek2 = peek(2);

  switch (ch) {

  case '\n':
    return Token(Newline, Span(source, cursor, 1));
  case '(':
    return Token(LParen, Span(source, cursor, 1));
  case ')':
    return Token(RParen, Span(source, cursor, 1));
  case '[':
    return Token(LBrac, Span(source, cursor, 1));
  case ']':
    return Token(RBrac, Span(source, cursor, 1));
  case '{':
    return Token(LCurl, Span(source, cursor, 1));
  case '}':
    return Token(RCurl, Span(source, cursor, 1));
  case '.':
    return Token(Dot, Span(source, cursor, 1));
  case ',':
    return Token(Comma, Span(source, cursor, 1));
  case ':':
    return Token(Colon, Span(source, cursor, 1));
  case ';':
    return Token(Semicolon, Span(source, cursor, 1));
  case '?':
    return Token(Question, Span(source, cursor, 1));
  case '%':
    return Token(Percent, Span(source, cursor, 1));

  case '+': {
    if (peek1 == '+') {
      eat();
      return Token(PlusPlus, Span(source, start, 2));
    }
    if (peek1 == '=') {
      eat();
      return Token(PlusEqual, Span(source, start, 2));
    }
    return Token(Plus, Span(source, start, 1));
  }
  case '-': {
    if (peek1 == '-') {
      eat();
      return Token(MinusMinus, Span(source, start, 2));
    }
    if (peek1 == '=') {
      eat();
      return Token(MinusEqual, Span(source, start, 2));
    }
    return Token(Minus, Span(source, start, 1));
  }
  case '*': {
    if (peek1 == '*' && peek2 == '=') {
      eat(2);
      return Token(StarStarEqual, Span(source, start, 3));
    }
    if (peek1 == '*') {
      eat();
      return Token(StarStar, Span(source, start, 2));
    }
    if (peek1 == '=') {
      eat();
      return Token(StarEqual, Span(source, start, 2));
    }
    return Token(Star, Span(source, start, 1));
  }
  case '/': {
    if (peek1 == '/' && peek2 == '=') {
      eat(2);
      return Token(SlashSlashEqual, Span(source, start, 3));
    }
    if (peek1 == '/') {
      eat();
      return Token(SlashSlash, Span(source, start, 2));
    }
    if (peek1 == '=') {
      eat();
      return Token(SlashEqual, Span(source, start, 2));
    }
    return Token(Slash, Span(source, start, 1));
  }

  case '<': {
    if (peek1 == '=') {
      eat();
      return Token(LessEqual, Span(source, start, 2));
    }
    return Token(Less, Span(source, start, 1));
  }
  case '>': {
    if (peek1 == '=') {
      eat();
      return Token(MoreEqual, Span(source, start, 2));
    }
    return Token(More, Span(source, start, 1));
  }
  case '!': {
    if (peek1 == '=') {
      eat();
      return Token(BangEqual, Span(source, start, 2));
    }
    return Token(Bang, Span(source, start, 1));
  }
  case '=': {
    if (peek1 == '=') {
      eat();
      return Token(EqualEqual, Span(source, start, 2));
    }
    return Token(Equal, Span(source, start, 1));
  }

  case '&': {
    if (peek1 == '&') {
      eat();
      return Token(AndAnd, Span(source, start, 2));
    }
    return Token(And, Span(source, start, 1));
  }
  case '|': {
    if (peek1 == '|') {
      eat();
      return Token(BarBar, Span(source, start, 2));
    }
    return Token(Bar, Span(source, start, 1));
  }
  }

  // If nothing else matches it's an illegal character
  const auto diag = Diagnostic(Diagnostic::Issue::InvalidCharacter,
                               Span(source, start, cursor - start + 1),
                               "This character is not allowed.");
  diagnostics.push(diag);
  return std::nullopt;
}

/* -------------------------------------------------------------------------- */
/* MAIN LOOP */
/* -------------------------------------------------------------------------- */

void Lexer::lex() {
  while (true) {
    const auto maybe_token = lex_once();

    // Check if the token is valid
    if (maybe_token.has_value()) {
      const auto token = maybe_token.value();
      tokens.push(token);

      // Break out of this loop if the last token is found
      if (token.kind == Token::Kind::Eof) {
        break;
      }
    }
    // Skip to next char
    eat();
  }
}