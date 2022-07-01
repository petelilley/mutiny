#include <mutiny/ast/ast.hpp>

using namespace mt;

void ASTNode::dump(Logger& log, u32 indent) const {
  for (u32 i = 0; i < indent; i++) {
    log << "|  ";
  }

  log << '<' << LogStyle::CLEAR << LogStyle::BOLD;

  switch (kind) {
    case Kind::GLOBAL_SCOPE:
      log << "GlobalScope";
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
    case Kind::STMT_COMPOUND:
      log << "CompoundStmt";
      break;
    case Kind::TYPE:
      log << "Type";
      break;
    case Kind::IDENTIFIER:
      log << "Identifier";
      break;
    default:
      log << LogStyle::RED << "Unknown";
      break;
  }

  log << LogStyle::CLEAR;

  if (kind == Kind::GLOBAL_SCOPE) {
    log << LogStyle::MAGENTA << " '" << location.path.c_str() << "'";
  }
  else if (kind == Kind::FUNC_DECL || kind == Kind::TYPE || kind == Kind::IDENTIFIER) {
    log << LogStyle::YELLOW << " '" << get_value<std::string>() << '\'';
  }

  log << LogStyle::CLEAR << " location=(" << location.line << ", " << location.col << ") range=(" << location.len << ')';
  
  log << ">\n" << LogStyle::CLEAR;

  for (const auto& child : children) {
    child.dump(log, indent + 1);
  }
}