#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.hpp"

#include "common/diagnostic.hpp"
#include "common/span.hpp"
#include "lexer/lexer.hpp"
#include "lexer/token.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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
  const Source src(raw_text);

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
  const Source src(raw_text);

  CHECK(src.line(1) == "Line 1\n");
  CHECK(src.line(2) == "Line 2\n");
  CHECK(src.line(3) == "Line 3");
}

TEST_CASE("Span buffer load operator overload") {
  std::string raw_text = "Line 1\n" // 6
                         "Line 2\n" // 13
                         "Line 3";  // 19
  const Source src(raw_text);
  auto ss = sstream_new();

  {
    const Span span(src, 0, 4);
    ss << span;
    const auto str = ss.str();
    std::cerr << str << std::endl;
    CHECK(str.find("<static>:1:1") != std::string::npos);
  }
}

TEST_CASE("Diagnostic creation and printing") {
  std::string raw_text = "Line 1\n" // 6
                         "Line 2\n" // 13
                         "Line 3";  // 19
  const Source src = Source(raw_text);
  // Collection diag_collect;
  auto ss = sstream_new();

  {
    const Span span(src, 0, 4);
    const Diagnostic diag(Diagnostic::Kind::InvalidString, span,
                          "Something about an error.");
    ss << diag;

    const auto str = ss.str();
    std::cerr << str << std::endl;
    CHECK(str.find("<static>:1:1") != std::string::npos);
    CHECK(str.find("invalid string") != std::string::npos);
    CHECK(str.find("Error:") != std::string::npos);
  }
  sstream_clr(ss);
  {
    const Span span(src, 7, 1);
    const Diagnostic diag(Diagnostic::Kind::InvalidCharacter, span,
                          "Something about an error.");
    ss << diag;

    const auto str = ss.str();
    std::cerr << str << std::endl;
    CHECK(str.find("<static>:2:1") != std::string::npos);
    CHECK(str.find("invalid character") != std::string::npos);
    CHECK(str.find("Error:") != std::string::npos);
  }
}

TEST_CASE("Basic lexer tokenization") {
  std::string raw_text = "main := function() 1234";
  const Source src = Source(raw_text);

  DiagCollect diagnostics;
  TokenCollect tokens;
  Lexer lexer(src, tokens, diagnostics);

  lexer.lex();
  const auto resulting_tokens = tokens.data();
  CHECK(resulting_tokens.size() == 8);
  CHECK(resulting_tokens[0].kind == Token::Kind::Identifier);
  CHECK(resulting_tokens[1].kind == Token::Kind::Colon);
  CHECK(resulting_tokens[2].kind == Token::Kind::Equal);
  CHECK(resulting_tokens[3].kind == Token::Kind::Function);
  CHECK(resulting_tokens[4].kind == Token::Kind::LParen);
  CHECK(resulting_tokens[5].kind == Token::Kind::RParen);
  CHECK(resulting_tokens[6].kind == Token::Kind::Integer);
  CHECK(resulting_tokens[7].kind == Token::Kind::Eof);
  CHECK(resulting_tokens[0].span.lexeme() == "main");
  CHECK(resulting_tokens[6].span.lexeme() == "1234");

  for (const auto &t : resulting_tokens) {
    std::cerr << t << "\n";
  }
}