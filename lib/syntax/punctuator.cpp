#include <mutiny/syntax/punctuator.hpp>

using namespace mt;

Punct PunctUtil::to_punct(std::string_view str) {
  return to_punct(str.data());
}

static constexpr std::array<const char*, 45> punct_strs = {
  "(",
  ")",
  "{",
  "}",
  "[",
  "]",
  ",",
  ";",
  ":",
  ".",
  "+",
  "-",
  "*",
  "/",
  "%",
  "&",
  "|",
  "^",
  "~",
  "<<",
  ">>",
  "=",
  "+=",
  "-=",
  "*=",
  "/=",
  "%=",
  "&=",
  "|=",
  "^=",
  "<<=",
  ">>=",
  "==",
  "!=",
  "<",
  "<=",
  ">",
  ">=",
  "&&",
  "||",
  "!",
  "++",
  "--",
  "->",
  "::",
};

Punct PunctUtil::to_punct(const char* str) {
  for (u16 i = 0; i < punct_strs.size(); i++) {
    if (std::strcmp(str, punct_strs.at(i)) == 0) {
      return static_cast<Punct>(i + 1);
    }
  }
  return Punct::UNKNOWN;
}

const char* PunctUtil::to_string(Punct punct) {
  if (punct == Punct::UNKNOWN) {
    return "UNKNOWN";
  }
  return punct_strs.at(static_cast<u16>(punct) - 1);
}