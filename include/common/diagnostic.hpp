#ifndef DIAGNOSTIC_H
#define DIAGNOSTIC_H
#include "common/span.hpp"
#include <iostream>
#include <string>
#include <vector>

#define DIAGNOSTIC_KINDS                                                       \
  X(InvalidCharacter, "invalid character", Error)                              \
  X(InvalidString, "invalid string", Error)                                    \
  X(UnterminatedString, "unterminated string", Error)

struct Diagnostic {
  enum class Severity { Error, Warning, Info };

  enum class Kind {
#define X(name, repr, severity) name,
    DIAGNOSTIC_KINDS
#undef X
  };

  const Severity severity;
  const Kind kind;
  const Span span;
  const std::string message;

  Diagnostic(Kind kind, const Span &span, std::string message);
};

std::ostream &operator<<(std::ostream &os, const Diagnostic &diag);
std::ostream &operator<<(std::ostream &os, const Diagnostic &severity);
std::ostream &operator<<(std::ostream &os, const Diagnostic &kind);

class DiagCollect {
  std::vector<Diagnostic> items;

public:
  DiagCollect();
  auto begin() const;
  auto end() const;

  void push(const Diagnostic &diag);
  void print_all() const;
};

#endif