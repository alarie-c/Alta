#ifndef SPAN_H
#define SPAN_H
#include <iostream>
#include <string>
#include <string_view>

struct Source {
  const std::string content;
  const std::string path;
  size_t size;

  Source(std::string content, std::string path);
  Source(std::string content);
};

struct Span {
  const Source &source;
  const size_t offset;
  const size_t length;

  Span(const Source &source, size_t offset, size_t length);

  [[nodiscard]] int line_number() const;
  [[nodiscard]] int column_number() const;
  [[nodiscard]] std::string_view lexeme() const;
};

std::ostream &operator<<(std::ostream &os, const Span &span);

#endif