namespace mt {

template<typename T>
constexpr T ASTNode::get_value() const {
  return std::get<T>(value);
}

template<typename T>
constexpr void ASTNode::set_value(T _value) {
  value = _value;
}

} // namespace mt
