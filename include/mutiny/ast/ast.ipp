namespace mt {

inline ASTNode::ASTNode(Kind kind, SourceLoc location, ValueType value)
: kind(kind), location(location), value(value) { }

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

} // namespace mt
