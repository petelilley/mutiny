#include <mutiny/mutiny.hpp>
#include <mutiny/translation_unit/lexer/lexer.hpp>
#include <mutiny/translation_unit/lexer/token.hpp>

using namespace mt;


Lexer::Lexer(InputFile& src_file, Logger& log_out, Logger& log_err, Logger& log_warn)
: src_file(src_file), log_out(log_out), log_err(log_err), log_warn(log_warn) { }

Lexer::~Lexer() {

}

void Lexer::exec() noexcept {

}