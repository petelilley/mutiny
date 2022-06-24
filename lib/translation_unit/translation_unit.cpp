#include <mutiny/translation_unit/translation_unit.hpp>

using namespace mt;

TranslationUnit::TranslationUnit(std::filesystem::path path) noexcept
: src_file(path),
  log_out(Logger::Stream::STDOUT, Logger::OutputMode::MANUAL),
  log_err(Logger::Stream::STDERR, Logger::OutputMode::MANUAL),
  log_warn(Logger::Stream::STDERR, Logger::OutputMode::MANUAL),
  lexer(src_file, log_out, log_err, log_warn),
  parser(lexer.get_tokens(), log_out, log_err, log_warn) {

}

TranslationUnit::~TranslationUnit() noexcept {

}

TranslationUnit::TranslationUnit(TranslationUnit&&) noexcept
: src_file(std::move(src_file)),
  log_out(std::move(log_out)), log_err(std::move(log_err)), log_warn(std::move(log_warn)),
  lexer(std::move(lexer)), parser(std::move(parser)) { }

void TranslationUnit::exec_lexer() noexcept {
  log_out << "Lexing " << src_file.get_path().c_str() << "..." << std::endl;
}

void TranslationUnit::exec_parser() noexcept {
  log_out << "Parsing " << src_file.get_path().c_str() << "..." << std::endl;
}

void TranslationUnit::dump_logs() noexcept {
  log_out.dump_buffer();
  log_err.dump_buffer();
  log_warn.dump_buffer();
}