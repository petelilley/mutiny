#include <mutiny/mutiny.hpp>
#include <mutiny/translation_unit/lexer/lexer.hpp>
#include <mutiny/translation_unit/lexer/token.hpp>
#include <mutiny/basic/source_loc.hpp>
#include <mutiny/syntax/keyword.hpp>
#include <mutiny/util/file.hpp>
#include <mutiny/util/logger.hpp>

using namespace mt;

Token Lexer::tokenize_identifier() {
  SourceLoc loc = { src_file.get_line_num(), src_file.get_column_num(), 1 };

  std::string str(1, src_file.current());
  for (c8 c = ++src_file; c && (std::isalpha(c) || c == '_' || std::isdigit(c)); c = ++src_file) {
    str.push_back(c);
  }

  loc.len = str.length();
  
  Keyword kw = KeywordUtil::to_keyword(str);
  if (kw == Keyword::UNKNOWN) {
    return Token(Token::Kind::IDENTIFIER, loc, std::move(str));
  }
  else {
    return Token(Token::Kind::KEYWORD, loc, kw);
  }
}