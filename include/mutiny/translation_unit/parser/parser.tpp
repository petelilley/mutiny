namespace mt {

template<typename... Args>
Token::Kind Parser::comp_token(Token::Kind kind, Args... others) {
  if (comp_token(kind) != Token::Kind::UNKNOWN) return kind;
  return comp_token(others...);
}

template<typename... Args>
Punct Parser::comp_token(Punct punct, Args... others) {
  if (comp_token(punct) != Punct::UNKNOWN) return punct;
  return comp_token(others...);
}

template<typename ... Args>
Keyword Parser::comp_token(Keyword keyword, Args... others) {
  if (comp_token(keyword) != Keyword::UNKNOWN) return keyword;
  return comp_token(others...);
}

} // namespace mt