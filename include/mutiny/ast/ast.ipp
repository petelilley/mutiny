namespace mt {

inline ASTNode::ASTNode(Kind kind, SourceLoc location, ValueType value)
: kind(kind), location(location), value(value) { }

inline ASTNode::ASTNode(std::vector<ASTNode>&& children)
: kind(Kind::GLOBAL_SCOPE), location(SourceLoc::at("", 1, 1)), value(ValueType()), children(std::move(children)) { }

inline ASTNode::~ASTNode() = default;

constexpr ASTNode::Kind ASTNode::get_kind() const {
  return kind;
}

inline SourceLoc ASTNode::get_location() const {
  return location;
}

inline void ASTNode::set_location(SourceLoc loc) {
  location = loc;
}

inline void ASTNode::add_child(ASTNode child) {
  children.push_back(child);
}

inline std::vector<ASTNode>& ASTNode::get_children() {
  return children;
}

} // namespace mt
