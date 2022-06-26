#pragma once

#include <mutiny/mutiny.hpp>
#include <mutiny/util/logger.hpp>
#include <mutiny/util/file.hpp>
#include <mutiny/basic/source_loc.hpp>

namespace mt {

class SyntaxReporter {
public:
  enum class Context {
    NONE,
    NOTE,
    WARNING,
    ERROR,
  };
  
  static void report_syntax(Context context, Logger& log, InputFile& file, SourceLoc location, std::string_view message, std::string_view correction = "");
};

} // namespace mt