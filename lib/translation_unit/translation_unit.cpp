#include <mutiny/translation_unit/translation_unit.hpp>

using namespace mt;

TranslationUnit::TranslationUnit(std::filesystem::path path, b8 warning_as_error, b8 dump_ast)
: src_file(path), status(warning_as_error),
  lexer(src_file, status), parser(src_file, status),
  dump_ast(dump_ast) { }

TranslationUnit::~TranslationUnit() = default;

TranslationUnit::TranslationUnit(TranslationUnit&& other)
: src_file(std::move(other.src_file)), status(std::move(other.status)),
lexer(std::move(other.lexer)), parser(std::move(other.parser)) { }

void TranslationUnit::exec_lexer() {
  lexer.exec();
  if (status.get_error_num()) {
    result = Result::INVALID_TOKENS;
  }
  else if (lexer.is_empty()) {
    result = Result::EMPTY_FILE;
  }
}

void TranslationUnit::exec_parser() {
  parser.exec(&lexer.get_tokens());
  if (status.get_error_num()) {
    result = Result::INVALID_SYNTAX;
  }
  else if (dump_ast) {
    parser.get_ast().dump(status.get_log_out());
  }
}

void TranslationUnit::dump_logs() {
  status.dump_logs();
}