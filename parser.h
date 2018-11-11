#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <vector>
#include "scanner.h"

enum Symbol {
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

class ParsingException {
  std::string msg;

  public:
  ParsingException();
  ParsingException(const std::string &msg);
  std::string getMessage() const;
};

class ParseTreeNode {
  Symbol type;
  std::string lexeme;
  std::vector<ParseTreeNode> children;

  public:
  ParseTreeNode();
  ParseTreeNode(const Symbol &type);
  ParseTreeNode(const Symbol &type, const std::string &lexeme);
  Symbol getType() const;
  std::string getLexeme() const;
  ParseTreeNode getChild(const int index) const;
  void addChild(const ParseTreeNode &node);
};

std::vector<std::vector<Symbol> > parse(std::vector<Token> tokens);
std::ostream &operator<<(std::ostream &out, const Symbol &sym);

#endif