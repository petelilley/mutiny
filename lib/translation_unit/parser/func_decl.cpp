#include <mutiny/translation_unit/parser/parser.hpp>

using namespace mt;

std::optional<ASTNode> Parser::parse_func_decl() {
  std::optional<ASTNode> func_nd = std::nullopt,
                         param_list_nd = std::nullopt,
                         body_nd = std::nullopt,
                         ret_type_nd;

  SourceLoc start_loc;
  SourceLoc ret_type_loc;

  std::string name, return_type = "";

  do {
    // func
    start_loc = tok_iter->get_location();
    tok_iter++;

    // Function name.
    if (comp_token(Token::Kind::IDENTIFIER) == Token::Kind::UNKNOWN) {
      status.report_syntax(Status::ReportContext::ERROR, src_file, tok_iter->get_location(), fmt::format("{}, expected function name following 'func' keyword", unexpected_token()));
      break;
    }
    name = tok_iter->get_value<std::string>();
    tok_iter++;

    // :
    if (comp_token(Punct::COLON) == Punct::UNKNOWN) {
      status.report_syntax(Status::ReportContext::ERROR, src_file, tok_iter->get_location(), fmt::format("{}, expected ':' following function name", unexpected_token(Token::Kind::PUNCTUATOR)));
      break;
    }
    tok_iter++;

    // (
    if (comp_token(Punct::LPAREN) == Punct::UNKNOWN) {
      status.report_syntax(Status::ReportContext::ERROR, src_file, tok_iter->get_location(), fmt::format("{}, expected function parameter list following ':'", unexpected_token(Token::Kind::PUNCTUATOR)));
      break;
    }
    tok_iter++;

    // Function parameters.
    param_list_nd = parse_func_decl_param_list();

    if (!param_list_nd && status.get_error_num() > 0) break;
    tok_iter++;

    // ; or -> or {
    Punct p = comp_token(Punct::SEMICOLON, Punct::ARROW, Punct::LBRACE);

END_DECL:
    if (p == Punct::UNKNOWN) {
      if (return_type.empty()) {
        status.report_syntax(Status::ReportContext::ERROR, src_file, tok_iter->get_location(), fmt::format("{}, expected ';', '->', or '{{' following function parameter list", unexpected_token(Token::Kind::PUNCTUATOR)));
      }
      else {
        status.report_syntax(Status::ReportContext::ERROR, src_file, tok_iter->get_location(), fmt::format("{}, expected ';' or '{{' following function return type", unexpected_token(Token::Kind::PUNCTUATOR)));
      }
    }

    // ;
    if (p == Punct::SEMICOLON) {
      // No definition.
      tok_iter++;
    }
    // ->
    else if (p == Punct::ARROW) {
      tok_iter++;
      // Return type.
      if (comp_token(Token::Kind::IDENTIFIER) == Token::Kind::UNKNOWN) {
        status.report_syntax(Status::ReportContext::ERROR, src_file, tok_iter->get_location(), fmt::format("{}, expected return type following '->'", unexpected_token()));
        break;
      }
      
      return_type = tok_iter->get_value<std::string>();
      ret_type_loc = tok_iter->get_location();
      tok_iter++;

      // ; or {
      p = comp_token(Punct::SEMICOLON, Punct::LBRACE);

      goto END_DECL;
    }
    // {
    else if (p == Punct::LBRACE) {
      // Function body.
      /*
      std::optional<ASTNode> body = parse_stmt_compound();
      if (body) {
        // TODO: Add body to func ast node.
      }
      */
    }

    func_nd = ASTNode(ASTNode::Kind::FUNC_DECL, start_loc, name);

    if (!return_type.empty()) {
      ret_type_nd = ASTNode(ASTNode::Kind::TYPE, ret_type_loc, return_type);
      func_nd->add_child(std::move(ret_type_nd.value()));
    }

    if (param_list_nd) {
      func_nd->add_child(std::move(param_list_nd.value()));
    }

    if (body_nd) {
      func_nd->add_child(std::move(body_nd.value()));
    }

  } while (0);
  
  return func_nd;
}

std::optional<ASTNode> Parser::parse_func_decl_param_list() {
  // )
  if (comp_token(Punct::RPAREN) != Punct::UNKNOWN) return std::nullopt;

  SourceLoc start_loc = tok_iter->get_location();
  SourceLoc name_loc, type_loc;
  
  std::optional<ASTNode> param_list_nd = std::nullopt,
                         param_nd, name_nd, type_nd;

  std::string name, type;
  do {
    param_nd = name_nd = type_nd = std::nullopt;
    
    // Parameter name.
    if (comp_token(Token::Kind::IDENTIFIER) == Token::Kind::UNKNOWN) {
      status.report_syntax(Status::ReportContext::ERROR, src_file, tok_iter->get_location(), fmt::format("{}, expected parameter name or type", unexpected_token()));
      break;
    }
    name = tok_iter->get_value<std::string>();
    name_loc = tok_iter->get_location();
    tok_iter++;

    // :
    if (comp_token(Punct::COLON) == Punct::UNKNOWN) {
      // No colon, so its just an unamed parameter, therefore the first identifier is the type.
      type = name;
      type_loc = name_loc;
      name = "";
    }
    else {
      // Parameter type.
      tok_iter++;

      // TODO: Dedicated type parser function.
      if (comp_token(Token::Kind::IDENTIFIER) == Token::Kind::UNKNOWN) {
        status.report_syntax(Status::ReportContext::ERROR, src_file, tok_iter->get_location(), fmt::format("{}, expected parameter type following ':'", unexpected_token()));
        break;
      }
      type = tok_iter->get_value<std::string>();
      type_loc = tok_iter->get_location();
      tok_iter++;
    }

    if (!param_list_nd) {
      param_list_nd = ASTNode(ASTNode::Kind::FUNC_DECL_PARAM_LIST, start_loc);
    }

    if (!name.empty()) {
      name_nd = ASTNode(ASTNode::Kind::IDENTIFIER, name_loc, name);
    }
  
    type_nd = ASTNode(ASTNode::Kind::TYPE, type_loc, type);

    param_nd = ASTNode(ASTNode::Kind::FUNC_DECL_PARAM, name.empty() ? type_loc : name_loc);

    if (name_nd) {
      param_nd->add_child(std::move(name_nd.value()));
    }
    param_nd->add_child(std::move(type_nd.value()));

    param_list_nd->add_child(std::move(param_nd.value()));

    // , or )
    Punct p = comp_token(Punct::COMMA, Punct::RPAREN);
    if (p == Punct::UNKNOWN) {
      status.report_syntax(Status::ReportContext::ERROR, src_file, tok_iter->get_location(), fmt::format("{}, expected ')' to close function parameter list, or ',' to separate parameters", unexpected_token(Token::Kind::PUNCTUATOR)));
      param_list_nd = std::nullopt;
      break;
    }
    else if (p == Punct::RPAREN) {
      break;
    }
    tok_iter++;
  } while (true);

  return param_list_nd;
}