#ifndef TOKEN_H
#define TOKEN_H
#include "common/span.hpp"
#include <iostream>
#include <vector>

#define INIT_TOKEN_RESERVE_SIZE 64
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

struct Token {
  enum class Kind {
#define X(name, repr) name,
    TOKEN_LIST
#undef X
  };

  const Kind kind;
  const Span span;

  Token(Kind kind, Span span);
};

class TokenCollect {
  std::vector<Token> items;

public:
  TokenCollect();
  auto begin() const;
  auto end() const;
  const std::vector<Token> &data() const;

  void push(const Token &token);
  void print_all() const;
};

std::ostream &operator<<(std::ostream &os, const Token &token);

#endif