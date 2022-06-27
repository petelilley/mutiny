#include <mutiny/mutiny.hpp>
#include <mutiny/translation_unit/lexer/lexer.hpp>
#include <mutiny/translation_unit/lexer/token.hpp>
#include <mutiny/basic/status.hpp>
#include <mutiny/util/file.hpp>
#include <mutiny/util/logger.hpp>

using namespace mt;

Token Lexer::tokenize_char_literal() {
  SourceLoc loc = { src_file.get_line_num(), src_file.get_column_num(), 1 };

  c8 value = *(&src_file.current() + 1);
  
  c8 c;
  for (c = ++src_file; c && (c != '\'') && (c != '\n'); c = ++src_file) {
    loc.len++;
  }

  if (!c || c == '\n') {
    status.report_syntax(Status::ReportContext::ERROR, src_file, loc, "Unterminated character literal");
    has_error = true;
    return Token(Token::Kind::END_OF_FILE, loc);
  }
  
  loc.len++;
  ++src_file;

  if (loc.len > 3) {
    status.report_syntax(Status::ReportContext::ERROR, src_file, loc, "Character literal is too long");
    has_error = true;
    return Token(Token::Kind::CHAR_LITERAL, loc);
  }
  else if (loc.len == 2) {
    status.report_syntax(Status::ReportContext::ERROR, src_file, loc, "Character literal is empty");
    has_error = true;
    return Token(Token::Kind::CHAR_LITERAL, loc);
  }
  
  return Token(Token::Kind::CHAR_LITERAL, loc, value);
}