#pragma once

#include <mutiny/mutiny.hpp>

namespace mt {

class Logger : private std::streambuf, public std::ostream {
public:
  enum class Stream {
    STDOUT = 0,
    STDERR = 1,
  };

  enum class OutputMode {
    AUTO = 0,
    MANUAL = 1,
  };

  Logger(Stream stream, OutputMode mode = OutputMode::AUTO);
  ~Logger();
  Logger(Logger&&);
  Logger(const Logger&) = delete;

  /**
   * @brief Manually dumps the output buffer to the stream.
   */
  void dump_buffer();

private:
  Stream stream;
  OutputMode output_mode;

  s32 overflow(s32 c) override;

  std::string buffer;
};

enum class LogStyle {
  CLEAR   = 0,
  BOLD    = 1,
  RED     = 31,
  GREEN   = 32,
  YELLOW  = 33,
  BLUE    = 34,
  MAGENTA = 35,
  CYAN    = 36,
  WHITE   = 37,
};

static inline std::ostream& operator<<(std::ostream& os, const LogStyle& style);

} // namespace mt

#include <mutiny/util/logger.ipp>