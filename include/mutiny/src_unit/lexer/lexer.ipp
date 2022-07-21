namespace mt {

inline std::vector<Token>& Lexer::get_tokens() {
  return tokens;
}

inline b8 Lexer::is_empty() const {
  return tokens.empty() || tokens.front().get_kind() == Token::Kind::END_OF_FILE;
}

} // namespace mt