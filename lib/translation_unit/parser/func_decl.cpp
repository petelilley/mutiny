#include <mutiny/translation_unit/parser/parser.hpp>

using namespace mt;

std::optional<ASTNode> Parser::parse_func_decl() {
  std::optional<ASTNode> func_nd, param_list_nd, body_nd, ret_type_nd;
  SourceLoc start_loc, end_loc;
  std::string name;

  do {
    // func
    start_loc = tok_iter->get_location();
    ++tok_iter;

    // Function name.
    if (comp_token(Token::Kind::IDENTIFIER) == Token::Kind::UNKNOWN) {
      status.report_syntax(Status::ReportContext::ERROR, src_file, tok_iter->get_location(), fmt::format("{}, expected function name following 'func' keyword", unexpected_token()));
      break;
    }
    name = tok_iter->get_value<std::string>();
    ++tok_iter;

    // (
    if (comp_token(Punct::LPAREN) == Punct::UNKNOWN) {
      status.report_syntax(Status::ReportContext::ERROR, src_file, tok_iter->get_location(), fmt::format("{}, expected function parameter list following function name", unexpected_token(Token::Kind::PUNCTUATOR)));
      break;
    }
    ++tok_iter;

    // Function parameters.
    param_list_nd = parse_func_decl_param_list();

    if (status.get_error_num()) break;
    ++tok_iter;

    // ; or -> or {
    Punct p(comp_token(Punct::SEMICOLON, Punct::ARROW, Punct::LBRACE));

END_DECL:
    if (p == Punct::UNKNOWN) {
      if (!ret_type_nd) {
        status.report_syntax(Status::ReportContext::ERROR, src_file, tok_iter->get_location(), fmt::format("{}, expected ';', '->', or '{{' following function parameter list", unexpected_token(Token::Kind::PUNCTUATOR)));
      }
      else {
        status.report_syntax(Status::ReportContext::ERROR, src_file, tok_iter->get_location(), fmt::format("{}, expected ';' or '{{' following function return type", unexpected_token(Token::Kind::PUNCTUATOR)));
      }
    }

    // ;
    if (p == Punct::SEMICOLON) {
      // No definition.
      end_loc = tok_iter->get_location();
      ++tok_iter;
    }
    // ->
    else if (p == Punct::ARROW) {
      ++tok_iter;
      
      // Return type.
      ret_type_nd = parse_var_type();

      // ; or {
      p = comp_token(Punct::SEMICOLON, Punct::LBRACE);

      goto END_DECL;
    }
    // {
    else if (p == Punct::LBRACE) {
      // Function body.
      body_nd = parse_stmt_list();
      
      end_loc = (tok_iter - 1)->get_location();
      
      if (status.get_error_num()) break;
    }

    func_nd = ASTNode(ASTNode::Kind::FUNC_DECL, SourceLoc::cat(start_loc, end_loc), name);

    if (ret_type_nd) {
      func_nd->add_child(std::move(ret_type_nd.value()));
    }

    if (param_list_nd) {
      func_nd->add_child(std::move(param_list_nd.value()));
    }

    if (body_nd) {
      func_nd->add_child(std::move(body_nd.value()));
    }

  } while (false);
  
  return func_nd;
}

std::optional<ASTNode> Parser::parse_func_decl_param_list() {
  // )
  if (comp_token(Punct::RPAREN) != Punct::UNKNOWN) return std::nullopt;

  SourceLoc start_loc = (tok_iter - 1)->get_location(), end_loc, name_loc, type_loc;
  std::optional<ASTNode> param_list_nd, param_nd, name_nd, type_nd;
  std::string name, type;
  
  do {
    param_nd = name_nd = type_nd = std::nullopt;

    ++tok_iter;
    // :
    if (comp_token(Punct::COLON) != Punct::UNKNOWN) {
      --tok_iter;
      // Parameter name.
      if (comp_token(Token::Kind::IDENTIFIER) == Token::Kind::UNKNOWN) {
        status.report_syntax(Status::ReportContext::ERROR, src_file, tok_iter->get_location(), fmt::format("{}, expected parameter name", unexpected_token()));
        break;
      }
      
      name = tok_iter->get_value<std::string>();
      name_loc = tok_iter->get_location();
      tok_iter += 2;
    }
    else {
      --tok_iter;
    }

    // Parameter type.
    type_nd = parse_var_type();
    if (status.get_error_num()) break;

    if (!param_list_nd) {
      param_list_nd = ASTNode(ASTNode::Kind::FUNC_DECL_PARAM_LIST, start_loc);
    }

    if (!name.empty()) {
      name_nd = ASTNode(ASTNode::Kind::IDENTIFIER, name_loc, name);
    }

    if (name.empty()) {
      param_nd = ASTNode(ASTNode::Kind::FUNC_DECL_PARAM, type_loc);
    }
    else {
      param_nd = ASTNode(ASTNode::Kind::FUNC_DECL_PARAM, SourceLoc::cat(name_loc, type_loc));
    }

    if (name_nd) {
      param_nd->add_child(std::move(name_nd.value()));
    }

    param_nd->add_child(std::move(type_nd.value()));

    param_list_nd->add_child(std::move(param_nd.value()));

    // , or )
    Punct p(comp_token(Punct::COMMA, Punct::RPAREN));
    if (p == Punct::UNKNOWN) {
      status.report_syntax(Status::ReportContext::ERROR, src_file, tok_iter->get_location(), fmt::format("{}, expected ')' to close function parameter list, or ',' to separate parameters", unexpected_token(Token::Kind::PUNCTUATOR)));
      param_list_nd = std::nullopt;
      break;
    }
    else if (p == Punct::RPAREN) {
      if (param_list_nd) {
        param_list_nd->set_location(SourceLoc::cat(param_list_nd->get_location(), tok_iter->get_location()));
      }
      break;
    }
    ++tok_iter;
  } while (true);

  return param_list_nd;
}
