#include <mutiny/ast/ast.hpp>

using namespace mt;

void ASTNode::dump(Logger& log, u32 indent) const {
  for (u32 i = 0; i < indent; i++) {
    log << "|  ";
  }

  log << "|<" << LogStyle::CLEAR << LogStyle::BOLD;

  // --- Kind ---
  
  switch (kind) {
    case Kind::GLOBAL_SCOPE:
      log << "GlobalScope";
      break;
    case Kind::LOCAL_SCOPE:
      log << "LocalScope";
      break;
    case Kind::FUNC_DECL:
      log << "FuncDecl";
      break;
    case Kind::FUNC_DECL_PARAM_LIST:
      log << "FuncDeclParamList";
      break;
    case Kind::FUNC_DECL_PARAM:
      log << "FuncDeclParam";
      break;
    case Kind::TYPE:
      log << "Type";
      break;
    case Kind::VAR_DECL:
      log << "VarDecl";
      break;
    case Kind::EXPR:
      log << "Expr";
      break;
    case Kind::FUNC_CALL:
      log << "FuncCall";
      break;
    case Kind::FUNC_CALL_PARAM_LIST:
      log << "FuncCallParamList";
      break;
    case Kind::LABEL:
      log << "Label";
      break;
    case Kind::IF_STMT:
      log << "IfStmt";
      break;
    case Kind::WHILE_STMT:
      log << "WhileStmt";
      break;
    case Kind::RETURN_STMT:
      log << "ReturnStmt";
      break;
    case Kind::GOTO_STMT:
      log << "GotoStmt";
      break;
    case Kind::IDENTIFIER:
      log << "Identifier";
      break;
    case Kind::INT_LITERAL:
      log << "Int";
      break;
    case Kind::FLOAT_LITERAL:
      log << "Float";
      break;
    case Kind::STRING_LITERAL:
      log << "String";
      break;
    case Kind::CHAR_LITERAL:
      log << "Char";
      break;
    case Kind::PREFIX_OP:
      log << "OpPrefix";
      break;
    case Kind::SUFFIX_OP:
      log << "OpSuffix";
      break;
    case Kind::ARITH_OP:
      log << "OpArith";
      break;
    case Kind::ASGN_OP:
      log << "OpAsgn";
      break;
    case Kind::CMP_OP:
      log << "OpCmp";
      break;
    case Kind::LOG_OP:
      log << "OpLog";
      break;
    default:
      log << LogStyle::RED << "Unknown";
      break;
  }

  log << LogStyle::CLEAR;

  // --- Value ---

  switch (kind) {
    case Kind::GLOBAL_SCOPE:
      log << LogStyle::MAGENTA << " '" << location.path.c_str() << "'";
      break;
    case Kind::FUNC_DECL:
    case Kind::TYPE:
    case Kind::VAR_DECL:
    case Kind::FUNC_CALL:
    case Kind::LABEL:
    case Kind::IDENTIFIER:
    case Kind::STRING_LITERAL:
      log << LogStyle::YELLOW << " '" << get_value<std::string>() << '\'';
      break;
    case Kind::INT_LITERAL:
      log << LogStyle::CYAN << " '" << get_value<u64>() << '\'';
      break;
    case Kind::FLOAT_LITERAL:
      log << LogStyle::CYAN << " '" << get_value<f128>() << '\'';
      break;
    case Kind::CHAR_LITERAL:
      log << LogStyle::YELLOW << " '" << get_value<c8>() << '\'';
      break;
    case Kind::PREFIX_OP:
    case Kind::SUFFIX_OP:
    case Kind::ARITH_OP:
    case Kind::ASGN_OP:
    case Kind::CMP_OP:
    case Kind::LOG_OP:
      log << LogStyle::BLUE << " '" << OpUtil::to_string(get_value<Operator>()) << '\'';
      break;
    default:
      break;
  }

  log << LogStyle::CLEAR << " range=(";
  log << location.line_i << "," << location.col_i << " to " << location.line_f << "," << location.col_f << ')';
  
  log << ">\n";

  for (const auto& child : children) {
    child.dump(log, indent + 1);
  }
}
