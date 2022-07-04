#include <mutiny/translation_unit/parser/parser.hpp>

using namespace mt;

std::optional<ASTNode> Parser::parse_expr() {
  std::vector<ExprFragment> frags;

  std::optional<ASTNode> tmp_nd;

  std::optional<ExprFragment> frag;

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

  std::optional<ASTNode> expr_nd = std::nullopt;

  if (!expect_unit) {
    if (!frags.empty()) {
      expr_nd = fragments_to_expr(frags);
    }
  }

  return expr_nd;
}

std::optional<ASTNode> Parser::parse_expr_unit() {
  std::optional<ASTNode> unit_nd = std::nullopt;

  do {
    // TODO: Parentheses check.

    // TODO: Check for prefix operators (++, --, &, *, !, +, -, ~).

    Token::Kind kind = comp_token(Token::Kind::IDENTIFIER, Token::Kind::INT_LITERAL, Token::Kind::FLOAT_LITERAL, Token::Kind::STRING_LITERAL, Token::Kind::CHAR_LITERAL);
    if (kind == Token::Kind::UNKNOWN) {
      status.report_syntax(Status::ReportContext::ERROR, src_file, tok_iter->get_location(), fmt::format("{}, expected identifier or literal in an expression unit", unexpected_token(Token::Kind::IDENTIFIER)));
      break;
    }
    else if (kind == Token::Kind::IDENTIFIER) {
      ++tok_iter;
      if (comp_token(Punct::LPAREN) != Punct::UNKNOWN) {
        --tok_iter;
        // Function call.
        // unit_nd = parse_func_call();
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

    // TODO: Check for suffix operators (++, --).
  } while (false);

  return unit_nd;
}

std::optional<ASTNode> Parser::parse_operator() {
  std::optional<ASTNode> op_nd = std::nullopt;
  
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