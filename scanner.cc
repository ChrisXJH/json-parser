#include "scanner.h"
#include <cctype>
#include <map>

Token::Token() {}

Token::Token(const Type &t, const std::string &le): type{t}, lexeme{le} {}

Token::Type Token::getType() const { return this->type; }

std::string Token::getLexeme() const { return this->lexeme; }

ScanningException::ScanningException() {}

ScanningException::ScanningException(const std::string &msg): msg{msg} {}

std::string ScanningException::getMessage() const { return this->msg; }

int anychar(const int chr) { return 1; }

int isidchar(const int chr) { return (int) chr != '"'; }

class DFA {
 public:
  enum State {
    START,
    ID,
    QUOTE,
    COLON,
    NUM,
    WHITESPACE,
    ID_SPECIAL_CHAR,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    ERROR
  };

  static std::map<State, std::map<char, State> > transitions;
  static std::vector<State> acceptingStates;

  static void init() {
    acceptingStates = {
      ID,
      QUOTE,
      COLON,
      NUM,
      WHITESPACE,
      LEFT_BRACKET,
      RIGHT_BRACKET,
      LEFT_BRACE,
      RIGHT_BRACE,
      COMMA
    };
    
    registerTransition(START, isidchar, ID);
    registerTransition(ID, isidchar, ID);
    registerTransition(ID, '\\', ID_SPECIAL_CHAR);
    registerTransition(ID_SPECIAL_CHAR, anychar, ID);
    registerTransition(START, isdigit, NUM);
    registerTransition(NUM, isdigit, NUM);
    registerTransition(START, '"', QUOTE);
    registerTransition(START, ':', COLON);
    registerTransition(START, '[', LEFT_BRACKET);
    registerTransition(START, ']', RIGHT_BRACKET);
    registerTransition(START, '{', LEFT_BRACE);
    registerTransition(START, '}', RIGHT_BRACE);
    registerTransition(START, ' ', WHITESPACE);
    registerTransition(START, ',', COMMA);
  }

  void static registerTransition(const State &origin, const char on, const State &result) {
    DFA::transitions[origin][on] = result;
  }

  void static registerTransition(const State &origin, int (*test)(int), const State &result) {
    for (int c = 0; c < 128; ++c) {
      if (test(c)) {
        DFA::transitions[origin][c] = result;
      }
    }
  }

  State static startState() {
    return START;
  }

  bool static isError(const State &state) {
    return state == ERROR;
  }

  State static nextState(const State &origin, const char on) {
    if (DFA::transitions.find(origin) != DFA::transitions.end() &&
    DFA::transitions[origin].find(on) != DFA::transitions[origin].end()) {
      return DFA::transitions[origin][on];
    }
    return ERROR;
  }

  bool static isAccepting(const State &state) {
    return std::find(DFA::acceptingStates.begin(), DFA::acceptingStates.end(), state) != DFA::acceptingStates.end();
  }
};

std::map<DFA::State, std::map<char, DFA::State> > DFA::transitions;
std::vector<DFA::State> DFA::acceptingStates;

Token::Type stateToTokenType(const DFA::State &state) {
  Token::Type result;
  switch (state) {
    case DFA::ID: result = Token::ID; break;
    case DFA::QUOTE: result = Token::QUOTE; break;
    case DFA::COLON: result = Token::COLON; break;
    case DFA::NUM: result = Token::NUM; break;
    case DFA::WHITESPACE: result = Token::WHITESPACE; break;
    case DFA::LEFT_BRACKET: result = Token::LEFT_BRACKET; break;
    case DFA::RIGHT_BRACKET: result = Token::RIGHT_BRACKET; break;
    case DFA::LEFT_BRACE: result = Token::LEFT_BRACE; break;
    case DFA::RIGHT_BRACE: result = Token::RIGHT_BRACE; break;
    case DFA::COMMA: result = Token::COMMA; break;
    default: result = Token::UNDEFINED; break;
  }
  return result;
}

std::vector<Token> scan(const std::string &code) {
  DFA::init();
  DFA::State currState = DFA::startState();
  DFA::State prevState = currState;
  std::string lexeme;
  std::vector<Token> result;
  std::string::const_iterator it = code.begin();
  while (true) {
    if (it == code.end()) {
      if (DFA::isAccepting(currState)) result.emplace_back(Token{stateToTokenType(currState), lexeme});
      break;
    }

    currState = DFA::nextState(currState, *it);
    #ifdef DEBUG
    std::cout << "input: " << *it << " curr: " << currState << " prev: " << prevState << std::endl;
    #endif

    if (DFA::isError(currState)) {
      if (DFA::isAccepting(prevState)) {
        result.emplace_back(Token{stateToTokenType(prevState), lexeme});
        currState = DFA::startState();
        prevState = currState;
        lexeme = "";
      }
      else {
        lexeme += *it;
        throw ScanningException{"ERROR: Unkown token \"" + lexeme + "\""};
      }
    }
    else {
      if (currState != DFA::ID_SPECIAL_CHAR) lexeme += *it;
      prevState = currState;
      ++it;
    }
  }
  return result;
}

std::ostream &operator<<(std::ostream &out, const Token &token) {
  std::string type;
  switch (token.getType()) {
    case Token::ID: type = "ID"; break;
    case Token::QUOTE: type = "QUOTE"; break;
    case Token::COLON: type = "COLON"; break;
    case Token::NUM: type = "NUM"; break;
    case Token::LEFT_BRACKET: type = "LEFT_BRACKET"; break;
    case Token::RIGHT_BRACKET: type = "RIGHT_BRACKET"; break;
    case Token::LEFT_BRACE: type = "LEFT_BRACE"; break;
    case Token::RIGHT_BRACE: type = "RIGHT_BRACE"; break;
    case Token::COMMA: type = "COMMA"; break;
    case Token::BOF: type = "BOF"; break;
    case Token::END: type = "END"; break;
    case Token::WHITESPACE: type = "WHITESPACE"; break;
    case Token::UNDEFINED: type = "UNDEFINED"; break;
    default: type = ""; break;
  }
  out << type << "(\'" << token.getLexeme() << "\') ";
  return out;
}
