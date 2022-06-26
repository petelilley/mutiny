#include <mutiny/mutiny.hpp>
#include <mutiny/translation_unit/lexer/lexer.hpp>
#include <mutiny/translation_unit/lexer/token.hpp>
#include <mutiny/syntax/syntax_reporter.hpp>

using namespace mt;

Lexer::Lexer(InputFile& src_file, Logger& log_out, Logger& log_err, Logger& log_warn)
: src_file(src_file), log_out(log_out), log_err(log_err), log_warn(log_warn) { }

Lexer::~Lexer() = default;

b8 Lexer::exec() {
  std::optional<Token> t;

  while (t = next_token()) {
    tokens.push_back(std::move(t.value()));
    if (tokens.back().get_kind() == Token::Kind::END_OF_FILE || has_error) {
      break;
    }
  }
  if (tokens.empty() || tokens.front().get_kind() != Token::Kind::END_OF_FILE) {
    SyntaxReporter::report_syntax(SyntaxReporter::Context::WARNING, log_err, src_file, {1, 1, 0}, "Translation unit is empty.");
  }

  return has_error;
}

std::optional<Token> Lexer::next_token() {
  c8 c;
  while ((c = src_file.current())) {
    if (std::isspace(c)) {
      src_file++;
      continue;
    }
    else if (src_file.starts_with("//")) {
      skip_line_comment();
      break;
    }
    else if (src_file.starts_with("/*")) {
      src_file += 2;
      skip_block_comment();
      break;
    }
    else if (std::isalpha(c) || c == '_') {
      return tokenize_identifier();
    }
    else if (std::isdigit(c) || (c == '.' && std::isdigit(src_file.peek_next()))) {
      return tokenize_numeric_literal();
    }
    else if (c == '"') {
      return tokenize_string_literal();
    }
    else if (c == '\'') {
      return tokenize_char_literal();
    }
    else if (is_punctuator()) {
      return tokenize_punctuator();
    }
    else if (!c) {
      return Token(Token::Kind::END_OF_FILE, { src_file.get_line_num(), src_file.get_column_num(), 0 });
    }
    else {
      SyntaxReporter::report_syntax(SyntaxReporter::Context::ERROR, log_err, src_file, {src_file.get_line_num(), src_file.get_column_num(), 1}, fmt::format("Invalid token '{}'", c));
      has_error = true;
      src_file++;
      continue;
    }
  }
  return std::nullopt;
}