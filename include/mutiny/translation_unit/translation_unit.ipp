namespace mt {

constexpr TranslationUnit::Result TranslationUnit::get_result() const {
  return result;
}

constexpr const Status& TranslationUnit::get_status() const {
  return status;
}

} // namespace mt