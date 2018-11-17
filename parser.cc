#include "parser.h"
#include <vector>
#include <map>
#include <iostream>

ParserToken::ParserToken() {}

ParserToken::ParserToken(const TokenType &t): Token{t} {}

ParserToken::ParserToken(const TokenType &t, const std::string &le): Token{t, le} {}

ParserToken::~ParserToken() {}

ParsingException::ParsingException() {}

ParsingException::ParsingException(const std::string &msg): msg{msg} {}

std::string ParsingException::getMessage() const { return this->msg; }

ParseTreeNode::ParseTreeNode() {};

ParseTreeNode::ParseTreeNode(const ParserToken &type): type{type} {}

ParseTreeNode::ParseTreeNode(const ParserToken &type, const std::string &lexeme): type{type}, lexeme{lexeme} {}

ParserToken ParseTreeNode::getType() const { return this->type; }

std::string ParseTreeNode::getLexeme() const { return this->lexeme; }

ParseTreeNode ParseTreeNode::getChild(const int index) const { return this->children[index]; }

void ParseTreeNode::addChild(const ParseTreeNode &node) { this->children.emplace_back(node); }

const std::vector<std::vector<TokenType> > productionRules{
  std::vector<TokenType>{ JSON, BOF, OBJECT, END },
  std::vector<TokenType>{ OBJECT, LEFT_BRACE, MEMBERS, RIGHT_BRACE },
  std::vector<TokenType>{ MEMBERS },
  std::vector<TokenType>{ MEMBERS, PAIRS, PAIR },
  std::vector<TokenType>{ PAIRS },
  std::vector<TokenType>{ PAIRS, PAIRS, PAIR, COMMA },
  std::vector<TokenType>{ PAIR, STRING, COLON, VALUE },
  std::vector<TokenType>{ VALUE, STRING },
  std::vector<TokenType>{ VALUE, NUM },
  std::vector<TokenType>{ VALUE, OBJECT },
  std::vector<TokenType>{ VALUE, ARRAY },
  std::vector<TokenType>{ VALUE, BOOLEAN },
  std::vector<TokenType>{ VALUE, NULL_VAL },
  std::vector<TokenType>{ STRING, QUOTE, STRING_EXP, QUOTE },
  std::vector<TokenType>{ ARRAY, LEFT_BRACKET, ELEMENTS, RIGHT_BRACKET },
  std::vector<TokenType>{ ELEMENTS },
  std::vector<TokenType>{ ELEMENTS, VALUES, VALUE },
  std::vector<TokenType>{ VALUES },
  std::vector<TokenType>{ VALUES, VALUES, VALUE, COMMA },
  std::vector<TokenType>{ STRING_EXP },
  std::vector<TokenType>{ STRING_EXP, STRING_EXP, NUM },
  std::vector<TokenType>{ STRING_EXP, STRING_EXP, ID },
};

const std::vector<TokenType> terminals{
  BOF,
  END,
  LEFT_BRACE,
  RIGHT_BRACE,
  LEFT_BRACKET,
  RIGHT_BRACKET,
  ID,
  BOOLEAN,
  NUM,
  NULL_VAL,
  COMMA,
  QUOTE,
  COLON
};

std::map<TokenType, std::map<TokenType, int> > predictionTable;

void registerPrediction(const TokenType &symbol, const TokenType &on, const int ruleIndex) {
  predictionTable[symbol][on] = ruleIndex;
}

