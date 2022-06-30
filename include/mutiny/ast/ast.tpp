namespace mt {

template<typename T>
constexpr T ASTNode::get_value() const {
  return std::get<T>(value);
}

} // namespace mt