#include <mutiny/util/logger.hpp>

using namespace mt;

Logger::Logger(Stream stream, OutputMode mode)
: std::ostream(this), stream(stream), output_mode(mode) { }

Logger::~Logger() = default;

Logger::Logger(Logger&& other)
: std::ostream(this), stream(other.stream), output_mode(other.output_mode) { }

void Logger::dump_buffer() {
  if (!buf_len) return;
  
  switch (stream) {
    case Stream::STDOUT:
      std::cout << buffer;
      break;
    case Stream::STDERR:
      std::cerr << buffer;
      break;
  }

  *buffer = 0;
  buf_len = 0;
}

s32 Logger::overflow(s32 c) {
  if (output_mode == OutputMode::AUTO) {
    switch (stream) {
      case Stream::STDOUT:
        std::cout << static_cast<char>(c);
        break;
      case Stream::STDERR:
        std::cerr << static_cast<char>(c);
        break;
    }
  }
  else {
    buffer[buf_len++] = c;
  }

  return 0;
}