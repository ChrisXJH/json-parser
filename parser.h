#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <vector>
#include "scanner.h"

enum ParserTokenType {
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
  UNDEFINED
};


class ParserToken {
  ParserTokenType type;
  std::string lexeme;

  public:
  ParserToken();
  ParserToken(const ParserTokenType &t);
  ParserToken(const ParserTokenType &t, const std::string &le);
  ParserTokenType getType() const;
  std::string getLexeme() const;
};

class ParsingException {
  std::string msg;

  public:
  ParsingException();
  ParsingException(const std::string &msg);
  std::string getMessage() const;
};

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

std::vector<std::vector<ParserToken> > parse(std::vector<Token> tokens);
std::ostream &operator<<(std::ostream &out, const ParserToken &sym);

#endif