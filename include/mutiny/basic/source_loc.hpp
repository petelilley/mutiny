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
   * @return The concatenated source location.
   */
  static inline SourceLoc cat(SourceLoc begin, SourceLoc end);

  /**
   * @brief Constructs a structure pointing to a single character.
   * 
   * @param path The path to the source file.
   * @param line The line number.
   * @param col The column number.
   * @return A new source location structure.
   */
  static inline SourceLoc at(std::filesystem::path path, u64 line, u64 col);
};

} // namespace mt

#include <mutiny/basic/source_loc.ipp>