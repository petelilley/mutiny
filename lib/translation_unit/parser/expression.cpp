#include <mutiny/translation_unit/parser/parser.hpp>

using namespace mt;

std::optional<ASTNode> Parser::parse_expr() {
  std::vector<ExprFragment> frags;
  std::optional<ExprFragment> frag;
  std::optional<ASTNode> tmp_nd;

  b8 expect_unit = true;
  do {
    if (expect_unit) {
      tmp_nd = parse_expr_unit();
      if (!tmp_nd) break;

      frag = { ExprFragment::Kind::UNIT, std::move(tmp_nd.value()) };
      expect_unit = false;
    }
    else {
      if (comp_token(Token::Kind::PUNCTUATOR) == Token::Kind::UNKNOWN) {
        expect_unit = true;
        status.report_syntax(Status::ReportContext::ERROR, src_file, tok_iter->get_location(), fmt::format("{}, expected puncutator", unexpected_token(Token::Kind::PUNCTUATOR)));
        break;
      }

      tmp_nd = parse_operator();
      if (!tmp_nd) break;

      frag = { ExprFragment::Kind::OPERATOR, std::move(tmp_nd.value()) };
      expect_unit = true;
    }

    frags.push_back(std::move(frag).value());
  } while (true);

  std::optional<ASTNode> expr_nd;

  if (!expect_unit) {
    if (!frags.empty()) {
      expr_nd = fragments_to_expr(frags);
    }
  }

  return expr_nd;
}

std::optional<ASTNode> Parser::parse_func_call() {
  std::optional<ASTNode> func_call_nd, param_list_nd;

  SourceLoc start_loc(tok_iter->get_location());

  std::string name(tok_iter->get_value<std::string>());

  do {
    // Identifier and (
    tok_iter += 2;

    // Parameter list.
    param_list_nd = parse_func_call_param_list();

    if (status.get_error_num()) break;

    func_call_nd = ASTNode(ASTNode::Kind::FUNC_CALL, SourceLoc::cat(start_loc, tok_iter->get_location()), name);

    if (param_list_nd) {
      func_call_nd->add_child(std::move(param_list_nd.value()));
    }
    
    ++tok_iter;
  } while (false);

  return func_call_nd;
}

std::optional<ASTNode> Parser::parse_func_call_param_list() {
  // )
  if (comp_token(Punct::RPAREN) != Punct::UNKNOWN) return std::nullopt;

  SourceLoc start_loc((tok_iter - 1)->get_location()), end_loc;
  SourceLoc name_loc, type_loc;

  std::optional<ASTNode> param_list_nd, param_nd;

  do {
    // A parameter.
    param_nd = parse_expr();
    if (!param_nd) break;

    if (!param_list_nd) {
      param_list_nd = ASTNode(ASTNode::Kind::FUNC_CALL_PARAM_LIST, SourceLoc::cat(start_loc, param_nd.value().get_location()));
    }

    param_list_nd->add_child(std::move(param_nd.value()));

    // , or )
    Punct p = comp_token(Punct::COMMA, Punct::RPAREN);
    if (p == Punct::UNKNOWN) {
      status.report_syntax(Status::ReportContext::ERROR, src_file, tok_iter->get_location(), fmt::format("{}, expected ')' to close function call parameter list, or ',' to separate parameters", unexpected_token(Token::Kind::PUNCTUATOR)));
      break;
    }
    // )
    else if (p == Punct::RPAREN) {
      if (param_list_nd) {
        param_list_nd->set_location(SourceLoc::cat(param_list_nd->get_location(), tok_iter->get_location()));
      }
      break;
    }
    // ,
    ++tok_iter;
  } while (true);

  return param_list_nd;
}

std::optional<ASTNode> Parser::parse_expr_unit() {
  std::optional<ASTNode> unit_nd;

  do {
    // TODO: Parentheses check.

    // TODO: Check for prefix operators (++, --, &, *, !, +, -, ~).

    Token::Kind kind(comp_token(Token::Kind::IDENTIFIER, Token::Kind::INT_LITERAL, Token::Kind::FLOAT_LITERAL, Token::Kind::STRING_LITERAL, Token::Kind::CHAR_LITERAL, Token::Kind::PUNCTUATOR));

    if (kind == Token::Kind::PUNCTUATOR && comp_token(Punct::LPAREN) == Punct::UNKNOWN) {
      kind = Token::Kind::UNKNOWN;
    }
    
    if (kind == Token::Kind::UNKNOWN) {
      status.report_syntax(Status::ReportContext::ERROR, src_file, tok_iter->get_location(), fmt::format("{}, expected identifier or literal in an expression unit", unexpected_token(Token::Kind::IDENTIFIER)));
      break;
    }
    else if (kind == Token::Kind::IDENTIFIER) {
      ++tok_iter;
      if (comp_token(Punct::LPAREN) != Punct::UNKNOWN) {
        --tok_iter;
        // Function call.
        unit_nd = parse_func_call();
        if (!unit_nd) break;
      }
      else {
        --tok_iter;
        // Some identifier.
        unit_nd = ASTNode(ASTNode::Kind::IDENTIFIER, tok_iter->get_location(), tok_iter->get_value<std::string>());
        ++tok_iter;
      }
    }
    else if (kind == Token::Kind::INT_LITERAL) {
      unit_nd = ASTNode(ASTNode::Kind::INT_LITERAL, tok_iter->get_location(), tok_iter->get_value<u64>());
      ++tok_iter;
    }
    else if (kind == Token::Kind::FLOAT_LITERAL) {
      unit_nd = ASTNode(ASTNode::Kind::FLOAT_LITERAL, tok_iter->get_location(), tok_iter->get_value<f128>());
      ++tok_iter;
    }
    else if (kind == Token::Kind::STRING_LITERAL) {
      unit_nd = ASTNode(ASTNode::Kind::STRING_LITERAL, tok_iter->get_location(), tok_iter->get_value<std::string>());
      ++tok_iter;
    }
    else if (kind == Token::Kind::CHAR_LITERAL) {
      unit_nd = ASTNode(ASTNode::Kind::CHAR_LITERAL, tok_iter->get_location(), tok_iter->get_value<c8>());
      ++tok_iter;
    }
    else if (kind == Token::Kind::PUNCTUATOR) {
      ++tok_iter;
      unit_nd = parse_expr();
      if (status.get_error_num()) break;
      if (comp_token(Punct::RPAREN) == Punct::UNKNOWN) {
        status.report_syntax(Status::ReportContext::ERROR, src_file, tok_iter->get_location(), fmt::format("{}, expected ')' to close parenthesized expression", unexpected_token(Token::Kind::PUNCTUATOR)));
        break;
      }
      ++tok_iter;
      break;
    }

    // TODO: Check for suffix operators (++, --).
  } while (false);

  return unit_nd;
}

