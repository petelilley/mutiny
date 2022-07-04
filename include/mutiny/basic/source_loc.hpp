#pragma once

#include <mutiny/mutiny.hpp>

namespace mt {

struct SourceLoc {
  std::filesystem::path path;
  u64 line_i = 0;
  u64 col_i = 0;
  u64 line_f = 0;
  u64 col_f = 0;

  static inline SourceLoc cat(SourceLoc begin, SourceLoc end);
};

} // namespace mt

#include <mutiny/basic/source_loc.ipp>