#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <vector>
#include "scanner.h"


/**
 * ParserToken class
 * extends from Token class
 */
class ParserToken: public Token {

  public:
  ParserToken();
  ParserToken(const TokenType &t);
  ParserToken(const TokenType &t, const std::string &le);
  ~ParserToken() override;
};


/**
 * ParsingException class
 */
class ParsingException {
  std::string msg;

  public:
  ParsingException();
  ParsingException(const std::string &msg);
  std::string getMessage() const;
};


/**
 * ParseTreeNode class
 */ 
class ParseTreeNode {
  ParserToken type;
  std::string lexeme;
  std::vector<ParseTreeNode> children;

  public:
  ParseTreeNode();
  ParseTreeNode(const ParserToken &type);
  ParseTreeNode(const ParserToken &type, const std::string &lexeme);
  ParserToken getType() const;
  std::string getLexeme() const;
  ParseTreeNode getChild(const int index) const;
  void addChild(const ParseTreeNode &node);
};


/**
 * Parses a list of scanned tokens
 */ 
std::vector<std::vector<ParserToken> > parse(std::vector<ScannerToken> tokens);

#endif