#include <mutiny/mutiny.hpp>
#include <mutiny/src_unit/lexer/lexer.hpp>
#include <mutiny/src_unit/lexer/token.hpp>
#include <mutiny/basic/status.hpp>
#include <mutiny/util/file.hpp>
#include <mutiny/util/logger.hpp>

using namespace mt;

Token Lexer::tokenize_char_literal() {
  SourceLoc loc(SourceLoc::at(src_file.get_path(), file_iter.line_num(), file_iter.column_num()));
  u64 len = 1;

  // The character following the '
  c8 value = *(&*file_iter + 1);
  
  // Count the number of characters before the next ', newline, or end of file.
  c8 c;
  for (c = *++file_iter; c && (c != '\'') && (c != '\n'); c = *++file_iter) {
    len++;
  }

  // If the literal ended at a newline or end of file, report an error.
  if (!c || c == '\n') {
    loc.col_f = loc.col_i + len - 1;
    status.report_syntax(Status::ReportContext::ERROR, src_file, loc, "Unterminated character literal");
    return Token(Token::Kind::END_OF_FILE, loc);
  }
  
  len++;
  ++file_iter;
  
  loc.col_f = loc.col_i + len - 1;

  // Check if the character literal is too long.
  if (len > 3) {
    status.report_syntax(Status::ReportContext::ERROR, src_file, loc, "Character literal is too long");
    return Token(Token::Kind::CHAR_LITERAL, loc);
  }
  // Check if the character literal is empty.
  else if (len == 2) {
    status.report_syntax(Status::ReportContext::ERROR, src_file, loc, "Character literal is empty");
    return Token(Token::Kind::CHAR_LITERAL, loc);
  }
  
  return Token(Token::Kind::CHAR_LITERAL, loc, value);
}
