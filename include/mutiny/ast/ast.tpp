namespace mt {

template<typename T>
constexpr T ASTNode::get_value() const {
  return std::get<T>(value);
}

template<typename T>
constexpr void ASTNode::set_value(T _value) {
  value = _value;
}

template<ASTNode::Kind K>
inline std::vector<ASTNode*> ASTNode::get_children_of_kind() {
  std::vector<ASTNode*> matches;
  
  for (auto& child : children) {
    if (child.get_kind() == K) {
      matches.push_back(&child);
    }
  }

  return matches;
}

} // namespace mt
