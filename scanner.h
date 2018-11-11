#ifndef JSON_SCANNER_H
#define JSON_SCANNER_H

#include <iostream>
#include <string>
#include <vector>

/**
 * Token class
 */
class Token {
  public:
  enum Type {
    ID,
    QUOTE,
    COLON,
    NUM,
    BOOLEAN,
    NULL_VAL,
    WHITESPACE,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    LEFT_BRACE,
    RIGHT_BRACE,
    UNDEFINED,
    COMMA,
    BOF,
    END
};

 private:
 Type type;
 std::string lexeme;

 public:
 Token();
 Token(const Type &t, const std::string &le);
 Type getType() const;
 std::string getLexeme() const;
};


/**
 * Scanning Exception
 */
class ScanningException {
  std::string msg;

  public:
  ScanningException();
  ScanningException(const std::string &msg);
  std::string getMessage() const;
};


/**
 * Scans code and returns a list of tokens
 */
std::vector<Token> scan(const std::string &code);

std::ostream &operator<<(std::ostream &out, const Token &token);

#endif