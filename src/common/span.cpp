#include "common/span.hpp"
#include <iostream>
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

int Span::line_number() const {
  if (offset >= source.size)
    return -1;

  const std::string_view sv(source.content.data(), offset);

  int line = 1;
  for (const auto ch : sv) {
    if (ch == '\n')
      ++line;
  }

  return line;
}

int Span::column_number() const {
  if (offset >= source.size || offset + length > source.size)
    return -1;

  const auto end_of_previous_line = source.content.rfind('\n', offset);

  // Implies that the the start of previous line DNE, therefore this is the
  // first line
  if (end_of_previous_line == std::string::npos)
    return static_cast<int>(offset) + 1;

  // Otherwise, determine how far into the line offset is
  const auto start_of_this_line = end_of_previous_line + 1;
  return static_cast<int>(offset - start_of_this_line + 1);
}

std::string_view Span::lexeme() const {
  if (offset >= source.size || offset + length > source.size)
    return {};

  return std::string_view(source.content.data() + offset, length);
}

std::ostream &operator<<(std::ostream &os, const Span &span) { return os; }
