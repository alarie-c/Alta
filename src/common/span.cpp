#include "common/span.hpp"
#include <cassert>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

Source::Source(std::string content, std::string path)
    : content(std::move(content)), path(std::move(path)),
      size(this->content.size()) {}

Source::Source(std::string content)
    : content(std::move(content)), path("<static>"),
      size(this->content.size()) {}

Span::Span(const Source &source, size_t offset, size_t length)
    : source(source), offset(offset), length(length) {}

std::string_view Source::line(const size_t ln) const {
  assert(ln > 0 && "Line numbers are 1-based");

  size_t line = 1;
  size_t start = 0;

  // Find start of requested line
  for (size_t i = 0; i < size; ++i) {
    if (line == ln) {
      start = i;
      break;
    }
    if (content[i] == '\n')
      ++line;
  }

  // If ln is beyond the number of lines, return empty view
  if (line < ln) {
    return {};
  }

  // Find the end of the line (either '\n' or EOF)
  size_t end = content.find('\n', start);

  if (end == std::string::npos) {
    // No newline found: return to EOF (no newline at end)
    return std::string_view(content.data() + start, size - start);
  } else {
    // Found a newline: include it in the view
    return std::string_view(content.data() + start, (end - start) + 1);
  }
}

std::optional<size_t> Span::line_number() const {
  if (offset >= source.size)
    return std::nullopt;

  const std::string_view sv(source.content.data(), offset);

  int line = 1;
  for (const auto ch : sv) {
    if (ch == '\n')
      ++line;
  }

  return line;
}

std::optional<size_t> Span::column_number() const {
  if (offset >= source.size || offset + length > source.size)
    return std::nullopt;

  const auto end_of_previous_line = source.content.rfind('\n', offset);
  if (end_of_previous_line == std::string::npos)
    return offset + 1;
  return offset - end_of_previous_line;
}

std::string_view Span::lexeme() const {
  if (offset >= source.size || offset + length > source.size)
    return {};

  return std::string_view(source.content.data() + offset, length);
}

std::ostream &operator<<(std::ostream &os, const Span &span) {
  const auto maybe_y = span.line_number();
  const auto maybe_x = span.column_number();
  const std::string y =
      maybe_y.has_value() ? std::to_string(maybe_y.value()) : "<y?>";
  const std::string x =
      maybe_x.has_value() ? std::to_string(maybe_x.value()) : "<x?>";

  os << span.source.path << ":" << y << ":" << x;
  return os;
}
