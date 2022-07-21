namespace mt {

constexpr SrcUnit::Result SrcUnit::get_result() const {
  return result;
}

constexpr const Status& SrcUnit::get_status() const {
  return status;
}

inline std::optional<ASTNode> SrcUnit::get_ast() {
  if (result != Result::SUCCESS) {
    return std::nullopt;
  }

  return parser.get_ast();
}

} // namespace mt