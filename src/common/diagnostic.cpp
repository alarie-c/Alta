#include "common/diagnostic.hpp"
#include "common/span.hpp"
#include <iostream>
#include <string>
#include <utility>

/* -------------------------------------------------------------------------- */
/* STREAM INSERTION OVERLOADS */
/* -------------------------------------------------------------------------- */

std::ostream &operator<<(std::ostream &os, const Diagnostic::Level &level) {
  using enum Diagnostic::Level;
  switch (level) {
  case Error:
    return os << "Error:";
  case Warning:
    return os << "Warning:";
  case Info:
    return os << "Info:";
  default:
    return os << "<unknown severity>";
  }
}

std::ostream &operator<<(std::ostream &os, const Diagnostic::Issue &issue) {
  using enum Diagnostic::Issue;
#define X(name, repr, issue)                                                   \
  case name:                                                                   \
    return os << repr;
  switch (issue) {
    DIAGNOSTIC_ISSUES
  default:
    return os << "<unknown kind>";
  }
#undef X
}

std::ostream &operator<<(std::ostream &os, const Diagnostic &diag) {
  const auto maybe_y = diag.span.line_number();
  const auto maybe_x = diag.span.column_number();
  const std::string y =
      maybe_y.has_value() ? std::to_string(maybe_y.value()) : "<y?>";
  const std::string x =
      maybe_x.has_value() ? std::to_string(maybe_x.value()) : "<x?>";

  const auto line = diag.span.source.line(maybe_y.value());

  os << diag.level << " " << diag.issue << "  -> " << diag.span << "\n"
     << "  " << line << "\n"
     << "Help: " << diag.message;

  return os;
}

/* -------------------------------------------------------------------------- */
/* IMPLEMENTATION-PRIVATE HELPERS */
/* -------------------------------------------------------------------------- */

constexpr Diagnostic::Level level_from_issue(const Diagnostic::Issue &kind) {
  using enum Diagnostic::Level;
  using enum Diagnostic::Issue;

#define X(name, repr, severity)                                                \
  case name:                                                                   \
    return severity;

  switch (kind) {
    DIAGNOSTIC_ISSUES
  default:
    return Error;
  }
#undef X
}

/* -------------------------------------------------------------------------- */
/* DIAGNOSTIC IMPLEMENTATION */
/* -------------------------------------------------------------------------- */

Diagnostic::Diagnostic(Issue issue, const Span &span, std::string message)
    : level(level_from_issue(issue)), issue(issue), span(span),
      message(std::move(message)) {}

/* -------------------------------------------------------------------------- */
/* COLLECTION IMPLEMENTATION */
/* -------------------------------------------------------------------------- */

DiagCollect::DiagCollect() : items({}) { items.reserve(16); }

auto DiagCollect::begin() const { return items.begin(); }
auto DiagCollect::end() const { return items.end(); }

void DiagCollect::print_all() const {
  for (const auto &d : *this) {
    std::cout << d << "\n";
  }
  std::cout << std::endl;
}

void DiagCollect::push(const Diagnostic &diag) { items.push_back(diag); }
