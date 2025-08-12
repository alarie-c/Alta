#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "common/span.hpp"
#include "doctest.hpp"
#include <iostream>
#include <sstream>
#include <string>

std::stringstream sstream_new() {
  std::stringstream ss;
  ss.str("");
  ss.clear();
  return ss;
}

void sstream_clr(std::stringstream &ss) {
  ss.str("");
  ss.clear();
}

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

TEST_CASE("Source line fetching") {
  std::string raw_text = "Line 1\n" // 6
                         "Line 2\n" // 13
                         "Line 3";  // 19
  const Source src = Source(raw_text);

  CHECK(src.line(1) == "Line 1\n");
  CHECK(src.line(2) == "Line 2\n");
  CHECK(src.line(3) == "Line 3");
}

TEST_CASE("Span buffer load operator overload") {
  std::string raw_text = "Line 1\n" // 6
                         "Line 2\n" // 13
                         "Line 3";  // 19
  const Source src = Source(raw_text);
  auto ss = sstream_new();

  {
    const Span span(src, 0, 4);
    ss << span;
    const auto str = ss.str();
    std::cerr << str << std::endl;
    CHECK(str.find("<static>:1:1") != std::string::npos);
  }
}