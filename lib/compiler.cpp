#include <mutiny/mutiny.hpp>
#include <mutiny/compiler.hpp>
#include <mutiny/util/logger.hpp>

using namespace mt;

Compiler::Compiler()
: log_out(Logger::Stream::STDOUT, Logger::OutputMode::AUTO),
  log_err(Logger::Stream::STDERR, Logger::OutputMode::AUTO),
  log_warn(Logger::Stream::STDERR, Logger::OutputMode::AUTO) { }

Compiler::~Compiler() = default;

void Compiler::setup(u32 argc, const char** argv) {
  std::vector<const char*> args(argv, argv + argc);
  decode_args(args);
  
  if (should_exit) return;
}

s32 Compiler::run() {
  if (should_exit) return static_cast<u32>(exit_code);

  return static_cast<u32>(exit_code);
}