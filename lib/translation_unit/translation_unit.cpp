#include <mutiny/translation_unit/translation_unit.hpp>

using namespace mt;

TranslationUnit::TranslationUnit(std::filesystem::path path, b8 warning_as_error)
: src_file(path), status(warning_as_error),
  lexer(src_file, status), parser(status) {

}

TranslationUnit::~TranslationUnit() {

}

TranslationUnit::TranslationUnit(TranslationUnit&&)
: src_file(std::move(src_file)), status(std::move(status)),
  lexer(std::move(lexer)), parser(std::move(parser)) { }

void TranslationUnit::exec_lexer() {
  if (!lexer.exec()) {
    result = Result::INVALID_TOKENS;
  }
}

void TranslationUnit::exec_parser() {
  if (!parser.exec()) {
    result = Result::INVALID_SYNTAX;
  }
}

void TranslationUnit::dump_logs() {
  status.dump_logs();
}