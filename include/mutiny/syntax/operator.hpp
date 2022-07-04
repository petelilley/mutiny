#pragma once

#include <mutiny/mutiny.hpp>
#include <mutiny/syntax/punctuator.hpp>

namespace mt {

enum class Operator : u16 {
  UNKNOWN = 0,
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
  INC_PRE,      // ++
  DEC_PRE,      // --
  INC_POST,     // ++
  DEC_POST,     // --
  MEMBER,       // .
  ARRAY,        // []
  REF,          // &
  DEREF,        // *
  POS,          // +
  NEG,          // -
};

class OpUtil {
public:
  static Operator to_operator(Punct punct);
  static const c8* to_string(Operator op);

  enum : u16 {
    MAX_PREC_LVL = 12,
    MIN_PREC_LVL = 0,
  };

  static u16 get_prec_lvl(Operator op);
};

} // namespace mt