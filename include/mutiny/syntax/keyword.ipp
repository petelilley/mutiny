namespace mt {

inline Keyword KeywordUtil::to_keyword(std::string_view str) {
  return KeywordUtil::to_keyword(str.data(), str.length());
}

} // namespace mt