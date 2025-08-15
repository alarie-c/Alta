#include "common/operator.hpp"
#include <cstdint>

namespace operators {

constexpr Flag operator|(Flag lhs, Flag rhs) {
  return static_cast<Flag>(static_cast<uint8_t>(lhs) |
                           static_cast<uint8_t>(rhs));
}

constexpr Flag operator&(Flag lhs, Flag rhs) {
  return static_cast<Flag>(static_cast<uint8_t>(lhs) &
                           static_cast<uint8_t>(rhs));
}

constexpr Flag get_flag_from_kind(const Kind &kind) {
#define X(name, repr, tok, flags)                                              \
  case Kind::name:                                                             \
    return flags;
  switch (kind) {
    OPERATOR_LIST
  default:
    return Flag::None;
  }
#undef X
}

Operator::Operator(Kind kind)
    : kind(kind), flags(get_flag_from_kind(this->kind)) {}

std::ostream &operator<<(std::ostream &os, const Operator &op) {
  os << "op(";
#define X(name, repr, tok, flags)                                              \
  case Kind::name:                                                             \
    os << repr;                                                                \
    break;
  switch (op.kind) {
    OPERATOR_LIST
  default:
    os << "?";
  }
#undef X
  return os << ")";
}

}; // namespace operators