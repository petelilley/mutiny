#include <mutiny/mutiny.hpp>
#include <mutiny/translation_unit/lexer/lexer.hpp>
#include <mutiny/translation_unit/lexer/token.hpp>
#include <mutiny/syntax/punctuator.hpp>
#include <mutiny/util/file.hpp>
#include <mutiny/util/logger.hpp>

using namespace mt;

b8 Lexer::is_punctuator() {
  c8 c = *file_iter;
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
  c8 c = *file_iter;
  c8 c1 = *(file_iter + 1);

  SourceLoc loc = { src_file.get_path(), file_iter.line_num(), file_iter.column_num(), file_iter.line_num(), file_iter.column_num() };

  u64 len = 1;

  switch (c) {
    case ':':
      if (c1 == ':') { // ::
        len = 2;
      }
      break;
    case '+': case '-':
      if (c1 == c || (c == '-' && c1 == '>')) { // ++ or -- or ->
        len = 2;
        break;
      }
      // fall through
    case '*': case '/':
    case '%': case '&':
    case '|': case '^':
      if (c1 == '=') { // += or -= or *= or /= or %= or &= or |= or ^=
        len = 2;
      }
      else if ((c == '&' || c == '|') && c == c1) {
        len = 2;
      }
      break;
    case '<': case '>':
      if (c == c1) { // << or >>
        c8 c2 = *(file_iter + 2);
        len = 2;
        if (c2 == '=') { // <<= or >>=
          len = 3;
        }
      }
      else if (c1 == '=') { // <= or >=
        len = 2;
      }
      break;
    case '=':
    case '!':
      if (c1 == '=') { // == or !=
        len = 2;
      }
      break;
    default:
      break;
  }

  const c8* first = &*file_iter;
  file_iter += len;
  loc.col_f = loc.col_i + len - 1;
  
  return Token(Token::Kind::PUNCTUATOR, loc, PunctUtil::to_punct(first, len));
}
