#ifndef TOKEN_H
#define TOKEN_H
#include "common/span.hpp"
#include <iostream>
#include <vector>

/// Refers to how many tokens to reserve in the collection vector upon creation.
constexpr unsigned short INIT_TOKEN_RESERVE_SIZE = 64;

/* -------------------------------------------------------------------------- */
/* TOKEN DEFINITIONS */
/* -------------------------------------------------------------------------- */

#define TOKEN_LIST                                                             \
  X(LParen, "(")                                                               \
  X(RParen, ")")                                                               \
  X(LCurl, "{")                                                                \
  X(RCurl, "}")                                                                \
  X(LBrac, "[")                                                                \
  X(RBrac, "]")                                                                \
                                                                               \
  X(Plus, "+")                                                                 \
  X(PlusPlus, "++")                                                            \
  X(PlusEqual, "++=")                                                          \
  X(Minus, "-")                                                                \
  X(MinusMinus, "--")                                                          \
  X(MinusEqual, "-=")                                                          \
  X(Star, "*")                                                                 \
  X(StarStar, "**")                                                            \
  X(StarEqual, "*=")                                                           \
  X(StarStarEqual, "**=")                                                      \
  X(Slash, "/")                                                                \
  X(SlashSlash, "//")                                                          \
  X(SlashEqual, "/=")                                                          \
  X(SlashSlashEqual, "//=")                                                    \
                                                                               \
  X(And, "&")                                                                  \
  X(AndAnd, "&&")                                                              \
  X(Bar, "|")                                                                  \
  X(BarBar, "||")                                                              \
  X(Less, "<")                                                                 \
  X(LessEqual, "<=")                                                           \
  X(More, ">")                                                                 \
  X(MoreEqual, ">=")                                                           \
  X(Bang, "!")                                                                 \
  X(BangEqual, "!=")                                                           \
  X(Equal, "=")                                                                \
  X(EqualEqual, "==")                                                          \
                                                                               \
  X(Dot, ".")                                                                  \
  X(Comma, ",")                                                                \
  X(Colon, ":")                                                                \
  X(Semicolon, ";")                                                            \
  X(Question, "?")                                                             \
  X(Percent, "%")                                                              \
                                                                               \
  X(Function, "function")                                                      \
  X(If, "if")                                                                  \
  X(Else, "else")                                                              \
  X(For, "for")                                                                \
  X(Break, "break")                                                            \
  X(Continue, "continue")                                                      \
                                                                               \
  X(Identifier, "identifier")                                                  \
  X(Decimal, "decimal")                                                        \
  X(Integer, "integer")                                                        \
  X(String, "string")                                                          \
  X(Boolean, "boolean")                                                        \
                                                                               \
  X(Eof, "<eof>")                                                              \
  X(Newline, "<\\n>")

/* -------------------------------------------------------------------------- */
/* TOKEN */
/* -------------------------------------------------------------------------- */

/// Reprents one token. To get the lexeme and/or raw (unparsed) literal
/// values, use `span.lexeme()` to get a string view.
struct Token {

  /// Represents some variant of a token, including the operators, literals,
  /// keywords, and meta types.
  enum class Kind {
#define X(name, repr) name,
    TOKEN_LIST
#undef X
  };

  const Kind kind;
  const Span span;

  Token(Kind kind, Span span);
};

/* -------------------------------------------------------------------------- */
/* TOKEN COLLECTION */
/* -------------------------------------------------------------------------- */

/// A basic `std::vector<Token>` wrapper used as a simple and portable
/// collection of tokens that can be passed around the compiler.
class TokenCollect {
  const Source &source;
  std::vector<Token> items;

public:
  TokenCollect(const Source &source);
  auto begin() const;
  auto end() const;
  size_t size() const;
  Token eof() const;

  /// Returns a const reference to the underlying vector in the collection.
  const std::vector<Token> &data() const;

  /// Pushes the given token to the vector.
  void push(const Token &token);

  /// Prints a numbered list of all the tokens currently stored to the
  /// `std::cout` stream.
  void print_all() const;
};

/* -------------------------------------------------------------------------- */
/* STREAM INSERTION OVERLOADS */
/* -------------------------------------------------------------------------- */

std::ostream &operator<<(std::ostream &os, const Token &token);

#endif