#include "token.h"

Token::Token() {};

Token::Token(const TokenType &t): type{t} {}

Token::Token(const TokenType &t, const std::string &le): type{t}, lexeme{le} {}

TokenType Token::getType() const { return this->type; }

std::string Token::getLexeme() const { return this->lexeme; }
