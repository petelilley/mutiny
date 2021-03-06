#include <mutiny/util/logger.hpp>

using namespace mt;

Logger::Logger(Stream stream, OutputMode mode)
: std::ostream(this), stream(stream), output_mode(mode) { }

Logger::~Logger() = default;

Logger::Logger(Logger&& other)
: std::ostream(this), stream(other.stream), output_mode(other.output_mode) { }

void Logger::dump_buffer() {
  switch (stream) {
    case Stream::STDOUT:
      std::cout << buffer;
      break;
    case Stream::STDERR:
      std::cerr << buffer;
      break;
  }
  
  buffer.clear();
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
    buffer.push_back(c);
  }

  return 0;
}