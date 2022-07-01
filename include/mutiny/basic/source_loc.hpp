#pragma once

#include <mutiny/mutiny.hpp>

namespace mt {

struct SourceLoc {
  u64 line = 0;
  u64 col = 0;
  u64 len = 0;
};

} // namespace mt