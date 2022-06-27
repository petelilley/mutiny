namespace mt {

inline Punct PunctUtil::to_punct(std::string_view str) {
  return PunctUtil::to_punct(str.data(), str.length());
}

} // namespace mt