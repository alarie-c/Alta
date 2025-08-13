#ifndef DIAGNOSTIC_H
#define DIAGNOSTIC_H
#include "common/span.hpp"
#include <iostream>
#include <string>
#include <vector>

namespace diagnostic {

enum class Severity { Error, Warning, Info };

#define DIAGNOSTIC_KINDS                                                       \
  X(InvalidCharacter, "invalid character", Error)                              \
  X(InvalidString, "invalid string", Error)                                    \
  X(UnterminatedString, "unterminated string", Error)

enum class Kind {
#define X(name, repr, severity) name,
  DIAGNOSTIC_KINDS
#undef X
};

struct Diagnostic {
  const Severity severity;
  const Kind kind;
  const Span span;
  const std::string message;

  Diagnostic(Kind kind, const Span &span, std::string message);
};

std::ostream &operator<<(std::ostream &os, const Diagnostic &diag);
std::ostream &operator<<(std::ostream &os, const Diagnostic &severity);
std::ostream &operator<<(std::ostream &os, const Diagnostic &kind);

class Collection {
  std::vector<Diagnostic> items;

public:
  Collection();
  auto begin() const;
  auto end() const;

  void push(const Diagnostic &diag);
  void print_all() const;
};

}; // namespace diagnostic

#endif