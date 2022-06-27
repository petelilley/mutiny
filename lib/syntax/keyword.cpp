#include <mutiny/syntax/keyword.hpp>

using namespace mt;

Keyword KeywordUtil::to_keyword(std::string_view str) {
  return to_keyword(str.data(), str.length());
}

static constexpr std::array<const char*, 7> kw_strs = {
  "func",
  "var",
  "if",
  "else",
  "while",
  "return",
  "goto",
};

Keyword KeywordUtil::to_keyword(const char* str, u32 len) {
  for (u16 i = 0; i < kw_strs.size(); i++) {
    if (std::strncmp(str, kw_strs.at(i), len) == 0) {
      return static_cast<Keyword>(i + 1);
    }
  }
  return Keyword::UNKNOWN;
}

const char* KeywordUtil::to_string(Keyword kw) {
  if (kw == Keyword::UNKNOWN) {
    return "UNKNOWN";
  }
  return kw_strs.at(static_cast<u16>(kw) - 1);
}