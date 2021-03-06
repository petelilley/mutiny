#pragma once

#include <mutiny/mutiny.hpp>
#include <mutiny/basic/source_loc.hpp>
#include <mutiny/syntax/keyword.hpp>
#include <mutiny/syntax/punctuator.hpp>

namespace mt {

class Token {
public:
  enum class Kind {
    UNKNOWN,
    IDENTIFIER,
    KEYWORD,
    PUNCTUATOR,
    INT_LITERAL,
    FLOAT_LITERAL,
    STRING_LITERAL,
    CHAR_LITERAL,
    END_OF_FILE,
  };

  // The union of possible token value types.
  using ValueType = std::variant<c8, std::string, Keyword, Punct, u64, f128>;

  inline Token(Kind kind, SourceLoc location, ValueType value = static_cast<c8>(0));
  inline ~Token();

  /**
   * @brief Returns the kind of the token.
   *
   * @return The kind of the token.
   */
  constexpr Kind get_kind() const;

  /**
   * @brief Returns the location of the token in the source file.
   * 
   * @return The location of the token in the source file.
   */
  inline SourceLoc get_location() const;

  /**
   * @brief Returns the value of the token.
   * 
   * @tparam T The type of the value to retrieve.
   * @return The value of the token.
   */
  template<typename T>
  constexpr T get_value() const;

private:
  Kind kind;
  SourceLoc location;
  ValueType value;
};

class TokenUtil {
public:
  static const c8* to_string(Token::Kind kind);
};

} // namespace mt

#include <mutiny/src_unit/lexer/token.ipp>
#include <mutiny/src_unit/lexer/token.tpp>