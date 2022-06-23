#pragma once

#include <mutiny/mutiny.hpp>

namespace mt {

struct Option {
  const u32 index;
  const char* name;
  const char short_name;
  const b8 requires_arg;
  const u32 context;
  const char* help;
};

struct InputOption {
  const Option& option;
  const std::string arg = nullptr;
};

} // namespace mt