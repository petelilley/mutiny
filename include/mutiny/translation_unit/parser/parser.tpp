namespace mt {

template<typename... Args>
Token::Kind Parser::match_token(Token::Kind kind, Args... others) {
  b8 res = comp_token(kind);
  if (res) return kind;

  return match_token(others...);
}

template<typename... Args>
Punct Parser::match_token_punct(Punct punct, Args... others) {
  b8 res = comp_token_punct(punct);
  if (res) return punct;

  return match_token_punct(others...);
}

template<typename ... Args>
Keyword Parser::match_token_keyword(Keyword keyword, Args... others) {
  b8 res = comp_token_keyword(keyword);
  if (res) return keyword;

  return match_token_keyword(others...);
}

} // namespace mt