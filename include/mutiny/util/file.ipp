namespace mt {

constexpr b8 InputFile::is_open() const {
  return open;
}

inline const c8* InputFile::get_path() const {
  return path.c_str();
}

constexpr InputFile::operator b8() const {
  return is_open();
}

constexpr u64 InputFile::get_line_num() const {
  return line;
}

constexpr u64 InputFile::get_column_num() const {
  return column;
}

inline const c8& InputFile::current() const {
  if (is_open() && pos < content.length()) {
    return content.at(pos);
  }
  return npos;
}

inline const c8& InputFile::operator++() {
  if (is_open()) {
    return next();
  }
  return npos;
}

inline const c8& InputFile::operator--() {
  if (is_open()) {
    return prev();
  }
  return npos;
}

inline const c8& InputFile::operator++(s32) {
  if (is_open()) {
    operator++();
    return peek_prev();
  }
  return npos;
}

inline const c8& InputFile::operator--(s32) {
  if (is_open()) {
    operator--();
    return peek_next();
  }
  return npos;
}

inline const c8& InputFile::operator+(u64 n) {
  if (is_open()) {
    return peek_next(n);
  }
  return npos;
}

inline const c8& InputFile::operator-(u64 n) {
  if (is_open()) {
    return peek_prev(n);
  }
  return npos;
}

inline const c8& InputFile::operator+=(u64 n) {
  if (is_open()) {
    return next(n);
  }
  return npos;
}

inline const c8& InputFile::operator-=(u64 n) {
  if (is_open()) {
    return prev(n);
  }
  return npos;
}

} // namespace mt