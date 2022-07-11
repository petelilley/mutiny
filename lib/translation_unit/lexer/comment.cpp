#include <mutiny/mutiny.hpp>
#include <mutiny/translation_unit/lexer/lexer.hpp>
#include <mutiny/translation_unit/lexer/token.hpp>
#include <mutiny/util/file.hpp>
#include <mutiny/util/logger.hpp>

using namespace mt;

void Lexer::skip_line_comment() {
  // Skip over every character until a newline or the end of file is reached.
  c8 c;
  while ((c = *++file_iter) && (c != '\n'));
}

void Lexer::skip_block_comment() {
  c8 c = *file_iter;

  while (true) {
    // End of the file, so report an error.
    if (!c) {
      status.report_syntax(Status::ReportContext::ERROR, src_file, SourceLoc::at(src_file.get_path(), file_iter.line_num(), file_iter.column_num()), "Unterminated /* comment");
      break;
    }
    // Begin a nested block comment.
    if (c == '/' && *(file_iter + 1) == '*') {
      file_iter += 2;
      skip_block_comment();
      if (!*file_iter) break;
      
      c = *file_iter;
      continue;
    }
    // End of the comment.
    if (c == '*' && *(file_iter + 1) == '/') {
      file_iter += 2;
      break;
    }
    
    c = *++file_iter;
  }
}
