#include <mutiny/src_unit/src_unit.hpp>

using namespace mt;

SrcUnit::SrcUnit(std::filesystem::path path, b8 warning_as_error)
: src_file(path), status(warning_as_error),
  lexer(src_file, status), parser(src_file, status) { }

SrcUnit::~SrcUnit() = default;

SrcUnit::SrcUnit(SrcUnit&& other)
: src_file(std::move(other.src_file)), status(std::move(other.status)),
lexer(std::move(other.lexer)), parser(std::move(other.parser)) { }

void SrcUnit::exec_lexer() {
  lexer.exec();
  if (status.get_error_num()) {
    result = Result::INVALID_TOKENS;
  }
  else if (lexer.is_empty()) {
    result = Result::EMPTY_FILE;
  }
}

void SrcUnit::exec_parser() {
  parser.exec(&lexer.get_tokens());
  if (status.get_error_num()) {
    result = Result::INVALID_SYNTAX;
  }
}

void SrcUnit::dump_logs() {
  status.dump_logs();
}