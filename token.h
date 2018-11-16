#ifndef JSON_TOKEN_H
#define JSON_TOKEN_H

#include <string>

enum TokenType {
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

class Token {
  TokenType type;
  std::string lexeme;

  public:
  Token();
  Token(const TokenType &t);
  Token(const TokenType &t, const std::string &le);
  TokenType getType() const;
  std::string getLexeme() const;
  virtual ~Token() = 0;
};

#endif