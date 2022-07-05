#include <mutiny/mutiny.hpp>
#include <mutiny/translation_unit/lexer/lexer.hpp>
#include <mutiny/translation_unit/lexer/token.hpp>
#include <mutiny/basic/status.hpp>

using namespace mt;

Lexer::Lexer(InputFile& src_file, Status& status)
: src_file(src_file), file_iter(src_file.cbegin()), status(status) { }

Lexer::~Lexer() = default;

void Lexer::exec() {
  while (true) {
    tokens.push_back(next_token());
    if (tokens.back().get_kind() == Token::Kind::END_OF_FILE || status.get_error_num() > 0) {
      break;
    }
  }
  if (tokens.empty() || tokens.front().get_kind() == Token::Kind::END_OF_FILE) {
    status.report(Status::ReportContext::WARNING, src_file, "Translation unit is empty.");
  }
}

Token Lexer::next_token() {
  c8 c;
  while ((c = *file_iter)) {
    // Skip whitespace.
    if (std::isspace(c)) {
      ++file_iter;
      continue;
    }
    // Skip line comments.
    else if (file_iter.starts_with("//")) {
      skip_line_comment();
      continue;
    }
    // Skip block comments.
    else if (file_iter.starts_with("/*")) {
      file_iter += 2;
      skip_block_comment();
      continue;
    }
    // Read an identifier or keyword.
    else if (std::isalpha(c) || c == '_') {
      return tokenize_identifier();
    }
    // Read a number literal.
    else if (std::isdigit(c) || (c == '.' && std::isdigit(*(file_iter + 1)))) {
      return tokenize_numeric_literal();
    }
    // Read a string literal.
    else if (c == '"') {
      return tokenize_string_literal();
    }
    // Read a character literal.
    else if (c == '\'') {
      return tokenize_char_literal();
    }
    // Read a punctuator.
    else if (is_punctuator()) {
      return tokenize_punctuator();
    }
    // Invalid token.
    else {
      status.report_syntax(Status::ReportContext::ERROR, src_file, {src_file.get_path(), file_iter.line_num(), file_iter.column_num(), file_iter.line_num(), file_iter.column_num()}, fmt::format("Invalid token '{}'", c));
      ++file_iter;
      continue;
    }
  }
  
  return Token(Token::Kind::END_OF_FILE, { src_file.get_path(), file_iter.line_num(), file_iter.column_num(), file_iter.line_num(), file_iter.column_num() });
}
