#include <mutiny/mutiny.hpp>
#include <mutiny/translation_unit/parser/parser.hpp>
#include <mutiny/translation_unit/lexer/token.hpp>

using namespace mt;

Parser::Parser(Logger& log_out, Logger& log_err, Logger& log_warn)
: log_out(log_out), log_err(log_err), log_warn(log_warn) { }

Parser::~Parser() {

}

b8 Parser::exec() {

}