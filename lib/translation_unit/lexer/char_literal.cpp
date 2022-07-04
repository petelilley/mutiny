#include <mutiny/mutiny.hpp>
#include <mutiny/translation_unit/lexer/lexer.hpp>
#include <mutiny/translation_unit/lexer/token.hpp>
#include <mutiny/basic/status.hpp>
#include <mutiny/util/file.hpp>
#include <mutiny/util/logger.hpp>

using namespace mt;

Token Lexer::tokenize_char_literal() {
  SourceLoc loc = { src_file.get_path(), file_iter.line_num(), file_iter.column_num(), file_iter.line_num(), file_iter.column_num() };

  u64 len = 1;

  c8 value = *(&*file_iter + 1);
  
  c8 c;
  for (c = *++file_iter; c && (c != '\'') && (c != '\n'); c = *++file_iter) {
    len++;
  }

  if (!c || c == '\n') {
    loc.col_f = loc.col_i + len - 1;
    status.report_syntax(Status::ReportContext::ERROR, src_file, loc, "Unterminated character literal");
    return Token(Token::Kind::END_OF_FILE, loc);
  }
  
  len++;
  ++file_iter;
  
  loc.col_f = loc.col_i + len - 1;

  if (len > 3) {
    status.report_syntax(Status::ReportContext::ERROR, src_file, loc, "Character literal is too long");
    return Token(Token::Kind::CHAR_LITERAL, loc);
  }
  else if (len == 2) {
    status.report_syntax(Status::ReportContext::ERROR, src_file, loc, "Character literal is empty");
    return Token(Token::Kind::CHAR_LITERAL, loc);
  }
  
  return Token(Token::Kind::CHAR_LITERAL, loc, value);
}