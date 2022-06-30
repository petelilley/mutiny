#include <mutiny/translation_unit/parser/parser.hpp>
#include <mutiny/ast/ast.hpp>

using namespace mt;

std::optional<ASTNode> Parser::parse_global_decl() {
  Keyword matched = match_token_keyword(Keyword::FUNC, Keyword::VAR);

  switch (matched) {
    case Keyword::FUNC:
      // return parse_func_decl();
    case Keyword::VAR:
      // return parse_var_decl();
    default:
      break;
  }

  return std::nullopt;
}