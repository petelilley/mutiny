#include <mutiny/mutiny.hpp>
#include <mutiny/translation_unit/lexer/lexer.hpp>
#include <mutiny/translation_unit/lexer/token.hpp>
#include <mutiny/util/file.hpp>
#include <mutiny/util/logger.hpp>

using namespace mt;

void Lexer::skip_line_comment() {
  c8 c;
  while ((c = ++src_file) && (c != '\n'));
}

void Lexer::skip_block_comment() {
  c8 c = src_file.current();

  while (true) {
    if (!c) {
      status.report_syntax(Status::ReportContext::ERROR, src_file, { src_file.get_line_num(), src_file.get_column_num(), 1 }, "Unterminated /* comment");
      break;
    }
    if (c == '/' && (src_file + 1) == '*') {
      src_file += 2;
      skip_block_comment();
      if (!src_file.current()) break;
    }
    if (c == '*' && (src_file + 1) == '/') {
      src_file += 2;
      break;
    }
    
    c = ++src_file;
  }
}