namespace mt {

inline b8 Parser::comp_token(Token::Kind kind) {
  return current_token.base()->get_kind() == kind;
}

inline b8 Parser::comp_token_punct(Punct punct) {
  auto base = current_token.base();
  return base->get_kind() == Token::Kind::PUNCTUATOR && base->get_value<Punct>() == punct;
}

inline b8 Parser::comp_token_keyword(Keyword keyword) {
  auto base = current_token.base();
  return base->get_kind() == Token::Kind::KEYWORD && base->get_value<Keyword>() == keyword;
}

} // namespace mt