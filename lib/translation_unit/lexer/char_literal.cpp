#include <mutiny/mutiny.hpp>
#include <mutiny/translation_unit/lexer/lexer.hpp>
#include <mutiny/translation_unit/lexer/token.hpp>
#include <mutiny/syntax/syntax_reporter.hpp>
#include <mutiny/util/file.hpp>
#include <mutiny/util/logger.hpp>

using namespace mt;

Token Lexer::tokenize_char_literal() {
  SourceLoc loc = { src_file.get_line_num(), src_file.get_column_num(), 1 };

  std::string str;
  c8 c;
  for (c = ++src_file; c && (c != '\'') && (c != '\n'); c = ++src_file) {
    str.push_back(c);
  }

  loc.len = str.length() + 1;

  if (!c || c == '\n') {
    SyntaxReporter::report_syntax(SyntaxReporter::Context::ERROR, log_err, src_file, loc, "Unterminated character literal");
    has_error = true;
    return Token(Token::Kind::END_OF_FILE, loc);
  }

  ++src_file;
  loc.len++;

  if (str.length() > 1) {
    SyntaxReporter::report_syntax(SyntaxReporter::Context::ERROR, log_err, src_file, loc, "Character literal too long");
    has_error = true;
    return Token(Token::Kind::CHAR_LITERAL, loc);
  }
  else if (str.empty()) {
    SyntaxReporter::report_syntax(SyntaxReporter::Context::ERROR, log_err, src_file, loc, "Character literal empty");
    has_error = true;
    return Token(Token::Kind::CHAR_LITERAL, loc);
  }
  
  return Token(Token::Kind::CHAR_LITERAL, loc, str.at(0));
}