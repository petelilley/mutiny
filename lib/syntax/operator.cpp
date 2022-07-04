#include <mutiny/syntax/operator.hpp>

using namespace mt;

static constexpr std::array<const char*, 41> op_strs = {
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
  "++",
  "--",
  ".",
  "[",
  "&",
  "*",
  "+",
  "-",
};

Operator OpUtil::to_operator(Punct punct) {
  const c8* punct_str = PunctUtil::to_string(punct);

  for (u16 i = 0; i < op_strs.size(); i++) {
    if (std::strcmp(punct_str, op_strs.at(i)) == 0) {
      return static_cast<Operator>(i + 1);
    }
  }
  return Operator::UNKNOWN;
}

const c8* OpUtil::to_string(Operator op) {
  if (op == Operator::UNKNOWN) {
    return "UNKNOWN";
  }
  return op_strs.at(static_cast<u16>(op) - 1);
}

u16 OpUtil::get_prec_lvl(Operator op) {
  switch (op) {
    case Operator::INC_PRE:      // ++ before
    case Operator::DEC_PRE:      // -- before
    case Operator::MEMBER:       // .
    case Operator::ARRAY:        // []
      return MAX_PREC_LVL;
    case Operator::BIT_NOT:      // ~
    case Operator::LOG_NOT:      // !
    case Operator::REF:          // &
    case Operator::DEREF:        // *
    case Operator::INC_POST:     // ++ after
    case Operator::DEC_POST:     // -- after
    case Operator::POS:          // + before
    case Operator::NEG:          // - before
      return MAX_PREC_LVL - 1;
    case Operator::MUL:          // *
    case Operator::DIV:          // /
    case Operator::MOD:          // %
      return MAX_PREC_LVL - 2;
    case Operator::ADD:          // +
    case Operator::SUB:          // -
      return MAX_PREC_LVL - 3;
    case Operator::BIT_SHL:      // <<
    case Operator::BIT_SHR:      // >>
      return MAX_PREC_LVL - 4;
    case Operator::CMP_GT:       // >
    case Operator::CMP_GE:       // >=
    case Operator::CMP_LT:       // <
    case Operator::CMP_LE:       // <=
      return MAX_PREC_LVL - 5;
    case Operator::CMP_EQ:       // ==
    case Operator::CMP_NE:       // !=
      return MAX_PREC_LVL - 6;
    case Operator::BIT_AND:      // &
      return MAX_PREC_LVL - 7;
    case Operator::BIT_XOR:      // ^
      return MAX_PREC_LVL - 8;
    case Operator::BIT_OR:       // |
      return MAX_PREC_LVL - 9;
    case Operator::LOG_AND:      // &&
      return MAX_PREC_LVL - 10;
    case Operator::LOG_OR:       // ||
      return MAX_PREC_LVL - 11;
    case Operator::ASGN:         // =
    case Operator::ASGN_ADD:     // +=
    case Operator::ASGN_SUB:     // -=
    case Operator::ASGN_MUL:     // *=
    case Operator::ASGN_DIV:     // /=
    case Operator::ASGN_MOD:     // %=
    case Operator::ASGN_BIT_AND: // &=
    case Operator::ASGN_BIT_OR:  // |=
    case Operator::ASGN_BIT_XOR: // ^=
    case Operator::ASGN_BIT_SHL: // <<=
    case Operator::ASGN_BIT_SHR: // >>=
      return MAX_PREC_LVL - 12;
  }
}