#pragma once

#include <mutiny/mutiny.hpp>

namespace mt {

struct SourceLoc {
  std::filesystem::path path;
  
  // Initial position.
  u64 line_i = 0;
  u64 col_i = 0;
  
  // Final position.
  u64 line_f = 0;
  u64 col_f = 0;

  /**
   * @brief Concatenates two source locations.
   * 
   * @param begin The beginning source location.
   * @param end The ending source location.
   * @return A new source location structture.
   */
  static inline SourceLoc cat(SourceLoc begin, SourceLoc end);
};

} // namespace mt

#include <mutiny/basic/source_loc.ipp>