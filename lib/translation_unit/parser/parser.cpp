#include <mutiny/mutiny.hpp>
#include <mutiny/translation_unit/parser/parser.hpp>
#include <mutiny/translation_unit/lexer/token.hpp>

using namespace mt;

Parser::Parser(Token& _tokens, Logger& _log_out, Logger& _log_err, Logger& _log_warn)
: tokens(_tokens), log_out(_log_out), log_err(_log_err), log_warn(_log_warn) { }

Parser::~Parser() {

}

void Parser::exec() noexcept {

}