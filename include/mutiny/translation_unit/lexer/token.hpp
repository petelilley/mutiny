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

  using ValueType = std::variant<std::string, Keyword, Punct, u64, f128, c8>;

  inline Token(Kind kind, SourceLoc location, ValueType value = static_cast<c8>(0)) : kind(kind), location(location), value(value) { }
  ~Token() = default;

  constexpr Kind get_kind() const { return kind; }

  template<typename T>
  constexpr T get_value() const { return std::get<T>(value); }

  // inline std::optional<Token> get_next() const { return next ? std::optional<Token>(*next) : std::nullopt; }
  // inline void set_next(Token* next) { this->next = next; }

private:
  Kind kind;
  SourceLoc location;
  ValueType value;

  // Token* next;
};

/*


struct Tok {
  enum class Kind {
    IDENTIFIER,
    KEYWORD,
    PUNCTUATOR,
    NUMERIC_LITERAL,
    STRING_LITERAL,
    CHAR_LITERAL,
    END_OF_FILE,
  };

  using ValueType = std::variant<std::string, Keyword, Punct, u64, f128, c8>;
  
  Kind kind;
  ValueType value;
  SourceLoc location;
  Tok* next;
};
*/

} // namespace mt

/*

class IdentifierToken : public Token {
public:
  IdentifierToken(std::string value);
  ~IdentifierToken();
  
  constexpr Kind get_kind() const override { return Kind::IDENTIFIER; }
  
  inline std::string_view get_value() const { return value; }

private:
  std::string value; // TODO: Hash string?
};

class KeywordToken : public Token {
public:

  KeywordToken(Keyword keyword);
  ~KeywordToken();
  
  constexpr Kind get_kind() const override { return Kind::KEYWORD; }
  Keyword get_value() const;

private:
  Keyword keyword;
};

class PunctuatorToken : public Token {
public:
  PunctuatorToken(Punct punct);
  ~PunctuatorToken();

  constexpr Kind get_kind() const override { return Kind::PUNCTUATOR; }
  inline Punct get_value() const { return value; }

private:
  Punct value;
};

class NumberToken : public Token {
public:
  enum class Type {
    INTEGER,
    FLOAT,
  };

  template<typename T, typename std::enable_if<std::is_same<u64, T>::value || std::is_same<f128, T>::value>::type* = nullptr>
  NumberToken(T value) {
    if (std::is_same<u64, T>::value) {
      type = Type::INTEGER;
      int_val = value;
    }
    else {
      type = Type::FLOAT;
      float_val = value;
    }
  }

  ~NumberToken() = default;

  constexpr Kind get_kind() const override { return Kind::NUMERIC_LITERAL; }
  
  template<typename T, typename std::enable_if<std::is_same<u64, T>::value || std::is_same<f128, T>::value>::type* = nullptr>
  inline T get_int_value() const {
    static_assert((std::is_same<u64, T>::value && type == Type::Integer) || (std::is_same<f128, T>::value && Type::FLOAT)), "Number type mismatch");
    
    if (type == Type::INTEGER) return int_val;
    else return float_val;
  }

  inline Type get_type() const { return type; }

private:
  Type type;

  union {
    u64 int_val;
    f128 float_val;
  };
};

class StringToken : public Token {
public:
  StringToken(std::string value);
  ~StringToken();

  constexpr Kind get_kind() const override { return Kind::STRING_LITERAL; }
  inline std::string_view get_value() const { return value; }

private:
  std::string value;
};

class CharToken : public Token {
public:
  CharToken(char value);
  ~CharToken();

  constexpr Kind get_kind() const override { return Kind::CHAR_LITERAL; }
  inline char get_value() const { return value; }

private:
  char value;
};

class EOFToken : public Token {
public:
  EOFToken();
  ~EOFToken();

  constexpr Kind get_kind() const override { return Kind::END_OF_FILE; }
};

} // namespace mt
*/