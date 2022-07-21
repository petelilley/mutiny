#include <mutiny/src_unit/parser/parser.hpp>

using namespace mt;

std::optional<ASTNode> Parser::parse_var_type() {
  std::optional<ASTNode> type_nd;
  SourceLoc start_loc = tok_iter->get_location(), end_loc;
  
  std::string name;
  do {
    // Type.
    start_loc = tok_iter->get_location();
    
    // TODO: Type Qualifiers and Type Specifiers.
    
    // Type name.
    if (comp_token(Token::Kind::IDENTIFIER) == Token::Kind::UNKNOWN) {
      status.report_syntax(Status::ReportContext::ERROR, src_file, tok_iter->get_location(), fmt::format("{}, expected type name", unexpected_token()));
      break;
    }
    name = tok_iter->get_value<std::string>();
    
    end_loc = tok_iter->get_location();
    ++tok_iter;

    type_nd = ASTNode(ASTNode::Kind::TYPE, SourceLoc::cat(start_loc, end_loc), name);
  } while (false);
  
  return type_nd;
}
