#ifndef SPAN_H
#define SPAN_H
#include <iostream>
#include <optional>
#include <string>
#include <string_view>

/// Stores all of the relevant componenets for some compilation unit, including
/// it's string in memory, it's path, and the length of the file. Provides
/// methods for getting the n'th line and related functionality.
struct Source {
  const std::string content;
  const std::string path;
  size_t size;

  Source(std::string content, std::string path);

  /// Creates a new source from just a string. This should be used to create
  /// static compilation units for testing where there is no actual file being
  /// loaded from the OS.
  Source(std::string content);

  /// Returns the ln'th line in the source content. `ln` is 1-based.
  [[nodiscard]] std::string_view line(size_t ln) const;
};

/// Points to some bytes in a compilation unit using a standard span model
/// (`offset` + `length`), all measured in bytes. Provides methods to compute
/// attributes about the span like line number and column number.
struct Span {
  const Source &source;
  const size_t offset;
  const size_t length;

  Span(const Source &source, size_t offset, size_t length);

  /// Fetch the line number, returns `-1` if out of bounds. Line numbers are
  /// 1-based.
  [[nodiscard]] std::optional<size_t> line_number() const;

  /// Fetch the column number, returns `-1` if out of bounds. Column numbers are
  /// 1-based.
  [[nodiscard]] std::optional<size_t> column_number() const;

  /// Returns a string view containing the bytes that this span points to.
  /// Returns an empty `string_view` if out of bounds.
  [[nodiscard]] std::string_view lexeme() const;
};

/* -------------------------------------------------------------------------- */
/* STREAM INSERTION OVERLOADS */
/* -------------------------------------------------------------------------- */

std::ostream &operator<<(std::ostream &os, const Span &span);

#endif