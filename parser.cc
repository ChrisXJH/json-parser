#include "parser.h"
#include <vector>
#include <map>
#include <iostream>

ParserToken::ParserToken() {}

ParserToken::ParserToken(const ParserTokenType &t): type{t} {}

ParserToken::ParserToken(const ParserTokenType &t, const std::string &le): type{t}, lexeme{le} {}

ParserTokenType ParserToken::getType() const { return this->type; }

std::string ParserToken::getLexeme() const { return this->lexeme; }

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

const std::vector<std::vector<ParserTokenType> > productionRules{
  std::vector<ParserTokenType>{ JSON, BOF, OBJECT, END },
  std::vector<ParserTokenType>{ OBJECT, LEFT_BRACE, MEMBERS, RIGHT_BRACE },
  std::vector<ParserTokenType>{ MEMBERS },
  std::vector<ParserTokenType>{ MEMBERS, PAIRS, PAIR },
  std::vector<ParserTokenType>{ PAIRS },
  std::vector<ParserTokenType>{ PAIRS, PAIRS, PAIR, COMMA },
  std::vector<ParserTokenType>{ PAIR, STRING, COLON, VALUE },
  std::vector<ParserTokenType>{ VALUE, STRING },
  std::vector<ParserTokenType>{ VALUE, NUM },
  std::vector<ParserTokenType>{ VALUE, OBJECT },
  std::vector<ParserTokenType>{ VALUE, ARRAY },
  std::vector<ParserTokenType>{ VALUE, BOOLEAN },
  std::vector<ParserTokenType>{ VALUE, NULL_VAL },
  std::vector<ParserTokenType>{ STRING, QUOTE, STRING_EXP, QUOTE },
  std::vector<ParserTokenType>{ ARRAY, LEFT_BRACKET, ELEMENTS, RIGHT_BRACKET },
  std::vector<ParserTokenType>{ ELEMENTS },
  std::vector<ParserTokenType>{ ELEMENTS, VALUES, VALUE },
  std::vector<ParserTokenType>{ VALUES },
  std::vector<ParserTokenType>{ VALUES, VALUES, VALUE, COMMA },
  std::vector<ParserTokenType>{ STRING_EXP },
  std::vector<ParserTokenType>{ STRING_EXP, STRING_EXP, NUM },
  std::vector<ParserTokenType>{ STRING_EXP, STRING_EXP, ID },
};

const std::vector<ParserTokenType> terminals{
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

std::map<ParserTokenType, std::map<ParserTokenType, int> > predictionTable;

void registerPrediction(const ParserTokenType &symbol, const ParserTokenType &on, const int ruleIndex) {
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

bool isTerminal(const ParserTokenType &sym) {
  return std::find(terminals.begin(), terminals.end(), sym) != terminals.end();
}

ParserTokenType getTokenParserTokenType(const Token &token) {
  switch (token.getType()) {
    case Token::ID:  return ID;
    case Token::NUM: return NUM;
    case Token::COLON:  return COLON;
    case Token::COMMA:  return COMMA;
    case Token::QUOTE:  return QUOTE;
    case Token::LEFT_BRACE: return LEFT_BRACE;
    case Token::RIGHT_BRACE: return RIGHT_BRACE;
    case Token::LEFT_BRACKET: return LEFT_BRACKET;
    case Token::RIGHT_BRACKET: return RIGHT_BRACKET;
    case Token::BOF: return BOF;
    case Token::END: return END;
    default: return UNDEFINED;
  }
  return UNDEFINED;
}

std::ostream &operator<<(std::ostream &out, const ParserToken &token) {
  switch (token.getType()) {
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
    default: out << "Sym not recognized."; break;
  }
  out << "(\'" << token.getLexeme() << "\')";
  return out;
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

std::vector<ParserTokenType> getRule(const int index) {
  return productionRules[index];
}

std::vector<ParserTokenType> getProductions(std::vector<ParserTokenType> rule) {
  rule.erase(rule.begin());
  return rule;
}

bool canReduce(const ParserTokenType &curr, const ParserTokenType &on) {
  return predictionTable.find(curr) != predictionTable.end() &&
  predictionTable[curr].find(on) != predictionTable[curr].end();
}

void print(const std::string &str) {
  std::cout << str;
}

void println(const std::string &str) {
  std::cout << str << std::endl;
}

std::vector<ParserToken> ruleToTokens(const std::vector<ParserTokenType> &rule) {
  std::vector<ParserToken> result{};
  for (const auto &type : rule) {
    result.emplace_back(ParserToken{type});
  }
  return result;
}

void reduce(std::vector<ParserTokenType> &stack, const Token &token, std::vector<std::vector<ParserToken> > &result) {
  ParserTokenType curr = stackTop<ParserTokenType>(stack);
  const ParserTokenType on = getTokenParserTokenType(token);
  while (canReduce(curr, on)) {
    const std::vector<ParserTokenType> rule = getRule(predictionTable[curr][on]);
    result.emplace_back(ruleToTokens(rule));
    const std::vector<ParserTokenType> productions = getProductions(rule);
    stackPop<ParserTokenType>(stack);
    stackPush<ParserTokenType>(stack, productions);
    curr = stackTop<ParserTokenType>(stack);
  }
}

std::vector<std::vector<ParserToken> > parse(std::vector<Token> tokens) {
  init();
  ParseTreeNode parseTree{};
  std::vector<ParserTokenType> stack{ JSON };
  std::vector<std::vector<ParserToken> > productions{};
  while (hasNext<Token>(tokens)) {
    const Token token = next<Token>(tokens);
    const std::string lexeme = token.getLexeme();
    if (token.getType() == Token::WHITESPACE) continue;
    const ParserTokenType tokenSym = getTokenParserTokenType(token);
    reduce(stack, token, productions);
    const ParserTokenType currState = stackTop(stack);
    if (tokenSym == currState) {
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
  return productions;
}
