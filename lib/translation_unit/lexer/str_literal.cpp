#include <mutiny/mutiny.hpp>
#include <mutiny/translation_unit/lexer/lexer.hpp>
#include <mutiny/translation_unit/lexer/token.hpp>
#include <mutiny/util/file.hpp>
#include <mutiny/util/logger.hpp>

using namespace mt;

Token Lexer::tokenize_string_literal() {
  SourceLoc loc = { src_file.get_line_num(), src_file.get_column_num(), 1 };

  std::string str;
  c8 c;
  for (c = ++src_file; c && (c != '"') && (c != '\n'); c = ++src_file) {
    str.push_back(c);
  }

  loc.len = str.length() + 1;

  if (!c || c == '\n') {
    status.report_syntax(Status::ReportContext::ERROR, src_file, loc, "Unterminated string literal");
    has_error = true;
    return Token(Token::Kind::END_OF_FILE, loc);
  }

  loc.len++;
  
  ++src_file;
  return Token(Token::Kind::STRING_LITERAL, loc, std::move(str));
}