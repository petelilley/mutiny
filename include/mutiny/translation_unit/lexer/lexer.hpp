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

  b8 exec();

  inline std::vector<Token> get_tokens() const { return tokens; }

private:
  InputFile& src_file;
  std::vector<Token> tokens;
  
  Logger& log_out;
  Logger& log_err;
  Logger& log_warn;

  b8 has_error = false;

  std::optional<Token> next_token();

  Token tokenize_identifier();
  b8 is_punctuator();
  Token tokenize_punctuator();
  Token tokenize_char_literal();
  Token tokenize_string_literal();
  Token tokenize_numeric_literal();

  void skip_line_comment();
  void skip_block_comment();
};

} // namespace mt