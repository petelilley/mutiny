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
    tokens.push_back(std::move(next_token()));
    if (tokens.back().get_kind() == Token::Kind::END_OF_FILE || status.get_error_num() > 0) {
      break;
    }
  }
  if (tokens.empty() || tokens.front().get_kind() == Token::Kind::END_OF_FILE) {
    status.report(Status::ReportContext::WARNING, src_file, "Translation unit is empty.");
  }

#if 1
  for (const Token& tok : tokens) {
    switch (tok.get_kind()) {
      case Token::Kind::UNKNOWN:
        std::cout << "unknown\n";
        break;
      case Token::Kind::IDENTIFIER:
        std::cout << "Identifier: " << tok.get_value<std::string>() << std::endl;
        break;
      case Token::Kind::KEYWORD:
        std::cout << "Keyword: " << KeywordUtil::to_string(tok.get_value<Keyword>()) << std::endl;
        break;
      case Token::Kind::PUNCTUATOR:
        std::cout << "Punctuator: " << PunctUtil::to_string(tok.get_value<Punct>()) << std::endl;
        break;
      case Token::Kind::INT_LITERAL:
        std::cout << "Integer literal: " << tok.get_value<u64>() << std::endl;
        break;
      case Token::Kind::FLOAT_LITERAL:
        std::cout << "Float literal: " << tok.get_value<f128>() << std::endl;
        break;
      case Token::Kind::STRING_LITERAL:
        std::cout << "String literal: " << tok.get_value<std::string>() << std::endl;
        break;
      case Token::Kind::CHAR_LITERAL:
        std::cout << "Character literal: " << tok.get_value<c8>() << std::endl;
        break;
      case Token::Kind::END_OF_FILE:
        std::cout << "EOF" << std::endl;
        break;
    }
  }
#endif
}

Token Lexer::next_token() {
  c8 c;
  while ((c = *file_iter)) {
    if (std::isspace(c)) {
      ++file_iter;
      continue;
    }
    else if (file_iter.starts_with("//")) {
      skip_line_comment();
      continue;
    }
    else if (file_iter.starts_with("/*")) {
      file_iter += 2;
      skip_block_comment();
      continue;
    }
    else if (std::isalpha(c) || c == '_') {
      return tokenize_identifier();
    }
    else if (std::isdigit(c) || (c == '.' && std::isdigit(*(file_iter + 1)))) {
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
    else {
      status.report_syntax(Status::ReportContext::ERROR, src_file, {src_file.get_path(), file_iter.line_num(), file_iter.column_num(), 1}, fmt::format("Invalid token '{}'", c));
      ++file_iter;
      continue;
    }
  }
  
  return Token(Token::Kind::END_OF_FILE, { src_file.get_path(), file_iter.line_num(), file_iter.column_num(), 0 });
}