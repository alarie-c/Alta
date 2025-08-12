#include "common/span.hpp"
#include <cassert>
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

std::string_view Source::line(const size_t ln) const {
  assert(ln > 0 && "Line numbers are 1-based");

  size_t line = 1;
  size_t start = 0;
  size_t end = 0;

  // Search for the beginning of the line
  for (size_t i = 0; i < size; ++i) {
    if (line >= ln) {
      start = i;
      break;
    }

    if (content.at(i) == '\n')
      ++line;
  }

  end = content.find('\n', start);

  // Implies the end of the line is EOF
  if (end == std::string::npos) {
    end = size;
  }

  auto line_length = end - start + 1;

  // Remove one if end points to EOF, since I don't want that in the string view
  if (end == size)
    line_length -= 1;

  return std::string_view(content.data() + start, line_length);
}

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

// template <size_t Lines>
//   requires(Lines > 0)
// std::array<std::string_view, Lines> Span::lines(bool search_down) const {
//   if (offset >= source.size || offset + length > source.size)
//     return {};

//   std::array<std::string_view, Lines> result = {};

//   // Start with the current line
//   const auto end_of_previous_line = source.content.rfind('\n', offset);
//   size_t first_left_endpoint;
//   size_t first_right_endpoint;

//   // Implies that the the start of previous line DNE, therefore this is the
//   // first line
//   if (end_of_previous_line == std::string::npos)
//     first_left_endpoint = 0;
//   else
//     first_left_endpoint = end_of_previous_line + 1;

//   // Find the right endpoint
//   first_right_endpoint = source.content.find('\n', first_left_endpoint);

//   // Implies the right endpoint is EOF
//   if (first_right_endpoint == std::string::npos) {
//     first_right_endpoint = source.size;
//   }

//   const auto first_length = first_right_endpoint - first_left_endpoint + 1;
//   const size_t first_index = search_down ? 0 : Lines - 1;

//   // Set the appropriate index in the array to this string view
//   result[first_index] = std::string_view(
//       source.content.data() + first_left_endpoint, first_length);

//   // Early return to prevent over-reading or writing out of bounds
//   if (Lines == 1)
//     return result;

//   // Create a new string_view for each line
//   size_t last_offset = offset;
//   size_t line_number = search_down ? 1 : Lines - 1;
//   while (line_number < Lines) {
//     const auto end_of_this_line = source.content.find('\n', last_offset);
//     size_t left_endpoint;
//     size_t right_endpoint;

//     // Implies that this line goes until EOF
//     if (end_of_this_line == std::string::npos) {

//       for (size_t i = )
//     }

//     if (search_down) {
//       ++line_number;
//       continue;
//     }
//     --line_number;
//   }

//   return result;
// }

std::ostream &operator<<(std::ostream &os, const Span &span) {
  const int y = span.line_number();
  const int x = span.column_number();
  os << span.source.path << ":" << y << ":" << x;
  return os;
}
