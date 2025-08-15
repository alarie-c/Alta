#ifndef OPERATOR_H
#define OPERATOR_H
#include <cstdint>
#include <iostream>

namespace operators {

enum class Flag : uint8_t {
  None = 0,
  Assign = 1 << 0,
  Binary = 1 << 1,
  Unary = 1 << 2,
  Logical = 1 << 3
};

constexpr Flag operator|(Flag lhs, Flag rhs);
constexpr Flag operator&(Flag lhs, Flag rhs);

#define OPERATOR_LIST                                                          \
  X(Add, "+", Token::Kind::Plus, Flag::Binary)                                 \
  X(Sub, "-", Token::Kind::Minus, Flag::Binary | Flag::Unary)                  \
  X(Mul, "*", Token::Kind::Star, Flag::Binary)                                 \
  X(Div, "/", Token::Kind::Slash, Flag::Binary)                                \
  X(Exp, "**", Token::Kind::StarStar, Flag::Binary)                            \
  X(Assign, "=", Token::Kind::Equal, Flag::Assign)

enum class Kind {
#define X(name, repr, tok, flags) name,
  OPERATOR_LIST
#undef X
};

constexpr Flag get_flag_from_kind(const Kind &kind);

struct Operator {
  Kind kind;
  Flag flags;
  Operator(Kind kind);
};

std::ostream &operator<<(std::ostream &os, const Operator &op);

}; // namespace operators

#endif