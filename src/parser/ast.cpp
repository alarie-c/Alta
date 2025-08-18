#include "parser/ast.hpp"
#include "common/operator.hpp"
#include <iostream>
#include <string>

namespace ast {
using Operator = operators::Operator;

void Int::print(unsigned indent) const {
  const std::string spaces(indent, ' ');
  std::cout << spaces << "INT(" << value << ")" << std::endl;
}

void Float::print(unsigned indent) const {
  const std::string spaces(indent, ' ');
  std::cout << spaces << "FLOAT(" << value << ")" << std::endl;
}

void Binary::print(unsigned indent) const {
  const std::string spaces(indent, ' ');
  std::cout << spaces << "BINARY " << op << std::endl;
  lhs.print(indent + 4);
  rhs.print(indent + 4);
}

void Node::print(unsigned indent) const {
  const std::string spaces(indent, ' ');
  std::cout << spaces << span << " ";

  switch (kind) {
  case Kind::Int: {
    data.node_int.print(0);
    break;
  }
  case Kind::Float: {
    data.node_float.print(0);
    break;
  }
  case Kind::Binary: {
    data.node_binary.print(0);
    break;
  }
  default: {
    std::cout << spaces << "<unknown node>" << std::endl;
  }
  }
}

Node::Node(Kind kind, Data data, Span span)
    : kind(std::move(kind)), data(std::move(data)), span(std::move(span)) {}

}; // namespace ast