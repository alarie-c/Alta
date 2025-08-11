#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/span.hpp"
#include "doctest.hpp"
#include <string>

TEST_CASE("Span line numbers, column numbers, and lexemes") {
  std::string raw_text = "Line 1\n" // 6
                         "Line 2\n" // 13
                         "Line 3";  // 19
  const Source src = Source(raw_text);

  {
    const Span span(src, 0, 4);
    CHECK(span.column_number() == 1);
    CHECK(span.line_number() == 1);
    CHECK(span.lexeme() == "Line");
  }
  {
    const Span span(src, 7, 4);
    CHECK(span.column_number() == 1);
    CHECK(span.line_number() == 2);
    CHECK(span.lexeme() == "Line");
  }
  {
    const Span span(src, 19, 1);
    CHECK(span.column_number() == 6);
    CHECK(span.line_number() == 3);
    CHECK(span.lexeme() == "3");
  }
}