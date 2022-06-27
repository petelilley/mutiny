namespace mt {

inline Token::Token(Token::Kind kind, SourceLoc location, Token::ValueType value)
: kind(kind), location(location), value(value) { }

inline Token::~Token() = default;

constexpr Token::Kind Token::get_kind() const {
  return kind;
}

} // namespace mt