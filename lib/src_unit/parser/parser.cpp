#include <mutiny/mutiny.hpp>
#include <mutiny/src_unit/parser/parser.hpp>
#include <mutiny/src_unit/lexer/token.hpp>

using namespace mt;

Parser::Parser(InputFile& src_file, Status& status)
: src_file(src_file), status(status), ast(ASTNode::Kind::GLOBAL_SCOPE, SourceLoc::at(src_file.get_path(), 1, 1)) { }

Parser::~Parser() = default;

void Parser::exec(const std::vector<Token>* _tokens) {
  tokens = _tokens;

  tok_iter = tokens->begin();

  std::optional<ASTNode> node;
  while (tok_iter->get_kind() != Token::Kind::END_OF_FILE && status.get_error_num() == 0) {
    node = parse_global_decl();
    if (!node) {
      break;
    }
    ast.add_child(std::move(node.value()));
  }

  ast.set_location(SourceLoc::cat(ast.get_location(), (tok_iter - 1)->get_location()));
}

std::string Parser::unexpected_token(Token::Kind kind) {
  if (tok_iter->get_kind() != kind || kind == Token::Kind::UNKNOWN) {
    return fmt::format("unexpected {}", TokenUtil::to_string(tok_iter->get_kind()));
  }
  else if (kind == Token::Kind::PUNCTUATOR) {
    return fmt::format("unexpected '{}'", PunctUtil::to_string(tok_iter->get_value<Punct>()));
  }
  else if (kind == Token::Kind::KEYWORD) {
    return fmt::format("unexpected '{}'", KeywordUtil::to_string(tok_iter->get_value<Keyword>()));
  }
  else {
    return "unexpected token";
  }
}
