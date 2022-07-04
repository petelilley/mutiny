#include <mutiny/translation_unit/parser/parser.hpp>

using namespace mt;

std::optional<ASTNode> Parser::parse_stmt_list() {
  ASTNode stmt_list_nd = ASTNode(ASTNode::Kind::LOCAL_SCOPE, tok_iter->get_location());
  ++tok_iter;

  std::optional<ASTNode> stmt_nd;

  do {
    if (comp_token(Punct::RBRACE) != Punct::UNKNOWN) break;
    
    // Statement.
    stmt_nd = parse_stmt();
    if (!stmt_nd && status.get_error_num() > 0) break;

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

  Token::Kind kind = comp_token(Token::Kind::IDENTIFIER, Token::Kind::KEYWORD, Token::Kind::PUNCTUATOR, Token::Kind::INT_LITERAL, Token::Kind::FLOAT_LITERAL, Token::Kind::STRING_LITERAL, Token::Kind::CHAR_LITERAL);

  do {
    if (kind == Token::Kind::IDENTIFIER) {
      ++tok_iter;
      // :
      if (comp_token(Punct::COLON) != Punct::UNKNOWN) {
        // Label.
        // stmt_nd = parse_label();
        ++tok_iter; //
      }
      else {
        --tok_iter;
        // Expression.
        stmt_nd = parse_expr();
        if (!stmt_nd) break;
        b8 sc = comp_token(Punct::SEMICOLON) != Punct::UNKNOWN;
        if (!sc) {
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
      Keyword kw = comp_token(Keyword::VAR, Keyword::IF, Keyword::WHILE, Keyword::RETURN, Keyword::GOTO);
      
      if (kw == Keyword::UNKNOWN) {
        status.report_syntax(Status::ReportContext::ERROR, src_file, tok_iter->get_location(), fmt::format("{} in statement list", unexpected_token(Token::Kind::KEYWORD)));
        break;
      }

      else if (kw == Keyword::VAR) {
        stmt_nd = parse_var_decl();
      }
      else if (kw == Keyword::IF) {
        // stmt_nd = parse_if_stmt();
        ++tok_iter; //
      }
      else if (kw == Keyword::WHILE) {
        // stmt_nd = parse_while_stmt();
        ++tok_iter; //
      }
      else if (kw == Keyword::RETURN) {
        // stmt_nd = parse_return_stmt();
        ++tok_iter; //
      }
      else if (kw == Keyword::GOTO) {
        // stmt_nd = parse_goto_stmt();
        ++tok_iter; //
      }
    }
    else {
      if (comp_token(Punct::SEMICOLON) != Punct::UNKNOWN) {
        ++tok_iter;
        break;
      }
      // Expression.
      stmt_nd = parse_expr();
      if (!stmt_nd) break;
      b8 sc = comp_token(Punct::SEMICOLON) != Punct::UNKNOWN;
      if (!sc) {
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
