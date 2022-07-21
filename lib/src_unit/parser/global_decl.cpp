#include <mutiny/src_unit/parser/parser.hpp>

using namespace mt;

std::optional<ASTNode> Parser::parse_global_decl() {
  Keyword matched = comp_token(Keyword::FUNC, Keyword::VAR);

  switch (matched) {
    case Keyword::FUNC:
      return parse_func_decl();
    case Keyword::VAR:
      return parse_var_decl();
    default:
      status.report_syntax(Status::ReportContext::ERROR, src_file, tok_iter->get_location(), fmt::format("{}, expected global declaration", unexpected_token(Token::Kind::KEYWORD)));
      break;
  }

  return std::nullopt;
}
