#ifndef AST_H
#define AST_H
#include "common/operator.hpp"
#include "common/span.hpp"
#include <cstdint>

namespace ast {
using Operator = operators::Operator;

struct Node;
struct Int;
struct Float;
struct Binary;

enum class Kind {
  Int,
  Float,
  Binary,
};

struct Int {
  int32_t value;
  void print(unsigned indent = 0) const;
};

struct Float {
  double value;
  void print(unsigned indent = 0) const;
};

struct Binary {
  Node &lhs;
  Node &rhs;
  Operator op;
  void print(unsigned indent = 0) const;
};

union Data {
  Int node_int;
  Float node_float;
  Binary node_binary;
};

struct Node {
  const Kind kind;
  const Data data;
  const Span span;
  Node(Kind kind, Data data, Span span);
  void print(unsigned indent = 0) const;
};

}; // namespace ast

#endif