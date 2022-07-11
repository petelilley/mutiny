namespace mt {

constexpr TranslationUnit::Result TranslationUnit::get_result() const {
  return result;
}

constexpr const Status& TranslationUnit::get_status() const {
  return status;
}

inline std::optional<ASTNode> TranslationUnit::get_ast() {
  if (result != Result::SUCCESS) {
    return std::nullopt;
  }

  return parser.get_ast();
}

} // namespace mt