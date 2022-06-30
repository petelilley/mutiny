namespace mt {

inline ASTNode::ASTNode(Kind kind, SourceLoc location, ValueType value)
: kind(kind), location(location), value(value) { }

inline ASTNode::~ASTNode() = default;

constexpr ASTNode::Kind ASTNode::get_kind() const {
  return kind;
}

} // namespace mt