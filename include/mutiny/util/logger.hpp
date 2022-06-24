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

  Logger(Stream stream, OutputMode mode = OutputMode::AUTO) noexcept;
  ~Logger() noexcept;
  Logger(Logger&&) noexcept;
  Logger(const Logger&) noexcept = delete;

  /**
   * @brief Manually dumps the output buffer to the stream.
   */
  void dump_buffer() noexcept;

private:
  Stream stream;
  OutputMode output_mode;

  s32 overflow(s32 c) noexcept override;

  char buffer[1024] = "";
  u32 buf_len = 0;
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
  
static inline std::ostream& operator<<(std::ostream& os, const LogStyle& style) {
  os << "\033[" << static_cast<u32>(style) << "m";
  return os;
}

} // namespace mt