std::optional<ASTNode> Parser::parse_operator() {
  std::optional<ASTNode> op_nd;
  
  do {
    if (comp_token(Token::Kind::PUNCTUATOR) == Token::Kind::UNKNOWN) {
      status.report_syntax(Status::ReportContext::ERROR, src_file, tok_iter->get_location(), fmt::format("{}, expected operaror", unexpected_token(Token::Kind::PUNCTUATOR)));
      break;
    }

    switch (tok_iter->get_value<Punct>()) {
      case Punct::ADD:          // +
      case Punct::SUB:          // -
      case Punct::MUL:          // *
      case Punct::DIV:          // /
      case Punct::MOD:          // %
      case Punct::BIT_AND:      // &
      case Punct::BIT_OR:       // |
      case Punct::BIT_XOR:      // ^
      case Punct::BIT_SHL:      // <<
      case Punct::BIT_SHR:      // >>
        op_nd = ASTNode(ASTNode::Kind::ARITH_OP, tok_iter->get_location());
        break;
      case Punct::ASGN:         // =
      case Punct::ASGN_ADD:     // +=
      case Punct::ASGN_SUB:     // -=
      case Punct::ASGN_MUL:     // *=
      case Punct::ASGN_DIV:     // /=
      case Punct::ASGN_MOD:     // %=
      case Punct::ASGN_BIT_AND: // &=
      case Punct::ASGN_BIT_OR:  // |=
      case Punct::ASGN_BIT_XOR: // ^=
      case Punct::ASGN_BIT_SHL: // <<=
      case Punct::ASGN_BIT_SHR: // >>=
        op_nd = ASTNode(ASTNode::Kind::ASGN_OP, tok_iter->get_location());
        break;
      case Punct::CMP_EQ:       // ==
      case Punct::CMP_NE:       // !=
      case Punct::CMP_LT:       // <
      case Punct::CMP_LE:       // <=
      case Punct::CMP_GT:       // >
      case Punct::CMP_GE:       // >=
        op_nd = ASTNode(ASTNode::Kind::CMP_OP, tok_iter->get_location());
        break;
      case Punct::LOG_AND:      // &&
      case Punct::LOG_OR:       // ||
        op_nd = ASTNode(ASTNode::Kind::LOG_OP, tok_iter->get_location());
        break;
      default:
        break;
    }
    
    if (op_nd) {
      op_nd->set_value<Operator>(OpUtil::to_operator(tok_iter->get_value<Punct>()));
      ++tok_iter;
    }
  } while (false);

  return op_nd;
}

std::optional<ASTNode> Parser::fragments_to_expr(std::vector<ExprFragment>& frags) {
  if (frags.size() == 1) return frags.at(0).node;

  using prec_lvl = std::pair<u16, std::vector<ExprFragment>::const_iterator>;

  prec_lvl highest_prec(OpUtil::MIN_PREC_LVL, frags.cbegin() + 1);

  // Find the operator with the highest precedence level.
  for (auto it = frags.cbegin() + 1; it != frags.cend(); it += 2) {
    u16 lvl = OpUtil::get_prec_lvl(it->node.get_value<Operator>());
    
    if (lvl > highest_prec.first) {
      highest_prec = prec_lvl(lvl, it);
    }
  }

  ASTNode lhs_nd(frags.at(highest_prec.second - frags.cbegin() - 1).node),
          op_nd(frags.at(highest_prec.second - frags.cbegin()).node),
          rhs_nd(frags.at(highest_prec.second - frags.cbegin() + 1).node);

  // Create the expression node.
  ASTNode unit_nd(ASTNode::Kind::EXPR, SourceLoc::cat(lhs_nd.get_location(), rhs_nd.get_location()));

  unit_nd.add_child(std::move(lhs_nd));
  unit_nd.add_child(std::move(op_nd));
  unit_nd.add_child(std::move(rhs_nd));

  // If this is the last/only operator, return the unit node.
  if (frags.size() == 3) {
    return unit_nd;
  }

  // Put the unit node into the expression fragment list.
  frags.at(highest_prec.second - 1 - frags.cbegin()) = { ExprFragment::Kind::UNIT, unit_nd };

  // Remove the extra fragments.
  frags.erase(highest_prec.second, highest_prec.second + 2);

  // Recurse.  :D
  return fragments_to_expr(frags);
}
