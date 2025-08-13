#ifndef DIAGNOSTIC_H
#define DIAGNOSTIC_H
#include "common/span.hpp"
#include <iostream>
#include <string>
#include <vector>

#define DIAGNOSTIC_ISSUES                                                      \
  X(InvalidCharacter, "invalid character", Error)                              \
  X(InvalidString, "invalid string", Error)                                    \
  X(UnterminatedString, "unterminated string", Error)                          \
  X(InternalError, "internal error", Error)

/// Used to represent some kind of compiler error that should be emitted to the
/// user. Not all diagnostics are errors, some may be warnings or just info
/// which is configured with the `level` member. `Diagnostics` also support
/// stream insertion.
struct Diagnostic {
  /// Represents the three different levels that a diagnostic can take on.
  /// `Error` is the only level that can abort compilation.
  enum class Level { Error, Warning, Info };

  /// Refers to the specific problem this diagnostic is reporting about, defined
  /// by the `DIAGNOSTIC_ISSUES` X-macro.
  enum class Issue {
#define X(name, repr, level) name,
    DIAGNOSTIC_ISSUES
#undef X
  };

  const Level level;
  const Issue issue;
  const Span span;
  const std::string message;

  Diagnostic(Issue issue, const Span &span, std::string message);
};

/// Basic `std::vector<Diagnostic>` wrapper that is used to collect errors.
/// Basically just gives a short-hand way of moving the errors around the
/// compiler. All the phases should take this collection by reference.
class DiagCollect {
  std::vector<Diagnostic> items;

public:
  DiagCollect();
  [[nodiscard]] auto begin() const;
  [[nodiscard]] auto end() const;

  /// Pushes the given diagnostic to the internal diagnostic vector.
  void push(const Diagnostic &diag);

  /// Prints all of the currently stored diagnostics to `std::cout`.
  void print_all() const;
};

/* -------------------------------------------------------------------------- */
/* STREAM INSERTION OVERLOADS */
/* -------------------------------------------------------------------------- */

std::ostream &operator<<(std::ostream &os, const Diagnostic &diag);
std::ostream &operator<<(std::ostream &os, const Diagnostic::Level &level);
std::ostream &operator<<(std::ostream &os, const Diagnostic::Issue &issue);

#endif