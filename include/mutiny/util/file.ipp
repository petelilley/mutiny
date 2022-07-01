namespace mt {

constexpr b8 InputFile::is_open() const {
  return open;
}

constexpr InputFile::operator bool() const {
  return is_open();
}

inline std::filesystem::path InputFile::get_path() const {
  return path;
}

inline const c8* InputFile::get_path_str() const {
  return path.c_str();
}

inline InputFile::const_iterator& InputFile::const_iterator::operator++() {
  if (iter == file.content.cend()) return *this;

  if (*iter == '\n') {
    line++;
    column = 1;
  }
  else {
    column++;
  }
  ++iter;

  return *this;
}

inline InputFile::const_iterator InputFile::const_iterator::operator++(int) {
  const_iterator result(file, iter);
  ++(*this);
  return result;
}

inline InputFile::const_iterator& InputFile::const_iterator::operator--() {
  if (iter == file.content.cbegin()) return *this;

  if (column == 1) {
    line--;
    column = file.line_locations.at(line + 1) - file.line_locations.at(line);
  }
  else {
    column--;
  }
  --iter;

  return *this;
}

inline InputFile::const_iterator InputFile::const_iterator::operator--(int) {
  const_iterator result(file, iter);
  --(*this);
  return result;
}

inline InputFile::const_iterator InputFile::const_iterator::operator+(u64 n) {
  return const_iterator(file, iter + n);
}

inline InputFile::const_iterator InputFile::const_iterator::operator-(u64 n) {
  return const_iterator(file, iter - n);
}

inline InputFile::const_iterator InputFile::const_iterator::operator+=(u64 n) {
  for (u64 i = 0; i < n; i++) {
    ++(*this);
  }
  return *this;
}

inline InputFile::const_iterator InputFile::const_iterator::operator-=(u64 n) {
  for (u64 i = 0; i < n; i++) {
    --(*this);
  }
  return *this;
}

inline b8 InputFile::const_iterator::operator==(const_iterator other) {
  return iter == other.iter;
}

inline b8 InputFile::const_iterator::operator!=(const_iterator other) {
  return iter != other.iter;
}

inline const c8& InputFile::const_iterator::operator*() {
  return *iter;
}

constexpr u64 InputFile::const_iterator::line_num() const {
  return line;
}

constexpr u64 InputFile::const_iterator::column_num() const {
  return column;
}

inline InputFile::const_iterator InputFile::cbegin() {
  return const_iterator(*this);
}

inline InputFile::const_iterator InputFile::cend() {
  return const_iterator(*this, content.cend());
}

} // namespace mt