#include <mutiny/src_unit/parser/parser.hpp>

using namespace mt;

std::optional<ASTNode> Parser::parse_var_decl() {
  std::optional<ASTNode> decl_nd, type_nd, expr_nd;
  SourceLoc start(tok_iter->get_location());

  std::string name;

  do {
    // var
    ++tok_iter;

    // Variable name.
    if (comp_token(Token::Kind::IDENTIFIER) == Token::Kind::UNKNOWN) {
      status.report_syntax(Status::ReportContext::ERROR, src_file, tok_iter->get_location(), fmt::format("{}, expected variable name following 'var'", unexpected_token(Token::Kind::IDENTIFIER)));
      break;
    }

    name = tok_iter->get_value<std::string>();

    ++tok_iter;

    // : specifying the type of the variable, or an assignment with an inferred type.
    Punct p(comp_token(Punct::COLON, Punct::ASGN, Punct::SEMICOLON));
    if (p == Punct::UNKNOWN) {
      status.report_syntax(Status::ReportContext::ERROR, src_file, tok_iter->get_location(), fmt::format("{}, expected ':' or '=' following variable name", unexpected_token(Token::Kind::PUNCTUATOR)));
      break;
    }
    else if (p == Punct::SEMICOLON) {
      status.report_syntax(Status::ReportContext::ERROR, src_file, tok_iter->get_location(), "variable requires a type if not initialized");
      break;
    }
    // =
    else if (p == Punct::ASGN) {
      ++tok_iter;
    }
    // :
    else if (p == Punct::COLON) {
      ++tok_iter;

      // The type of the variable.
      type_nd = parse_var_type();
      if (!type_nd || status.get_error_num()) break;

      // = or ;
      p = comp_token(Punct::ASGN, Punct::SEMICOLON);
      if (p == Punct::UNKNOWN) {
        status.report_syntax(Status::ReportContext::ERROR, src_file, tok_iter->get_location(), fmt::format("{}, expected ';' or '=' following variable type", unexpected_token(Token::Kind::PUNCTUATOR)));
        break;
      }

      ++tok_iter;
    }

    // =
    if (p == Punct::ASGN) {
      // The initial value of the variable.
      expr_nd = parse_expr();
      if (!expr_nd || status.get_error_num()) break;

      // ;
      if (comp_token(Punct::SEMICOLON) == Punct::UNKNOWN) {
        status.report_syntax(Status::ReportContext::ERROR, src_file, SourceLoc::at(src_file.get_path(), (tok_iter - 1)->get_location().line_f, (tok_iter - 1)->get_location().col_f + 1), "expected ';' following variable initialization", "insert ';' here");
        break;
      }
      ++tok_iter;
    }

    decl_nd = ASTNode(ASTNode::Kind::VAR_DECL, SourceLoc::cat(start, (tok_iter - 1)->get_location()), std::move(name));

    if (type_nd) {
      decl_nd->add_child(std::move(type_nd.value()));
    }

    if (expr_nd) {
      decl_nd->add_child(std::move(expr_nd.value()));
    }

  } while (false);

  return decl_nd;
}
