#pragma once

#include <mutiny/mutiny.hpp>

namespace mt {

enum class Keyword : u16 {
  UNKNOWN = 0,
  FUNC, VAR,
  IF, ELSE, WHILE, RETURN, GOTO,
};

class KeywordUtil {
public:
  static Keyword to_keyword(std::string_view str);
  static Keyword to_keyword(const char* str);
};

} // namespace mt