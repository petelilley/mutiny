#include <mutiny/translation_unit/parser/parser.hpp>

using namespace mt;

std::optional<ASTNode> Parser::parse_stmt_list() {
  ASTNode stmt_list_nd(ASTNode::Kind::LOCAL_SCOPE, tok_iter->get_location());
  ++tok_iter;

  std::optional<ASTNode> stmt_nd;

  do {
    // } Signifies the end of the statement list.
    if (comp_token(Punct::RBRACE) != Punct::UNKNOWN) break;
    
    // Statement.
    stmt_nd = parse_stmt();
    if (status.get_error_num()) break;

    if (stmt_nd) {
      stmt_list_nd.add_child(std::move(stmt_nd.value()));
    }
  } while (true);

  stmt_list_nd.set_location(SourceLoc::cat(stmt_list_nd.get_location(), tok_iter->get_location()));

  if (comp_token(Punct::RBRACE) != Punct::UNKNOWN) {
    ++tok_iter;
  }

  return stmt_list_nd;
}

std::optional<ASTNode> Parser::parse_stmt() {
  std::optional<ASTNode> stmt_nd;

  Token::Kind kind(comp_token(Token::Kind::IDENTIFIER, Token::Kind::KEYWORD, Token::Kind::PUNCTUATOR, Token::Kind::INT_LITERAL, Token::Kind::FLOAT_LITERAL, Token::Kind::STRING_LITERAL, Token::Kind::CHAR_LITERAL));

  do {
    if (kind == Token::Kind::IDENTIFIER) {
      // :
      if (peek_comp_token(Punct::COLON) != Punct::UNKNOWN) {
        // Label.
        // stmt_nd = parse_label();
        ++tok_iter; //
      }
      else {
        // Expression.
        stmt_nd = parse_expr();
        if (!stmt_nd) break;

        // ; Following an expression.
        b8 sc = comp_token(Punct::SEMICOLON) != Punct::UNKNOWN;
        if (!sc) {
          // Point to the space directly following the expression.
          SourceLoc sc_loc = (tok_iter - 1)->get_location();
          sc_loc.line_i = sc_loc.line_f;
          sc_loc.col_i = ++sc_loc.col_f;
          status.report_syntax(Status::ReportContext::ERROR, src_file, sc_loc, "expected ';' following expression in statement list", "insert ';' here");
          break;
        }
        ++tok_iter;
      }
    }
    else if (kind == Token::Kind::KEYWORD) {
      Keyword kw(comp_token(Keyword::VAR, Keyword::IF, Keyword::WHILE, Keyword::RETURN, Keyword::GOTO));
      
      // Invalid keyword.
      if (kw == Keyword::UNKNOWN) {
        status.report_syntax(Status::ReportContext::ERROR, src_file, tok_iter->get_location(), fmt::format("{} in statement list", unexpected_token(Token::Kind::KEYWORD)));
        break;
      }
      // Local variable declaration.
      else if (kw == Keyword::VAR) {
        stmt_nd = parse_var_decl();
      }
      // If statement.
      else if (kw == Keyword::IF) {
        stmt_nd = parse_if_stmt();
      }
      // While statement.
      else if (kw == Keyword::WHILE) {
        // stmt_nd = parse_while_stmt();
        ++tok_iter; //
      }
      // Return statement.
      else if (kw == Keyword::RETURN) {
        // stmt_nd = parse_return_stmt();
        ++tok_iter; //
      }
      // Goto statement.
      else if (kw == Keyword::GOTO) {
        // stmt_nd = parse_goto_stmt();
        ++tok_iter; //
      }
    }
    // An expression or an empty statement.
    else {
      // An empty statement.
      if (comp_token(Punct::SEMICOLON) != Punct::UNKNOWN) {
        ++tok_iter;
        break;
      }
      // Expression.
      stmt_nd = parse_expr();
      if (!stmt_nd) break;

      // ; Following an expression.
      b8 sc = comp_token(Punct::SEMICOLON) != Punct::UNKNOWN;
      if (!sc) {
        // Point to the space directly following the expression.
        SourceLoc sc_loc = (tok_iter - 1)->get_location();
        sc_loc.line_i = sc_loc.line_f;
        sc_loc.col_i = ++sc_loc.col_f;
        status.report_syntax(Status::ReportContext::ERROR, src_file, sc_loc, "expected ';' following expression in statement list", "insert ';' here");
        break;
      }
      ++tok_iter;
    }
    
  } while (false);

  return stmt_nd;
}

std::optional<ASTNode> Parser::parse_if_stmt() {
  std::optional<ASTNode> stmt_nd, cond_nd, body_nd;

  SourceLoc start(tok_iter->get_location());

  do {
    ++tok_iter;

    // The condition.
    cond_nd = parse_expr();
    if (!cond_nd || status.get_error_num()) break;

    // } following the condition.
    if (comp_token(Punct::LBRACE) != Punct::UNKNOWN) {
      body_nd = parse_stmt_list();
      if (status.get_error_num()) break;
    }
    else {
      status.report_syntax(Status::ReportContext::ERROR, src_file, SourceLoc::at(src_file.get_path(), cond_nd->get_location().line_f, cond_nd->get_location().col_f + 1), "expected '{' following condition in if statement");
    }

    stmt_nd = ASTNode(ASTNode::Kind::IF_STMT, SourceLoc::cat(start, (tok_iter - 1)->get_location()));
    stmt_nd->add_child(std::move(cond_nd.value()));
    
    if (body_nd) {
      stmt_nd->add_child(std::move(body_nd.value()));
    }
    
  } while (false);

  return stmt_nd;
}
