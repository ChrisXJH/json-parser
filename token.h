#ifndef JSON_TOKEN_H
#define JSON_TOKEN_H

#include <string>


/**
 * TokenType enum
 * Valid token types
 */ 
enum TokenType {
  JSON,
  BOF,
  END,
  OBJECT,
  LEFT_BRACE,
  RIGHT_BRACE,
  LEFT_BRACKET,
  RIGHT_BRACKET,
  VALUE,
  VALUES,
  ELEMENT,
  ELEMENTS,
  MEMBERS,
  PAIRS,
  PAIR,
  ARRAY,
  ID,
  BOOLEAN,
  NUM,
  NULL_VAL,
  COMMA,
  QUOTE,
  COLON,
  STRING,
  STRING_EXP,
  WHITESPACE,
  TERMINAL,
  UNDEFINED
};


/**
 * Token abstract class
 */ 
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


/**
 * Overriden output stream operator for TokenType
 */ 
std::ostream &operator<<(std::ostream &out, const TokenType &type);


/**
 * Overriden output stream operator for Token
 */ 
std::ostream &operator<<(std::ostream &out, const Token &sym);

#endif