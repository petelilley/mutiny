#pragma once

#include <mutiny/mutiny.hpp>

namespace mt {

enum class Punct : u16 {
  UNKNOWN = 0,
  LPAREN,       // (
  RPAREN,       // )
  LBRACE,       // {
  RBRACE,       // }
  LBRACKET,     // [
  RBRACKET,     // ]
  COMMA,        // ,
  SEMICOLON,    // ;
  COLON,        // :
  DOT,          // .
  ADD,          // +
  SUB,          // -
  MUL,          // *
  DIV,          // /
  MOD,          // %
  BIT_AND,      // &
  BIT_OR,       // |
  BIT_XOR,      // ^
  BIT_NOT,      // ~
  BIT_SHL,      // <<
  BIT_SHR,      // >>
  ASGN,         // =
  ASGN_ADD,     // +=
  ASGN_SUB,     // -=
  ASGN_MUL,     // *=
  ASGN_DIV,     // /=
  ASGN_MOD,     // %=
  ASGN_BIT_AND, // &=
  ASGN_BIT_OR,  // |=
  ASGN_BIT_XOR, // ^=
  ASGN_BIT_SHL, // <<=
  ASGN_BIT_SHR, // >>=
  CMP_EQ,       // ==
  CMP_NE,       // !=
  CMP_LT,       // <
  CMP_LE,       // <=
  CMP_GT,       // >
  CMP_GE,       // >=
  LOG_AND,      // &&
  LOG_OR,       // ||
  LOG_NOT,      // !
  INC,          // ++
  DEC,          // --
  ARROW,        // ->
  COLON_COLON,  // ::
};

class PunctUtil {
public:
  static Punct to_punct(std::string str);
  static Punct to_punct(std::string_view str);
  static Punct to_punct(const char* str);
};

} // namespace mt