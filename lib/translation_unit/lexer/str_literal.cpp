#include <mutiny/mutiny.hpp>
#include <mutiny/translation_unit/lexer/lexer.hpp>
#include <mutiny/translation_unit/lexer/token.hpp>
#include <mutiny/util/file.hpp>
#include <mutiny/util/logger.hpp>

using namespace mt;

Token Lexer::tokenize_string_literal() {
  SourceLoc loc = { src_file.get_path(), file_iter.line_num(), file_iter.column_num(), file_iter.line_num(), file_iter.column_num() };

  u64 len = 1;

  // The beginning of the string literal.
  const c8* first = &*file_iter;

  // Count the number of characters before the next ", newline, or end of file.
  c8 c;
  for (c = *++file_iter; c && (c != '"') && (c != '\n'); c = *++file_iter) {
    len++;
  }

  loc.col_f = loc.col_i + len;

  // If the literal ended at a newline or end of file, report an error.
  if (!c || c == '\n') {
    status.report_syntax(Status::ReportContext::ERROR, src_file, loc, "Unterminated string literal");
    return Token(Token::Kind::END_OF_FILE, loc);
  }
  
  ++file_iter;
  return Token(Token::Kind::STRING_LITERAL, loc, std::string(first + 1, len - 1));
}
