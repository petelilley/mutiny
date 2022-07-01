#pragma once

#include <mutiny/mutiny.hpp>

namespace mt {

struct SourceLoc {
  std::filesystem::path path;
  u64 line = 0;
  u64 col = 0;
  u64 len = 0;
};

} // namespace mt