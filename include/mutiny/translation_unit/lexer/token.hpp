#pragma once

#include <mutiny/mutiny.hpp>
#include <mutiny/basic/source_loc.hpp>
#include <mutiny/syntax/keyword.hpp>
#include <mutiny/syntax/punctuator.hpp>

namespace mt {

class Token {
public:
  enum class Kind {
    IDENTIFIER,
    KEYWORD,
    PUNCTUATOR,
    NUMERIC_LITERAL,
    STRING_LITERAL,
    CHAR_LITERAL,
    END_OF_FILE,
  };

  // The union of possible token value types.
  using ValueType = std::variant<c8, std::string, Keyword, Punct, u64, f128>;

  inline Token(Kind kind, SourceLoc location, ValueType value = static_cast<c8>(0)) : kind(kind), location(location), value(value) { }
  ~Token() = default;

  /**
   * @brief Returns the kind of the token.
   *
   * @return The kind of the token.
   */
  constexpr Kind get_kind() const { return kind; }

  /**
   * @brief Returns the value of the token.
   * 
   * @tparam T The type of the value to retrieve.
   * @return The value of the token.
   */
  template<typename T>
  constexpr T get_value() const { return std::get<T>(value); }

private:
  Kind kind;
  SourceLoc location;
  ValueType value;
};

} // namespace mt