void init() {
  registerPrediction(JSON, END, 0);
  registerPrediction(OBJECT, RIGHT_BRACE, 1);
  registerPrediction(MEMBERS, LEFT_BRACE, 2);
  registerPrediction(MEMBERS, RIGHT_BRACE, 3);
  registerPrediction(MEMBERS, RIGHT_BRACKET, 3);
  registerPrediction(MEMBERS, QUOTE, 3);
  registerPrediction(MEMBERS, NUM, 3);
  registerPrediction(MEMBERS, BOOLEAN, 3);
  registerPrediction(MEMBERS, NULL_VAL, 3);
  registerPrediction(PAIR, RIGHT_BRACE, 6);
  registerPrediction(PAIR, RIGHT_BRACKET, 6);
  registerPrediction(PAIR, QUOTE, 6);
  registerPrediction(PAIR, NUM, 6);
  registerPrediction(PAIR, BOOLEAN, 6);
  registerPrediction(PAIR, NULL_VAL, 6);
  registerPrediction(PAIRS, LEFT_BRACE, 4);
  registerPrediction(PAIRS, RIGHT_BRACE, 5);
  registerPrediction(PAIRS, RIGHT_BRACKET, 5);
  registerPrediction(PAIRS, QUOTE, 5);
  registerPrediction(PAIRS, NUM, 5);
  registerPrediction(PAIRS, BOOLEAN, 5);
  registerPrediction(PAIRS, NULL_VAL, 5);
  registerPrediction(PAIRS, COMMA, 5);
  registerPrediction(VALUE, RIGHT_BRACE, 9);
  registerPrediction(VALUE, RIGHT_BRACKET, 10);
  registerPrediction(VALUE, QUOTE, 7);
  registerPrediction(VALUE, NUM, 8);
  registerPrediction(VALUE, BOOLEAN, 11);
  registerPrediction(VALUE, NULL_VAL, 12);
  registerPrediction(VALUES, LEFT_BRACKET, 17);
  registerPrediction(VALUES, RIGHT_BRACE, 18);
  registerPrediction(VALUES, RIGHT_BRACKET, 18);
  registerPrediction(VALUES, QUOTE, 18);
  registerPrediction(VALUES, NUM, 18);
  registerPrediction(VALUES, BOOLEAN, 18);
  registerPrediction(VALUES, NULL_VAL, 18);
  registerPrediction(VALUES, COMMA, 18);
  registerPrediction(ARRAY, RIGHT_BRACKET, 14);
  registerPrediction(ELEMENTS, LEFT_BRACKET, 15);
  registerPrediction(ELEMENTS, RIGHT_BRACE, 16);
  registerPrediction(ELEMENTS, RIGHT_BRACKET, 16);
  registerPrediction(ELEMENTS, QUOTE, 16);
  registerPrediction(ELEMENTS, NUM, 16);
  registerPrediction(ELEMENTS, BOOLEAN, 16);
  registerPrediction(ELEMENTS, NULL_VAL, 16);
  registerPrediction(STRING, QUOTE, 13);
  registerPrediction(STRING_EXP, QUOTE, 19);
  registerPrediction(STRING_EXP, NUM, 20);
  registerPrediction(STRING_EXP, ID, 21);
}

bool isTerminal(const TokenType &sym) {
  return std::find(terminals.begin(), terminals.end(), sym) != terminals.end();
}

template <class T>
bool hasNext(const std::vector<T> &items) {
  return items.begin() != items.end();
}

template <class T>
T next(std::vector<T> &tokens) {
  const T next = tokens.back();
  tokens.pop_back();
  return next;
}

template <class T>
T stackTop(const std::vector<T> &stack) {
  return stack.back();
}

template <class T>
void stackPop(std::vector<T> &stack, int num = 1) {
  while (num > 0) {
    stack.pop_back();
    --num;
  }
}

template <class T>
void stackPush(std::vector<T> &stack, const T &item) {
  stack.emplace_back(item);
}

template <class T>
void stackPush(std::vector<T> &stack, const std::vector<T> &items) {
  for (const auto &item : items) {
    stackPush<T>(stack, item);
  }
}

std::vector<TokenType> getRule(const int index) {
  return productionRules[index];
}

std::vector<TokenType> getProductions(std::vector<TokenType> rule) {
  rule.erase(rule.begin());
  return rule;
}

bool canReduce(const TokenType &curr, const TokenType &on) {
  return predictionTable.find(curr) != predictionTable.end() &&
  predictionTable[curr].find(on) != predictionTable[curr].end();
}

std::vector<ParserToken> ruleToTokens(const std::vector<TokenType> &rule) {
  std::vector<ParserToken> result{};
  for (const auto &type : rule) {
    result.emplace_back(ParserToken{type});
  }
  return result;
}

void reduce(std::vector<TokenType> &stack, const Token &token, std::vector<std::vector<ParserToken> > &result) {
  TokenType curr = stackTop<TokenType>(stack);
  const TokenType on = token.getType();
  while (canReduce(curr, on)) {
    const std::vector<TokenType> rule = getRule(predictionTable[curr][on]);
    result.emplace_back(ruleToTokens(rule));
    const std::vector<TokenType> productions = getProductions(rule);
    stackPop<TokenType>(stack);
    stackPush<TokenType>(stack, productions);
    curr = stackTop<TokenType>(stack);
  }
}

std::vector<std::vector<ParserToken> > parse(std::vector<ScannerToken> tokens) {
  init();
  ParseTreeNode parseTree{};
  std::vector<TokenType> stack{ JSON };
  std::vector<std::vector<ParserToken> > result{};
  while (hasNext<ScannerToken>(tokens)) {
    const ScannerToken token = next<ScannerToken>(tokens);
    const std::string lexeme = token.getLexeme();
    const TokenType type = token.getType();
    if (type == WHITESPACE) continue;
    reduce(stack, token, result);
    const TokenType currState = stackTop(stack);
    if (type == currState) {
      result.emplace_back(std::vector<ParserToken>{
        ParserToken{type},
        ParserToken{TERMINAL, token.getLexeme()}
      });
      stackPop(stack);
    }
    else {
      #ifdef DEBUG
      std::cerr << "Token: " << token << token.getLexeme() << std::endl;
      std::cerr << "Current state: " << currState << std::endl;
      #endif
      throw ParsingException{"Failed to parse token: " + token.getLexeme() + "."};
    }
  }
  return result;
}
