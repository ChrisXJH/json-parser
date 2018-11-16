#ifndef JSON_SCANNER_H
#define JSON_SCANNER_H

#include <iostream>
#include <string>
#include <vector>
#include "token.h"

/**
 * Token class
 */
class ScannerToken: public Token {};

/**
 * Scanning Exception
 */
class ScanningException {
  std::string msg;

  public:
  ScanningException();
  ScanningException(const std::string &msg);
  std::string getMessage() const;
};


/**
 * Scans code and returns a list of tokens
 */
std::vector<ScannerToken> scan(const std::string &code);

std::ostream &operator<<(std::ostream &out, const Token &token);

#endif