namespace mt {

inline ASTNode::ASTNode(Kind kind, SourceLoc location, ValueType value)
: kind(kind), location(location), value(value) { }

inline ASTNode::~ASTNode() = default;

constexpr ASTNode::Kind ASTNode::get_kind() const {
  return kind;
}

inline void ASTNode::add_child(ASTNode child) {
  children.push_back(child);
}

} // namespace mt
