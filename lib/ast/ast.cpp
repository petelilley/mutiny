#include <mutiny/ast/ast.hpp>

using namespace mt;

void ASTNode::dump(Logger& log, u32 indent) const {
  for (u32 i = 0; i < indent; i++) {
    log << ' ';
  }

  switch (kind) {
    case Kind::GLOBAL_SCOPE:
      log << "GLOBAL_SCOPE\n";
      break;
    case Kind::FUNC_DECL:
      log << "FUNC_DECL\n";
      break;
    case Kind::FUNC_DECL_PARAM_LIST:
      log << "FUNC_DECL_PARAM_LIST\n";
      break;
    case Kind::FUNC_DECL_PARAM:
      log << "FUNC_DECL_PARAM\n";
      break;
    case Kind::STMT_COMPOUND:
      log << "STMT_COMPOUND\n";
      break;
    case Kind::TYPE:
      log << "TYPE\n";
      break;
    case Kind::IDENTIFIER:
      log << "IDENTIFIER\n";
      break;
    default:
      log << "UNKNOWN\n";
      break;
  }

  for (const auto& child : children) {
    child.dump(log, indent + 1);
  }
}