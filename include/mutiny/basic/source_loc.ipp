namespace mt {

inline SourceLoc SourceLoc::cat(SourceLoc begin, SourceLoc end) {
  SourceLoc loc;
  loc.path = begin.path;
  loc.line_i = begin.line_i;
  loc.col_i = begin.col_i;
  loc.line_f = end.line_f;
  loc.col_f = end.col_f;
  return loc;
}

} // namespace mt