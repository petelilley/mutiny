namespace mt {

inline SourceLoc SourceLoc::cat(SourceLoc begin, SourceLoc end) {
  return { begin.path, begin.line_i, begin.col_i, end.line_f, end.col_f };
}

inline SourceLoc SourceLoc::at(std::filesystem::path path, u64 line, u64 col) {
  return { path, line, col, line, col };
}

} // namespace mt