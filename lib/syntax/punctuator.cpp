#include <mutiny/syntax/punctuator.hpp>

using namespace mt;

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

Punct PunctUtil::to_punct(const char* str, u32 len) {
  for (u16 i = 0; i < punct_strs.size(); i++) {
    if (std::strncmp(str, punct_strs.at(i), len) == 0) {
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