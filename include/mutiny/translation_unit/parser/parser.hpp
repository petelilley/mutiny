#pragma once

#include <mutiny/mutiny.hpp>
#include <mutiny/util/file.hpp>
#include <mutiny/util/logger.hpp>

namespace mt {

class Token;
class ASTNode;

class Parser {
public:
  Parser(Token& token, Logger& log_out, Logger& log_err, Logger& log_warn);
  ~Parser();

  void exec() noexcept;

private:
  Token& tokens;
  
  Logger& log_out;
  Logger& log_err;
  Logger& log_warn;
};

} // namespace mt