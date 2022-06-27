#pragma once

#include <mutiny/mutiny.hpp>
#include <mutiny/translation_unit/lexer/token.hpp>
#include <mutiny/basic/status.hpp>
#include <mutiny/util/file.hpp>
#include <mutiny/util/logger.hpp>

namespace mt {

class Lexer {
public:
  Lexer(InputFile& src_file, Status& status);
  ~Lexer();

  void exec();

  inline std::vector<Token> get_tokens() const;

private:
  InputFile& src_file;
  std::vector<Token> tokens;
  
  Status& status;

  Token next_token();

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

#include <mutiny/translation_unit/lexer/lexer.ipp>