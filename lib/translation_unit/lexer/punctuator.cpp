#include <mutiny/mutiny.hpp>
#include <mutiny/translation_unit/lexer/lexer.hpp>
#include <mutiny/translation_unit/lexer/token.hpp>
#include <mutiny/syntax/punctuator.hpp>
#include <mutiny/util/file.hpp>
#include <mutiny/util/logger.hpp>

using namespace mt;

b8 Lexer::is_punctuator() {
  c8 c = src_file.current();
  switch (c) {
    case '(': case ')':
    case '{': case '}':
    case '[': case ']':
    case ',': case ';':
    case ':': case '.':
    case '+': case '-':
    case '*': case '/':
    case '%': case '&':
    case '|': case '^':
    case '~': case '<':
    case '>': case '=':
    case '!':
      return true;
    default:
      return false;
  }
}

Token Lexer::tokenize_punctuator() {
  c8 c = src_file.current();
  c8 c1 = src_file + 1;

  SourceLoc loc = { src_file.get_path(), src_file.get_line_num(), src_file.get_column_num(), 1 };

  switch (c) {
    case ':':
      if (c1 == ':') { // ::
        loc.len = 2;
      }
      break;
    case '+': case '-':
      if (c1 == c || (c == '-' && c1 == '>')) { // ++ or -- or ->
        loc.len = 2;
        break;
      }
      // fall through
    case '*': case '/':
    case '%': case '&':
    case '|': case '^':
      if (c1 == '=') { // += or -= or *= or /= or %= or &= or |= or ^=
        loc.len = 2;
      }
      else if ((c == '&' || c == '|') && c == c1) {
        loc.len = 2;
      }
      break;
    case '<': case '>':
      if (c == c1) { // << or >>
        c8 c2 = src_file + 2;
        loc.len = 2;
        if (c2 == '=') { // <<= or >>=
          loc.len = 3;
        }
      }
      else if (c1 == '=') { // <= or >=
        loc.len = 2;
      }
      break;
    case '=':
    case '!':
      if (c1 == '=') { // == or !=
        loc.len = 2;
      }
      break;
    default:
      break;
  }

  const c8* first = &src_file.current();
  src_file += loc.len;

  return Token(Token::Kind::PUNCTUATOR, loc, PunctUtil::to_punct(first, loc.len));
}