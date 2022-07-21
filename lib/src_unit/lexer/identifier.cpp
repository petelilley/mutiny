#include <mutiny/mutiny.hpp>
#include <mutiny/src_unit/lexer/lexer.hpp>
#include <mutiny/src_unit/lexer/token.hpp>
#include <mutiny/basic/source_loc.hpp>
#include <mutiny/syntax/keyword.hpp>
#include <mutiny/util/file.hpp>
#include <mutiny/util/logger.hpp>

using namespace mt;

Token Lexer::tokenize_identifier() {
  SourceLoc loc(SourceLoc::at(src_file.get_path(), file_iter.line_num(), file_iter.column_num()));
  u64 len = 1;

  // Count the number of characters before a non-alphanumeric character.
  const c8* first = &*file_iter;
  for (c8 c = *++file_iter; c && (std::isalpha(c) || c == '_' || std::isdigit(c)); c = *++file_iter) {
    len++;
  }
  
  loc.col_f = loc.col_i + len - 1;
  
  // Attempt to match the identifier to a keyword.
  Keyword kw = KeywordUtil::to_keyword(first, len);
  
  if (kw == Keyword::UNKNOWN) {
    return Token(Token::Kind::IDENTIFIER, loc, std::string(first, len));
  }
  else {
    return Token(Token::Kind::KEYWORD, loc, kw);
  }
}
