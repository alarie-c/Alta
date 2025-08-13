#include "common/diagnostic.hpp"
#include "common/span.hpp"
#include <iostream>
#include <string>
#include <utility>

namespace diagnostic {

std::ostream &operator<<(std::ostream &os, const Severity &severity) {
  using enum Severity;
  switch (severity) {
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

std::ostream &operator<<(std::ostream &os, const Kind &kind) {
  using enum Kind;
#define X(name, repr, severity)                                                \
  case name:                                                                   \
    return os << repr;
  switch (kind) {
    DIAGNOSTIC_KINDS
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

  os << diag.severity << " " << diag.kind << "  -> " << diag.span << "\n"
     << "  " << line << "\n"
     << "Help: " << diag.message;

  return os;
}
} // namespace diagnostic

using namespace diagnostic;

constexpr Severity get_kind_severity(const Kind &kind) {
  using enum Severity;
  using enum Kind;

#define X(name, repr, severity)                                                \
  case name:                                                                   \
    return severity;

  switch (kind) {
    DIAGNOSTIC_KINDS
  default:
    return Severity::Error;
  }
#undef X
}

Diagnostic::Diagnostic(Kind kind, const Span &span, std::string message)
    : severity(get_kind_severity(kind)), kind(kind), span(span),
      message(std::move(message)) {}

Collection::Collection() : items({}) { items.reserve(16); }

auto Collection::begin() const { return items.begin(); }
auto Collection::end() const { return items.end(); }

void Collection::print_all() const {
  for (const auto &d : *this) {
    std::cout << d << "\n";
  }
  std::cout << std::endl;
}

void Collection::push(const Diagnostic &diag) { items.push_back(diag); }
