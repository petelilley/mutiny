namespace mt {

inline std::ostream& operator<<(std::ostream& os, const LogStyle& style) {
  os << "\033[" << static_cast<u32>(style) << "m";
  return os;
}

} // namespace mt