#include <mutiny/mutiny.hpp>
#include <mutiny/translation_unit/lexer/lexer.hpp>
#include <mutiny/translation_unit/lexer/token.hpp>
#include <mutiny/basic/status.hpp>

using namespace mt;

Lexer::Lexer(InputFile& src_file, Status& status)
: src_file(src_file), status(status) { }

Lexer::~Lexer() = default;

b8 Lexer::exec() {
  std::optional<Token> t;

  while (t = next_token()) {
    tokens.push_back(std::move(t.value()));
    if (tokens.back().get_kind() == Token::Kind::END_OF_FILE || has_error) {
      break;
    }

    if (t.value().get_kind() == Token::Kind::STRING_LITERAL) {
      std::cout << "str " << t.value().get_value<std::string>() << '\n';
    }
    if (t.value().get_kind() == Token::Kind::CHAR_LITERAL) {
      std::cout << "char " << t.value().get_value<char>() << '\n';
    }
    if (t.value().get_kind() == Token::Kind::PUNCTUATOR) {
      std::cout << "punc " << PunctUtil::to_string(t.value().get_value<Punct>()) << '\n';
    }
  }
  if (tokens.empty() || tokens.front().get_kind() != Token::Kind::END_OF_FILE) {
    status.report(Status::ReportContext::WARNING, src_file, "Translation unit is empty.");
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
      status.report_syntax(Status::ReportContext::ERROR, src_file, {src_file.get_line_num(), src_file.get_column_num(), 1}, fmt::format("Invalid token '{}'", c));
      has_error = true;
      src_file++;
      continue;
    }
  }
  return std::nullopt;
}