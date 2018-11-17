#include "token.h"
#include <iostream>

Token::Token() {};

Token::Token(const TokenType &t): type{t} {}

Token::Token(const TokenType &t, const std::string &le): type{t}, lexeme{le} {}

Token::~Token() {}

TokenType Token::getType() const { return this->type; }

std::string Token::getLexeme() const { return this->lexeme; }

std::ostream &operator<<(std::ostream &out, const Token &token) {
  out << token.getType();
  if (token.getLexeme() != "") out << "(\'" << token.getLexeme() << "\')";
  return out;
}

std::ostream &operator<<(std::ostream &out, const TokenType &type) {
  switch (type) {
    case ID:  out << "ID"; break;
    case NUM: out << "NUM"; break;
    case ARRAY: out << "ARRAY"; break;
    case COLON:  out << "COLON"; break;
    case COMMA:  out << "COMMA"; break;
    case QUOTE:  out << "QUOTE"; break;
    case LEFT_BRACE: out << "LEFT_BRACE"; break;
    case RIGHT_BRACE: out << "RIGHT_BRACE"; break;
    case LEFT_BRACKET: out << "LEFT_BRACKET"; break;
    case RIGHT_BRACKET: out << "RIGHT_BRACKET"; break;
    case BOF: out << "BOF"; break;
    case END: out << "END"; break;
    case OBJECT: out << "OBJECT"; break;
    case ELEMENTS: out << "ELEMENTS"; break;
    case STRING: out << "STRING"; break;
    case STRING_EXP: out << "STRING_EXP"; break;
    case VALUE: out << "VALUE"; break;
    case VALUES: out << "VALUES"; break;
    case PAIR: out << "PAIR"; break;
    case PAIRS: out << "PAIRS"; break;
    case MEMBERS: out << "MEMBERS"; break;
    case JSON:  out << "JSON"; break;
    case WHITESPACE:  out << "WHITESPACE"; break;
    case TERMINAL:  out << "TERMINAL"; break;
    default: out << "Type not recognized."; break;
  }
  return out;
}