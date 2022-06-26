#pragma once

#include <mutiny/mutiny.hpp>

namespace mt {

struct SourceLoc {
  u64 line;
  u64 col;
  u64 len;
};

} // namespace mt