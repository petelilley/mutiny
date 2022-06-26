#include <mutiny/syntax/keyword.hpp>

using namespace mt;

Keyword KeywordUtil::to_keyword(std::string_view str) {
  return to_keyword(str.data());
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

Keyword KeywordUtil::to_keyword(const char* str) {
  for (u16 i = 0; i < kw_strs.size(); i++) {
    if (std::strcmp(str, kw_strs.at(i)) == 0) {
      return static_cast<Keyword>(i + 1);
    }
  }
  return Keyword::UNKNOWN;
}