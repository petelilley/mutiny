#include <mutiny/translation_unit/lexer/token.hpp>

using namespace mt;

static constexpr std::array<const c8*, 9> tok_strs = {
  "unknown",
  "identifier",
  "keyword",
  "punctuator",
  "int literal",
  "float literal",
  "string literal",
  "char literal",
  "end of file",
};

const c8* TokenUtil::to_string(Token::Kind kind) {
  return tok_strs.at(static_cast<u16>(kind));
}