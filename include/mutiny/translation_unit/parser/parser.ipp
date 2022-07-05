namespace mt {

inline ASTNode& Parser::get_ast() {
  return ast;
}

inline Token::Kind Parser::comp_token(Token::Kind kind) {
  if (tok_iter->get_kind() == kind) {
    return kind;
  }
  return Token::Kind::UNKNOWN;
}

inline Punct Parser::comp_token(Punct punct) {
  if (tok_iter->get_kind() == Token::Kind::PUNCTUATOR && tok_iter->get_value<Punct>() == punct) {
    return punct;
  }
  return Punct::UNKNOWN;
}

inline Keyword Parser::comp_token(Keyword keyword) {
  if (tok_iter->get_kind() == Token::Kind::KEYWORD && tok_iter->get_value<Keyword>() == keyword) {
    return keyword;
  }
  return Keyword::UNKNOWN;
}

inline Token::Kind Parser::peek_comp_token(Token::Kind kind, u32 offset) {
  tok_iter += offset;
  Token::Kind res = comp_token(kind);
  tok_iter -= offset;
  return res;
}

inline Punct Parser::peek_comp_token(Punct punct, u32 offset) {
  tok_iter += offset;
  Punct res = comp_token(punct);
  tok_iter -= offset;
  return res;
}

inline Keyword Parser::peek_comp_token(Keyword keyword, u32 offset) {
  tok_iter += offset;
  Keyword res = comp_token(keyword);
  tok_iter -= offset;
  return res;
}


} // namespace mt