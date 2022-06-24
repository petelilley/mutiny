#pragma once

#include <mutiny/mutiny.hpp>
#include <mutiny/translation_unit/lexer/token.hpp>
#include <mutiny/util/file.hpp>
#include <mutiny/util/logger.hpp>

namespace mt {

class Lexer {
public:
  Lexer(InputFile& src_file, Logger& log_out, Logger& log_err, Logger& log_warn);
  ~Lexer();

  void exec() noexcept;

  Token& get_tokens() noexcept { return tokens; }

private:
  InputFile& src_file;
  Token tokens;
  
  Logger& log_out;
  Logger& log_err;
  Logger& log_warn;
};

} // namespace mt