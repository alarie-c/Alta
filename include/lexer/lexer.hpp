#ifndef LEXER_H
#define LEXER_H
#include "common/diagnostic.hpp"
#include "common/span.hpp"
#include "lexer/token.hpp"
#include <optional>

/// Used to tokenize a given source file. Takes in some source string and writes
/// the tokens into a `TokenCollect`. Will also emit diagnostics to a
/// `DiagCollect` if any are found. Errors will not abort tokenization.
class Lexer {
  /// The source file to tokenize and the source string to use.
  const Source &source;

  /// The collection of diagnostics to write to in case an error is found.
  DiagCollect &diagnostics;

  /// The collection of tokens to write to during tokenization.
  TokenCollect &tokens;

  /// The current position (in bytes) of the lexer in the file.
  size_t cursor;

private:
  /// Returns whether or not the lexer will be out of bounds at `k` tokens
  /// ahead, which is `0` by default.
  bool is_at_end(unsigned k = 0) const;

  /// Returns the current character pointed to by `cursor`, will return `\0` if
  /// EOF.
  char current() const;

  /// Returns the `k` 'th character from the lexer's cursor.
  char peek(unsigned k = 1) const;

  /// Will move the lexer's cursor ahead `k` times. Will not move past
  /// `source.size`.
  void eat(unsigned k = 1);

  /// Will advance the lexer until the `current()` returns something that isn't
  /// whitespace (newlines are not included in "whitespace").
  void skip_whitespace();

  /// Will tokenize as much of an identifier as it can. Before returning, it
  /// will check to see if that identifier is a keyword.
  Token lex_identifier();

  /// Will tokenize as much of a number as possible, along with distinguishing
  /// between integer and decimal literals.
  Token lex_number();

  /// Will attempt to produce one token. Will emit an diagnostic if it runs into
  /// an error.
  std::optional<Token> lex_once();

public:
  Lexer(const Source &source, TokenCollect &tokens, DiagCollect &diagnostics);

  /// Tokenizes the entire source file and writes all encountered tokens and
  /// diagnostics to their respective collections.
  void lex();
};

#endif